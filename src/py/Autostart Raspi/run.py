from datetime import datetime
import time

# own functions
import lcd
import arduino
import imgProc


def endProgram(status, msg):
    if status == 0:
        mytext = "Done"
        lcd.setText(mytext)
        # white
        lcd.setColor_white()
        imgProc.stopCam()
        exit(0)
    else:
        mytext = msg
        lcd.setColor_red()
        lcd.setText(mytext)
        imgProc.stopCam()
        exit(status)


############################################################################################################################################
# MAIN PROGRAM
############################################################################################################################################


currentDateAndTime = datetime.now()
mytext = "Main program"
lcd.setText(mytext)

# red
lcd.setColor_red()
print("Run started at ", currentDateAndTime)

# start camera from img processor module
ret = imgProc.startCam()

if ret == False:
    endProgram(1, "cam ERR")

# arduino handshake function
ret = arduino.handshake()

if ret == False:
    endProgram(2, "ino ERR")

# yellow for ready
lcd.setColor_yellow()

# set the text
lcd.print("ready\n")

print("start loop")

while True:
    # get image from img processor module
    img = imgProc.getImg()

    # process image from img processor module
    data = imgProc.procImg(img)

    # check if arduino requested data
    arduino.answer2req(data)
########################################################
#    Program end
########################################################
endProgram(0, "OK")
