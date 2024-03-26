from ultralytics import YOLO
import os
# os.environ["CUDA_VISIBLE_DEVICES"]="0,1"

root = os.getcwd()
## 配置文件路径
name_yaml             = os.path.join(root, "D:/yolov8/ultralytics/yolo/data/datasets/VOC.yaml")
name_pretrain         = os.path.join(root, "yolov8s.pt")
## 原始训练路径
path_train            = os.path.join(root, "runs/detect/VOC")
name_train            = os.path.join(path_train, "weights/last.pt")
## 约束训练路径、剪枝模型文件
path_constraint_train = os.path.join(root, "runs/detect/VOC_Constraint")
name_prune_before     = os.path.join(path_constraint_train, "weights/last.pt")
name_prune_after      = os.path.join(path_constraint_train, "weights/last_prune.pt")
## 微调路径
path_fineturn         = os.path.join(root, "runs/detect/VOC_finetune")

def else_api():
    path_data = ""
    path_result = ""
    model = YOLO(name_pretrain)
    metrics = model.val()  # evaluate model performance on the validation set
    model.export(format='onnx', opset=11, simplify=True, dynamic=False, imgsz=640)
    model.predict(path_data, device="0", save=True, show=False, save_txt=True, imgsz=[288,480], save_conf=True, name=path_result, iou=0.5)  # 这里的imgsz为高宽

def step1_train():
    model = YOLO(name_pretrain)
    model.train(data=name_yaml, device="0,1", imgsz=640, epochs=50, batch=32, workers=16, save_period=1, name=path_train)  # train the model

def step2_Constraint_train():
    model = YOLO(name_train)
    model.train(data=name_yaml, device="0,1", imgsz=640, epochs=50, batch=32, workers=16, save_period=1,name=path_constraint_train)  # train the model

def step3_pruning():
    from LL_pruning import do_pruning
    do_pruning(os.path.join(name_prune_before, name_prune_after))

def step4_finetune():
    model = YOLO(name_prune_after)     # load a pretrained model (recommended for training)
    model.train(data=name_yaml, device="0,1", imgsz=640, epochs=50, batch=32, workers=16, save_period=1, name=path_fineturn)  # train the model

step1_train()
# step2_Constraint_train()
# step3_pruning()
# step4_finetune()

