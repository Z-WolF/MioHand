import serial
from PyoConnectLib import *

isMyoConnected = False

myo = Myo(sys.argv[1] if len(sys.argv) >= 2 else None)
arduino = serial.Serial('/dev/ttyACM0', 9600)


def ConnectMyo():
    global isMyoConnected
    if isMyoConnected:
        myo.disconnect()
        time.sleep(1)
    myo.connect()
    isMyoConnected = True


def DisconnectMyo():
    global isMyoConnected
    if isMyoConnected:
        myo.disconnect()
        isMyoConnected = False
        print('Myo disconnected')
    else:
        print('Myo not connected')


def printeateste(pose, edge):
    if edge is "on":
        print("Pose: {}".format(pose))
        arduino.write(bytes(PoseToNum(pose), 'utf-8'))


def deslockeo():
    myo.rotSetCenter()
    myo.unlock("hold")

def PoseToNum(posestr):
    if posestr == 'rest':
        return '7'
    if posestr == 'fist':
        return '1'
    if posestr == 'waveIn':
        return '2'
    if posestr == 'waveOut':
        return '3'
    if posestr == 'fingersSpread':
        return '4'
    if posestr == 'doubleTap':
        return '5'
    return 'unknown'


ConnectMyo()
myo.Add_onUnlock(lambda: deslockeo())
myo.Add_onPoseEdge(lambda pose, edge: printeateste(pose, edge))

while True:
    if isMyoConnected:
        t0 = time.time()
        p = myo.run(1.0)
        if time.time() - t0 > 1:
            DisconnectMyo()
        else:
            myo.tick()