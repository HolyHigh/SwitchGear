__author__ = 'joe'
__author__ = 'joe'
#coding=utf-8

from socket import *
from time import ctime
import json
#while True:
#    HOST=''
#    PORT=21590
#    BUFSIZ=1024
#    ADDR=(HOST,PORT)
#    udpSerSock=socket(AF_INET,SOCK_DGRAM)
#    udpSerSock.bind(ADDR)
#    print 'waiting for message...'
#    data,addr=udpSerSock.recvfrom(BUFSIZ)
#    udpSerSock.sendto('[%s] %s' %(ctime(),data),addr)
#    print '...received from and returned to:',data,addr
#    print '...test data :',data.strip()
#    udpSerSock.close()

while True:
    HOST=''
    PORT=60000
    BUFSIZ=1024
    ADDR=(HOST,PORT)
    udpSerSock=socket(AF_INET,SOCK_DGRAM)
    udpSerSock.bind(ADDR)
    print 'waiting for message...'
    data,addr= udpSerSock.recvfrom(BUFSIZ)
    data1=json.loads(data)
    #    data,addr=udpSerSock.recvfrom(BUFSIZ)
    udpSerSock.sendto('[%s] %s' %(ctime(),data1),addr)
    print '...received from and returned to:',data1,addr
    print "Type:",data1['TP']
    print "cmdID:",data1['CID']
    print "Datetime:",data1['TM']
    print "DepartmentID:",data1['DID']
    print "SwitchgearID:",data1['SID']
    print "TTL:",data1['TTL']
    print "INT:",data1['INT']
    udpSerSock.close()
