#coding=gbk
import socket  
import time  
import struct  
import os

DEFAULTADDR = ('127.0.0.1',8964)

#�����׽��ֶ���
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)  
sock.settimeout(5)

err_flag = False
try:  
    sock.connect(DEFAULTADDR)  #��������
    print 'Connecting...'  
except socket.timeout:  
    print 'timeout'
    err_flag = True
except socket.error:  
    print 'error'
    err_flag = True
if not err_flag:
    FILEINFO = struct.calcsize('128sI') #�����ļ���Ϣ
    while True:
        while True:
            try:
                filename = raw_input("Enter your filename:")
                fp = open(filename,'rb')
                if fp:break
            except:
                print "File not found.Retype please:"

        fhead = struct.pack('128sI',filename,os.stat(filename).st_size)#����ļ���Ϣ
        sock.send(fhead)#�����ļ���Ϣ
        print "Sending..."
        while True:#�����ļ�
            data = fp.read(1024)
            if not data:break
            sock.send(data)
        fp.close()        #�ļ�������ϣ��ر��ļ�ָ��
        print "Sending ", filename," finished"
