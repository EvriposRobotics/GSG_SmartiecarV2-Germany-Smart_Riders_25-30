import cv2
import numpy as np

imgcount = 0
# set the video resolution (height, width)
frameWidth = 320
frameHeight = 240

# empty cam obj
cam = None

# green and red trackbar values as globally accessable variables
G_h_min = 0
G_h_max = 0
G_s_min = 0
G_s_max = 0
G_v_min = 0
G_v_max = 0

R_h_min = 0
R_h_max = 0
R_s_min = 0
R_s_max = 0
R_v_min = 0
R_v_max = 0


def startCam():
    global cam
    cam = cv2.VideoCapture(0)
    cam.set(cv2.CAP_PROP_FRAME_WIDTH, frameWidth)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, frameHeight)


def getImg():
    ret, imgraw = cam.read()
    imgraw2 = cv2.flip(imgraw, -1)
    # crop Region of Interest img[y:y+h, x:x+w]
    img_h = round(0.56 * frameHeight)
    img_w = frameWidth
    img = imgraw[0:img_h, 0:img_w]
    img2 = cv2.flip(img, -1)

    return img2


def procImg():
    return


def load_params_from_file(filename, trackbar_name, window_name, index, default_value):
    try:
        with open(filename, "r") as f:
            param_str = f.read()
            txt = param_str.split("\n")
            if len(txt) <= index:
                value = default_value
            else:
                txt2 = txt[index].split(" = ")
                if len(txt2) == 2:
                    value = int(txt2[1])
                else:
                    value = default_value
    # if the file is not found or the index is out of range, set the value to default
    except (FileNotFoundError, IndexError, ValueError):
        value = default_value

    return value


# reads the trackbar values
def init():
    global G_h_min
    global G_h_max
    global G_s_min
    global G_s_max
    global G_v_min
    global G_v_max
    global R_h_min
    global R_h_max
    global R_s_min
    global R_s_max
    global R_v_min
    global R_v_max
    # hard coded paths
    file_path1 = "/home/pi/Desktop/tools"
    file_path2 = "/home/pi/Desktop/tools"

    # opens the file "G_params.txt" and reads the trackbar positions
    G_h_min = load_params_from_file(
        file_path1, "G HUE Min", "G_HSV", 0, default_value=0
    )
    G_h_max = load_params_from_file(
        file_path1, "G HUE Max", "G_HSV", 1, default_value=179
    )
    G_s_min = load_params_from_file(
        file_path1, "G SAT Min", "G_HSV", 2, default_value=0
    )
    G_s_max = load_params_from_file(
        file_path1, "G SAT Max", "G_HSV", 3, default_value=255
    )
    G_v_min = load_params_from_file(
        file_path1, "G VAL Min", "G_HSV", 4, default_value=0
    )
    G_v_max = load_params_from_file(
        file_path1, "G VAL Max", "G_HSV", 5, default_value=255
    )

    # opens the file "R_params.txt" and reads the trackbar positions
    R_h_min = load_params_from_file(
        file_path2, "R HUE Min", "R_HSV", 0, default_value=0
    )
    R_h_max = load_params_from_file(
        file_path2, "R HUE Max", "R_HSV", 1, default_value=179
    )
    R_s_min = load_params_from_file(
        file_path2, "R SAT Min", "R_HSV", 2, default_value=0
    )
    R_s_max = load_params_from_file(
        file_path2, "R SAT Max", "R_HSV", 3, default_value=255
    )
    R_v_min = load_params_from_file(
        file_path2, "R VAL Min", "R_HSV", 4, default_value=0
    )
    R_v_max = load_params_from_file(
        file_path2, "R VAL Max", "R_HSV", 5, default_value=255
    )
