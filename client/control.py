import socket
import keyboard
import pygame
from time import sleep as wait
from time import time as now
from threading import Thread


def tupToStr(A):
    return str(f"({A[0]},{A[1]},{A[2]},{A[3]},{A[4]},{A[5]})")


def prepToSend(A):
    return (A + ";").encode()


def check_keys():
    AX, AY, AR = 0, 0, 0
    if keyboard.is_pressed("w"):
        AY += 100
    if keyboard.is_pressed("a"):
        AX += 80
    if keyboard.is_pressed("s"):
        AY -= 100
    if keyboard.is_pressed("d"):
        AX -= 80
    if keyboard.is_pressed("q"):
        AR += 100
    if keyboard.is_pressed("e"):
        AR -= 100
    return (AX, AY, AR)


def check_controller():
    global pastBX, pastBY, global_speed
    pygame.event.get()

    AX = int(joystick.get_axis(0) * 10) * 10
    AY = int(joystick.get_axis(1) * -10) * 10

    if joystick.get_button(8) > 0:
        BX = 180 - int(90 * joystick.get_axis(2) + 90)
        BY = 180 - int(-45 * joystick.get_axis(3) + 45)
    else:
        BX = pastBX
        BY = pastBY

    R = int((joystick.get_axis(5) + 1) / 2 * 11) * 10
    L = int((joystick.get_axis(4) + 1) / 2 * 11) * 10
    R = R - L
    OP = min(joystick.get_button(9) + joystick.get_button(10), 1)
    OP += joystick.get_button(15) * 2
    pastBX = BX
    pastBY = BY

    if joystick.get_button(11) > 0:
        global_speed += 0.000001
    if joystick.get_button(12) > 0:
        global_speed -= 0.000001
    global_speed = min(1, max(0, global_speed))
    # print("speed: " + str(global_speed))

    return kneeCapV2((AX, AY, BX, BY, R, OP), global_speed)


def kneeCapV1(A: tuple, lim: int = 40) -> tuple:
    B = list(A)
    for i in range(len(B)):
        B[i] = min(lim, max(-lim, B[i]))

    return tuple(B)


def kneeCapV2(A: tuple, amp: int = 0.4) -> tuple:
    B = list(A)
    for i in range(len(B)):
        B[i] = int(B[i] * amp)
        # B[i] = min(amp, max(-amp, B[i]))

    return tuple(B)


def d(a, b, t=10):
    delta = 0
    for i in range(len(a)):
        delta += abs(a[i] - b[i])
    # delta += abs(a[4] - b[4]) * 50
    return delta > t


def GET_IP():
    return "http://" + socket.gethostbyname(socket.gethostname()) + ":8080/"


pygame.init()
pygame.joystick.init()
clock = pygame.time.Clock()


pastBX = 0
pastBY = 0

pastKeyInput = (0, 0, 0, 0, 0, 0)
pastConInput = (0, 0, 0, 0, 0, 0)

IMGURL = "http://192.168.166.228"
HOST = input("input the IP: ")  # Replace with ESP32's IP address
PORT = 80
lastSend = 0
global_speed = 50

while True:
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            s.sendall(prepToSend(GET_IP()))
            if pygame.joystick.get_count() > 0:
                joystick = pygame.joystick.Joystick(0)
                joystick.init()
                axes = joystick.get_numaxes()
                while True:
                    conInput = check_controller()
                    if d(conInput, pastConInput) or (now() - lastSend > 1):
                        print(tupToStr(conInput))
                        s.sendall(prepToSend(tupToStr(conInput)))
                        lastSend = now()
                        pastConInput = conInput
                        wait(0.1)
            else:
                import interface_flask.interflask as ifl
                ifl.IMGURL = IMGURL
                t = Thread(target=ifl.run)
                t.start()
                while True:
                    s.sendall(prepToSend(tupToStr(ifl.get_data())))
                    wait(0.2)
            # else:
            # while True:
            #     keyInput = check_keys()
            #     if d(keyInput, pastKeyInput):
            #         s.sendall(prepToSend(tupToStr(keyInput)))
            #         pastKeyInput = keyInput
            #         wait(0.2)

    except ConnectionResetError as e:
        print("Connection reset")
