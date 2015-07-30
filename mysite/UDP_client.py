__author__ = 'joe'
#coding=utf-8
from socket import *
from time import ctime

HOST='localhost'
PORT=21590
BUFSIZ=1024
ADDR=(HOST,PORT)
client=socket(AF_INET,SOCK_DGRAM)
info={"sid":"123","cid":"23","snum":"45"}
while True:
    client.sendto(repr(info),ADDR)
    data,ADDR=client.recvfrom(BUFSIZ)
    if not data:
        break
    print data
client.close()