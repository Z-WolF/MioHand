from PyoConnectLib import *

isMyoConnected = False

myo = Myo(sys.argv[1] if len(sys.argv) >= 2 else None)


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

def printonio():
    if myo.isUnlocked():
        myo.mouseMove(600+myo.rotYaw()*2000, 500-myo.rotPitch()*2000)
        print("Yaw: {}, Pitch: {}".format(600+myo.rotYaw()*2000, 500-myo.rotPitch()*2000))

def deslockeo():
    myo.rotSetCenter()
    myo.unlock("hold")


ConnectMyo()
myo.Add_onUnlock(lambda: deslockeo())
myo.Add_onPoseEdge(lambda pose, edge: printeateste(pose, edge))
#myo.Add_onPeriodic(lambda: printonio())


while True:
    if isMyoConnected:
        t0 = time.time()
        p = myo.run(1.0)
        if time.time() - t0 > 1:
            DisconnectMyo()
        else:
            myo.tick()
