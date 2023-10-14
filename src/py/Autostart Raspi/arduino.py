import time
import serial
import serial.tools.list_ports

import lcd

# empty serial obj for all funcs
ser = None


def handshake():
    global ser

    try:
        # connect arduino
        ports = serial.tools.list_ports.comports()
        for port, desc, hwid in sorted(ports):
            print(port, desc)
            if desc == "USB Serial":
                arduino_port = port

        # open serial port
        ser = serial.Serial(arduino_port, 9600, timeout=1)
        ser.reset_input_buffer()

        # Wait until arduino has reset
        print("Wait for arduino reset")
        while ser.in_waiting == 0:
            time.sleep(1)

        # call ready status
        line = ser.readline().decode("utf-8").rstrip()
        ser.write(b"ready!\n")
        return True

    except:
        print("arduino err")
        return False


def answer2req(data):
    if ser.in_waiting > 0:
        line = ser.readline().decode("utf-8").rstrip()
        lcd.clear()
        lcd.setText(line)
        ser.write(data.encoding("utf-8"))
