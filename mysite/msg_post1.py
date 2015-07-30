__author__ = 'joe'
#coding=utf-8
import urllib
import urllib2
url = 'http://www.106008.com/WebAPI/SmsAPI.asmx/SendSmsExt'
values = {
    'user':'chenjun',
    'pwd':'HFUT123hfut',
    'mobiles':'15656008620',
    'contents':'test',
    'chid':'0',
    'sendtime':'',
}
data = urllib.urlencode(values)
req = urllib2.Request(url, data)
response = urllib2.urlopen(req)
the_page = response.read()  #the_page是字符串
print the_page
