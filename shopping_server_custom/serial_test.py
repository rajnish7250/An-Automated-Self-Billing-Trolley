import serial
import time

data = serial.Serial(
                  'COM7',
                  baudrate = 9600,
                  parity=serial.PARITY_NONE,
                  stopbits=serial.STOPBITS_ONE,
                  bytesize=serial.EIGHTBITS,                  
                  timeout=1
                  )


def scan_phone():
    while True:
        d = data.readline()
        d = d.decode('utf-8', 'ignnore')
        if 'Phone Num:' in d:
            print(d)
            phone = d.replace('Phone Num:', '')
            break
    return phone
    
def scan_weight():
    rfid = []
    weight = []
    while True:
        d = data.readline()
        d = d.decode('utf-8', 'ignnore')
        d = d.strip()
        if len(d) > 0:
            print(d)
            if d == 'E':
                break
            else:
                if len(d) == 12:
                    rfid.append(d)
                elif '.' in d:
                    d = d.replace('Weight: ', '')
                    d = d.replace('gms', '')
                    weight.append(d)
    return rfid, weight