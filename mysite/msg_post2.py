__author__ = 'joe'
#coding=utf-8
import httplib,urllib  #加载模块

#定义需要进行发送的数据
data = urllib.urlencode({ 'user':'chenjun',
                            'pwd':'HFUT123hfut',
                            'mobiles':'15656008620',
                            'contents':'test',
                            'chid':'0',
                            'sendtime':'',})
#定义一些文件头
headers = {"Content-Type":"application/x-www-form-urlencoded",
           "Connection":"Keep-Alive","Referer":"http://www.106008.com/WebAPI/SmsAPI.asmx/SendSmsExt"}
#与网站构建一个连接
conn = httplib.HTTPConnection("www.106008.com")
#开始进行数据提交   同时也可以使用get进行
conn.request(method="POST",url="/WebAPI/SmsAPI.asmx/SendSmsExt",body=data,headers=headers)
#返回处理后的数据
response = conn.getresponse()
#判断是否提交成功
if response.status == 302:
    print "发布成功!^_^!"
else:
    print "发布失败\^0^/"
#关闭连接
the_page=response.read()
print the_page
conn.close()
