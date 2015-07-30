#coding=gbk
import socket  
import threading  
import time  
import struct
DEFAULTADDR = ('127.0.0.1',8964)

#建立监听端口
sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.bind(DEFAULTADDR)
#开始监听
sock.listen(5)

print "Waiting..."
while True:
    #建立传输连接
    conn,addr=sock.accept()
    print "Connection from:",addr
    #定义文件信息数据结构
    FILEINFO = struct.calcsize('128sI')
    while True:
        try:
            fhead = conn.recv(FILEINFO)#获得文件信息
            filename,filesize = struct.unpack('128sI',fhead)
            print "Filename",filename.strip('\00')
            print "Filesize",filesize
            filename = "dup_"+filename.strip('\00')
            fp = open(filename,'wb')#本地创建文件
            print "Recving..."
            
            #传输文件
            restsize = filesize
            while True:
                if restsize > 1024:
                    data = conn.recv(1024)
                else:
                    data = conn.recv(restsize)
                if not data:break
                fp.write(data)
                restsize = restsize - len(data)
                if restsize <=0:break
            fp.close()
            print "Recv",filename,"done"
            print "-----------------------------------"
        except:
            print "Connection failed"
            conn.close#连接断开，关闭该连接
            break
        
