__author__ = 'joe'
#coding=utf-8
import urllib
import urllib2
import json
import time,datetime

cTime=datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


url='http://127.0.0.1/CVoltage/updateCV/'
upload_data={
    'TP':'upload',
    'CID':'123',
    'TM':cTime,
    'DID':'s001',
    'SID':'c100',
    'SGS':
        [
            {
                'SID':'c100',
                'Ua':'220',
                'Ub':'220',
                'Uc':'220',
                'Ia':'2.1',
                'Ib':'0.2',
                'Ic':'0.4',
                'PFa':'11.1',
                'PFb':'12.1',
                'PFc':'13.1',
                },

            {
                'SID':'c100',
                '00':0,
                '01':1,
                '02':0,
                '03':1,
                '04':1,
                '05':1,
                '06':0,
                '07':1,
                '08':0,
                '09':1,
                '10':1,
                '11':0,
                '12':1,
                '13':0,
                '14':1,
                '15':1,


                }


        ]
    }

#values = "{'inum':'inum';'Ua':'123'}"

data_json=json.dumps(upload_data)   #用json传送
print data_json
req=urllib2.Request(url,data_json)   #发送请求同时传data表单
response=urllib2.urlopen(req)   #接受反馈的信息
the_page=response.read()       #读取反馈的内容