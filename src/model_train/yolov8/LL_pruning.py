from ultralytics import YOLO
import torch
from torch import nn
from ultralytics.nn.modules import Bottleneck, Conv, C2f, SPPF, Detect
from ultralytics.nn.change import ParNet_C2f, ParNet_Bottleneck
import os
# os.environ["CUDA_VISIBLE_DEVICES"] = "2"

class PRUNE():
    def __init__(self) -> None:
        self.threshold = None

    def get_threshold(self, model, factor=0.8):
        ws = []
        bs = []
        for name, m in model.named_modules():
            if isinstance(m, torch.nn.BatchNorm2d):
                w = m.weight.abs().detach()
                b = m.bias.abs().detach()
                ws.append(w)
                bs.append(b)
                # print(name, w.max().item(), w.min().item(), b.max().item(), b.min().item())
                # print()
        # keep
        ws = torch.cat(ws)
        self.threshold = torch.sort(ws, descending=True)[0][int(len(ws) * factor)]

    def prune_conv(self, conv1: Conv, conv2: Conv):
        ## a. 根据BN中的参数，获取需要保留的index================
        gamma = conv1.bn.weight.data.detach()
        beta  = conv1.bn.bias.data.detach()
        
        keep_idxs = []
        local_threshold = self.threshold
        while len(keep_idxs) < 8:  ## 若剩余卷积核<8, 则降低阈值重新筛选
            keep_idxs = torch.where(gamma.abs() >= local_threshold)[0]
            local_threshold = local_threshold * 0.5
        n = len(keep_idxs)
        # n = max(int(len(idxs) * 0.8), p)
        print(n / len(gamma) * 100)
        # scale = len(idxs) / n

        
#         print("Before pruning - conv1 weight shape:", conv1.conv.weight.shape)
#         print("Before pruning - gamma shape:", gamma.shape)
#         print("Before pruning - beta shape:", beta.shape)
#         print("Before pruning - keep_idxs length:", len(keep_idxs))
#         print("Before pruning - keep_idxs:", keep_idxs)
        # 在剪枝代码中添加额外的检查

        
          ## b. 利用index对BN进行剪枝============================
        conv1.bn.weight.data = gamma[keep_idxs]
        conv1.bn.bias.data   = beta[keep_idxs]
        conv1.bn.running_var.data = conv1.bn.running_var.data[keep_idxs]
        conv1.bn.running_mean.data = conv1.bn.running_mean.data[keep_idxs]
        conv1.bn.num_features = n
        conv1.conv.weight.data = conv1.conv.weight.data[keep_idxs]
        conv1.conv.out_channels = n
        
        ## c. 利用index对conv1进行剪枝=========================
        if conv1.conv.bias is not None:
            conv1.conv.bias.data = conv1.conv.bias.data[keep_idxs]

        ## d. 利用index对conv2进行剪枝=========================
        if not isinstance(conv2, list):
            conv2 = [conv2]
        for item in conv2:
            if item is None: continue
            if isinstance(item, Conv):
                conv = item.conv
            else:
                conv = item
            conv.in_channels = n
            conv.weight.data = conv.weight.data[:, keep_idxs]
    
    def prune_parnet_bottleneck(self, bottleneck):
        conv1 = bottleneck.cv1
        conv2 = bottleneck.cv2
        self.prune_conv(conv1, conv2)
    
    # def prune(self, m1, m2):
    #     if hasattr(m1, 'cv2'):      # ParNet_C2f as a top conv
    #         m1 = m1.cv2
    #     if not isinstance(m2, list): # m2 is just one module
    #         m2 = [m2]
    #     for i, item in enumerate(m2):
    #         if hasattr(item, 'cv1'):
    #             m2[i] = item.cv1
    #     self.prune_conv(m1, m2)
    
    def prune(self, m1, m2):
        if isinstance(m1, C2f) or isinstance(m1, ParNet_C2f):      # C2f as a top conv
            m1 = m1.cv2
        if not isinstance(m2, list): # m2 is just one module
            m2 = [m2]
        for i, item in enumerate(m2):
            if isinstance(item, C2f) or isinstance(item, SPPF) or isinstance(item, ParNet_C2f):
                m2[i] = item.cv1
        self.prune_conv(m1, m2)


def do_pruning(modelpath, savepath):
    pruning = PRUNE()

    ### 0. 加载模型
    yolo = YOLO(modelpath)                  # build a new model from scratch
    
    if 'prune' != modelpath.split('/')[-1].split('.')[0].split('_')[-1]:
        
        pruning.get_threshold(yolo.model, 0.8)  # 获取剪枝时bn参数的阈值，这里的0.8为剪枝率。14GF -> 10GF

        ### 1. 剪枝c2f 中的Bottleneck
        for name, m in yolo.model.named_modules():
            if isinstance(m, Bottleneck):
                pruning.prune_conv(m.cv1, m.cv2)
            if isinstance(m, ParNet_Bottleneck):
                pruning.prune_parnet_bottleneck(m)

        ### 2. 指定剪枝不同模块之间的卷积核
        seq = yolo.model.model
        for i in [3,5,7]: 
            pruning.prune(seq[i], seq[i+1])

        ### 3. 对检测头进行剪枝
        # 在P3层: seq[15]之后的网络节点与其相连的有 seq[16]、detect.cv2[0] (box分支)、detect.cv3[0] (class分支)
        # 在P4层: seq[18]之后的网络节点与其相连的有 seq[19]、detect.cv2[1] 、detect.cv3[1] 
        # 在P5层: seq[21]之后的网络节点与其相连的有 detect.cv2[2] 、detect.cv3[2] 
        detect:Detect = seq[-1]
        last_inputs   = [seq[15], seq[18], seq[21]]
        colasts       = [seq[16], seq[19], None]
        for last_input, colast, cv2, cv3 in zip(last_inputs, colasts, detect.cv2, detect.cv3):
            pruning.prune(last_input, [colast, cv2[0], cv3[0]])
            pruning.prune(cv2[0], cv2[1])
            pruning.prune(cv2[1], cv2[2])
            pruning.prune(cv3[0], cv3[1])
            pruning.prune(cv3[1], cv3[2])

        ### 4. 模型梯度设置与保存
        for name, p in yolo.model.named_parameters():
            p.requires_grad = True

    yolo.val()
    torch.save(yolo.ckpt, savepath)
    yolo.model.pt_path = yolo.model.pt_path.replace("last.pt", os.path.basename(savepath))
    yolo.export(format="onnx")

    ## 重新load模型，修改保存命名，用以比较剪枝前后的onnx的大小
    yolo = YOLO(modelpath)  # build a new model from scratch
    yolo.export(format="onnx")

    
if __name__ == "__main__":

    modelpath = "runs/prune/locount_Constraint/weights/best_prune.pt"
    savepath  = "runs/prune/locount_Constraint/weights/best_prune.pt"
    do_pruning(modelpath, savepath)
