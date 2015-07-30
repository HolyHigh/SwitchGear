#coding=gbk
import socket  
import threading  
import time  
import struct
DEFAULTADDR = ('127.0.0.1',8964)

#���������˿�
sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.bind(DEFAULTADDR)
#��ʼ����
sock.listen(5)

print "Waiting..."
while True:
    #������������
    conn,addr=sock.accept()
    print "Connection from:",addr
    #�����ļ���Ϣ���ݽṹ
    FILEINFO = struct.calcsize('128sI')
    while True:
        try:
            fhead = conn.recv(FILEINFO)#����ļ���Ϣ
            filename,filesize = struct.unpack('128sI',fhead)
            print "Filename",filename.strip('\00')
            print "Filesize",filesize
            filename = "dup_"+filename.strip('\00')
            fp = open(filename,'wb')#���ش����ļ�
            print "Recving..."
            
            #�����ļ�
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
            conn.close#���ӶϿ����رո�����
            break
        
