__author__ = 'joe'
#coding=utf-8
import urllib
import urllib2

url='http://127.0.0.1:80/Switch/Scontral/'

values={'suo_id':'123',
        'gui_id':'456',
        'switch_id':'789',
        }
#values = "{'inum':'inum';'Ua':'123'}"
data=urllib.urlencode(values)    #编码工作
req=urllib2.Request(url,data)   #发送请求同时传data表单
response=urllib2.urlopen(req)   #接受反馈的信息
the_page=response.read()       #读取反馈的内容