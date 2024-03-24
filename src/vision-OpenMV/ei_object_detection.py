# Edge Impulse - OpenMV Object Detection Example

import sensor, image, time, os, tf, math, uos, gc
from machine import UART
import json

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.

net = None
labels = None
min_confidence = 0.8
counts = 0

uart = UART(3, 115200)

try:
    # load the model, alloc the model file on the heap if we have at least 64K free after loading
    net = tf.load("trained.tflite", load_to_fb=uos.stat('trained.tflite')[6] > (gc.mem_free() - (64*1024)))
except Exception as e:
    raise Exception('Failed to load "trained.tflite", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

try:
    labels = [line.rstrip('\n') for line in open("labels.txt")]
except Exception as e:
    raise Exception('Failed to load "labels.txt", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

colors = [ # Add more colors if you are detecting more than 7 types of classes at once.
    (255,   0,   0),
    (  0, 255,   0),
    (255, 255,   0),
    (  0,   0, 255),
    (255,   0, 255),
    (  0, 255, 255),
    (255, 255, 255),
]

def non_max_suppression(boxes, threshold):
    if len(boxes) == 0:
        return []

    # 按照得分降序排序
    boxes = sorted(boxes, key=lambda x: x[4], reverse=True)
    # 存储最终通过NMS的边界框
    keep = []
    while len(boxes) > 0:
        # 保留当前得分最高的边界框
        i = 0
        max_score_box = boxes[i]
        keep.append(max_score_box)
        # 计算剩余边界框与当前得分最高的边界框的IoU
        boxes = [box for j, box in enumerate(boxes[1:]) if jaccard_overlap(max_score_box, box) < threshold]

    return keep

def jaccard_overlap(x1,y1,x2,y2,x3,y3x4,y4):
    x1, y1, x2, y2 = box1[:4]
    x3, y3, x4, y4 = box2[:4]

    # 计算相交区域的左上和右下坐标
    inter_x1 = max(x1, x3)
    inter_y1 = max(y1, y3)
    inter_x2 = min(x2, x4)
    inter_y2 = min(y2, y4)

    # 计算相交区域的面积
    inter_area = max(0, inter_x2 - inter_x1) * max(0, inter_y2 - inter_y1)

    # 计算两个框的面积
    box1_area = (x2 - x1) * (y2 - y1)
    box2_area = (x4 - x3) * (y4 - y3)

    # 计算IoU
    iou = inter_area / (box1_area + box2_area - inter_area)

    return iou

clock = time.clock()
while(True):
    clock.tick()

    img = sensor.snapshot()

    # detect() returns all objects found in the image (splitted out per class already)
    # we skip class index 0, as that is the background, and then draw circles of the center
    # of our objects

    for i, detection_list in enumerate(net.detect(img, thresholds=[(math.ceil(min_confidence * 255), 255)])):
        if (i == 0): continue # background class
        if (len(detection_list) == 0): continue # no detections for this class?

#        x1 = 0
#        y1 = 0
#        x2 = 0
#        y2 = 0
#        x3 = 0
#        y3 = 0
#        x4 = 0
#        y4 = 0

        print("********** %s **********" % labels[i])
        for d in detection_list:
            counts += 1
            [x, y, w, h] = d.rect()
#            if(i = 0) #have some question,i isn't define
#                x1 = x
#                y1 = y
#                x2 = x+w
#                y2 = y+h
#            else
#                x3 = x
#                y3 = y
#                x4 = x+w
#                y4 = y+h
            center_x = math.floor(x + (w / 2))
            center_y = math.floor(y + (h / 2))
            print('x %d\ty %d' % (center_x, center_y))
            img.draw_circle((center_x, center_y, 12), color=colors[i], thickness=2)
            print('counts:',counts)
            print('detection:',json.dumps(detection_list))
            str_counts=json.dumps(counts)
            uart.write(str_counts+'\r\n')
            uart.write(json.dumps(labels[i]))
        counts = 0

    print(clock.fps(), "fps", end="\n\n")
