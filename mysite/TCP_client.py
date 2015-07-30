#coding=gbk
import socket  
import time  
import struct  
import os

DEFAULTADDR = ('127.0.0.1',8964)

#创建套接字对象
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)  
sock.settimeout(5)

err_flag = False
try:  
    sock.connect(DEFAULTADDR)  #建立连接
    print 'Connecting...'  
except socket.timeout:  
    print 'timeout'
    err_flag = True
except socket.error:  
    print 'error'
    err_flag = True
if not err_flag:
    FILEINFO = struct.calcsize('128sI') #定义文件信息
    while True:
        while True:
            try:
                filename = raw_input("Enter your filename:")
                fp = open(filename,'rb')
                if fp:break
            except:
                print "File not found.Retype please:"

        fhead = struct.pack('128sI',filename,os.stat(filename).st_size)#打包文件信息
        sock.send(fhead)#传输文件信息
        print "Sending..."
        while True:#传输文件
            data = fp.read(1024)
            if not data:break
            sock.send(data)
        fp.close()        #文件传输完毕，关闭文件指针
        print "Sending ", filename," finished"
