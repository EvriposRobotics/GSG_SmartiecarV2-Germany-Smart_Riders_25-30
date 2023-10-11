from datetime import datetime
import time

# own functions
import lcd
import arduino
import imgProc

currentDateAndTime = datetime.now()
mytext = "Main program"
lcd.setText(mytext)

# red
lcd.setColor_red()
print("Run started at ", currentDateAndTime)

# start camera from img processor module
imgProc.startCam()

# arduino handshake function
arduino.handshake()

# yellow for ready
lcd.setColor_yellow()

# set the text
lcd.print("ready\n")

# start the while loop
while imgcount < 10:
    contrast1 = 3.5
    brightness1 = 0.0
    # increase contrast and brightness with addWeighted
    # img_contrast1 = cv2.convertScaleAbs(img2, alpha=contrast1, beta=brightness1)
    img_contrast1 = cv2.addWeighted(img2, contrast1, img2, 0, brightness1)
    testimg = img_contrast1  # save for debuging
    # cv2.imshow('test', testimg)
    # cv2.waitKey(1)
    filename = "/home/pi/Desktop/Autostart/TestPhotos/img_" + str(imgcount) + ".jpg"
    if not cv2.imwrite(filename, testimg):
        raise Exception("Img write error")
    print(filename)
    imgcount = imgcount + 1
    time.sleep(1)

########################################################
#    Program end
########################################################
mytext = "Done"
lcd.setText(mytext)
# white
lcd.setColor_white()
