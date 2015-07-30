# Create your views here.
#coding=utf-8

import datetime
from math import log
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.db.models import Avg
from django.shortcuts import render_to_response,HttpResponse
from django.template import RequestContext,Context
from django.utils import simplejson
from mysite.apps.CVoltage.models import switch,station,cvoltage,cabinet_station,UserInfo,groupInfo
from django.contrib.auth.models import User
from socket import *
from time import ctime
def index(request):
    '''
    homepage for recommendation application
    '''
    #request.session.set_test_cookie()
    if request.user.is_authenticated():
        return render_to_response('showInfo/index.html',RequestContext(request))
    else:
        response=HttpResponse()
        response.write("<script>alert('请先登录'),window.location.href='/homepage/index/'</script>")
        return response







#def showStation(request):
#    stations =station.objects.all()
#
#    incidentjson = {}
#    i = 0
#    for item in stations:
#        i = i + 1
#        incident = {}
#        incident["sid"]=item.sid
#        incident["sname"] = u"%s" %item.sname
#
#        cabinets=cvoltage.objects.all()
##        cabinets=cabinets.filter(sid__contains=item.sid)
#
#        cab={}
#        j=0
#        for citem in cabinets:
#            j = j + 1
#            cab[j]=citem.cid
#
#        incident["citem"] = cab
#        incidentjson[i] = incident
#    jsonstr = {"incident":incidentjson}
#    return HttpResponse(simplejson.dumps(jsonstr))



def showStation(request):
    if request.user.is_superuser:
        stations =station.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)

#    stations =station.objects.all()
    incidentjson = {}
    i = 0
    for item in stations:
        i = i + 1
        incident = {}
        incident["sid"]=item.sid
        incident["sname"] = u"%s" %item.sname
        incidentjson[i] = incident
    jsonstr = {"incident":incidentjson}
    return HttpResponse(simplejson.dumps(jsonstr))

def showSwitchgear(request):
    station_cabinets=cabinet_station.objects.all()
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        station_cabinets=station_cabinets.filter(sid=station.objects.get(sid=sid))
    gearJson={}
    i = 0
    for item in station_cabinets:
        i=i+1
        incident = {}
        incident["cid"]=item.cid
        incident["type_gui"]=item.type_voltage
        gearJson[i] = incident
    jsonstr = {"gears":gearJson}
    return HttpResponse(simplejson.dumps(jsonstr))

def showGear(request):
    cvoltages = cvoltage.objects.all()
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        cvoltages=cvoltages.filter(sid=station.objects.get(sid=sid))
    return render_to_response('CVoltage/showGear.html',locals(),RequestContext(request))


def showSwitch(request):
    cvoltages = cvoltage.objects.all()
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        cvoltages=cvoltages.filter(sid=station.objects.get(sid=sid))
    return render_to_response('Switch/showSwitch.html',locals(),RequestContext(request))









def switch_contral(request):
    response = HttpResponse()
    incident = {}
    try:
        if request.method == 'POST':
#            req = simplejson.loads(request.body)
            incident["suo_id"] = request.POST['suo_id']
            incident["gui_id"] = request.POST['gui_id']
            incident["switch_id"] = request.POST['switch_id']

            HOST='127.0.0.1'
            PORT=21590
            BUFSIZ=1024
            ADDR=(HOST,PORT)
            client=socket(AF_INET,SOCK_DGRAM)
            while True:
                client.sendto(incident)
                data,ADDR=client.recvfrom(BUFSIZ)
                if not data:
                    break

            client.close()

    except Exception,ex:
        print Exception,":",ex
        print "error"
        response.write("error")
        return response



    response.write("success")
    return response








