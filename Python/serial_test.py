import serial

ser = serial.Serial('/dev/ttyACM0', 9600)

while True:
    sent = input('Ingrese comando: ')

    ser.write(bytes(sent, 'utf-8'))
    print(ser.readline().decode())
