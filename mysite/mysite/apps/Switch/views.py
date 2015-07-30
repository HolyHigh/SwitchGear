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
        if request.user.is_superuser:
            stations =station.objects.all()
        else:
            user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
            stations=station.objects.filter(gname=user_login.gname)
        #        stations = station.objects.all()
        stations = stations.order_by('sid')
        return render_to_response('Switch/index.html',locals(),RequestContext(request))
    else:
        response=HttpResponse()
        response.write("<script>alert('请先登录'),window.location.href='/homepage/index/'</script>")
        return response



def info2(request):
    if request.user.is_superuser:
        switchs = switch.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        switchs=switch.objects.filter(sid__in=stations)
#    switchs = switch.objects.all()
    switchs=switchs.order_by('sid')
    for item in switchs:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        switchs=switchs.filter(sid=station.objects.get(sid=sid))
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'cnum'in request.GET and request.GET['cnum']:
        cnum=request.GET['cnum']
        switchs=switchs.filter(cid=cabinet_station.objects.get(cid=cnum))
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'snum'in request.GET and request.GET['snum']:
        snum=request.GET['snum']
        switchs=switchs.filter(snum__contains=snum)
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    count=switchs.count()
    paginator=Paginator(switchs,10)
    try:
        page=int(request.GET.get("page",1))
        if page<1:
            page=1
    except ValueError:
        page=1
    try:
        switchinfo=paginator.page(page)
    except PageNotAnInteger:
        switchinfo=paginator.page(1)
    except EmptyPage:
        switchinfo=paginator.page(paginator.num_pages)
    return render_to_response('Switch/info2.html',locals(),RequestContext(request))

def infoSwitch(request):
    if request.user.is_superuser:
        switchs = switch.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        switchs=switch.objects.filter(sid__in=stations)
    switchs=switchs.order_by('sid')
    for item in switchs:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        switchs=switchs.filter(sid=station.objects.get(sid=sid))
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'cnum'in request.GET and request.GET['cnum']:
        cnum=request.GET['cnum']
        switchs=switchs.filter(cid=cabinet_station.objects.get(cid=cnum))
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'snum'in request.GET and request.GET['snum']:
        snum=request.GET['snum']
        switchs=switchs.filter(snum__contains=snum)
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")

    if 'page' in request.GET and request.GET['page']:
        pagenum = request.GET['page']
    else:
        pagenum = 1
    page = Paginator(switchs, 10)
    SWinfo_json={}
    i = 0
    for item in page.page(pagenum).object_list:
        i=i+1
        SWinfo={}
        SWinfo["sid"]=item.sid.sid
        SWinfo["cid"]=item.cid.cid
        SWinfo["S1"]=item.S1
        SWinfo["S2"]=item.S2
        SWinfo["S3"]=item.S3
        SWinfo["S4"]=item.S4
        SWinfo["S5"]=item.S5
        SWinfo["S6"]=item.S6
        SWinfo["S7"]=item.S7
        SWinfo["S8"]=item.S8
        SWinfo["S9"]=item.S9
        SWinfo["S10"]=item.S10
        SWinfo["S11"]=item.S11
        SWinfo["S12"]=item.S12
        SWinfo["S13"]=item.S13
        SWinfo["S14"]=item.S14
        SWinfo["S15"]=item.S15
        SWinfo["S16"]=item.S16
        SWinfo["update_time"]=item.update_time
        SWinfo_json[i]=SWinfo
    infojson = {"count" : page.count, "numpages" : page.num_pages, "page":pagenum}
    jsonstr = {"incident":SWinfo_json, "info":infojson}
    return HttpResponse(simplejson.dumps(jsonstr))

def info2_mp(request):
    if request.user.is_superuser:
        switchs = switch.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        switchs=switch.objects.filter(sid__in=stations)
#    switchs = switch.objects.all()
    switchs=switchs.order_by('sid')
    for item in switchs:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        switchs=switchs.filter(sid=station.objects.get(sid=sid))
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'cnum'in request.GET and request.GET['cnum']:
        cnum=request.GET['cnum']
        switchs=switchs.filter(cid=cabinet_station.objects.get(cid=cnum))
        for item in switchs:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'snum'in request.GET and request.GET['snum']:
        snum=request.GET['snum']
        switchs=switchs.filter(snum__contains=snum)

    count=switchs.count()
    paginator=Paginator(switchs,10)
    try:
        page=int(request.GET.get("page",1))
        if page<1:
            page=1
    except ValueError:
        page=1
    try:
        switchinfo=paginator.page(page)
    except PageNotAnInteger:
        switchinfo=paginator.page(1)
    except EmptyPage:
        switchinfo=paginator.page(paginator.num_pages)
    i=0
    SWinfo_json=[]
    info={}
    for item in switchinfo:
        i=i+1
        SWinfo={}
        SWinfo["sid"]=item.sid.sid
        SWinfo["cid"]=item.cid.cid
        SWinfo["S1"]=item.S1
        SWinfo["S2"]=item.S2
        SWinfo["S3"]=item.S3
        SWinfo["S4"]=item.S4
        SWinfo["S5"]=item.S5
        SWinfo["S6"]=item.S6
        SWinfo["S7"]=item.S7
        SWinfo["S8"]=item.S8
        SWinfo["S9"]=item.S9
        SWinfo["S10"]=item.S10
        SWinfo["S11"]=item.S11
        SWinfo["S12"]=item.S12
        SWinfo["S13"]=item.S13
        SWinfo["S14"]=item.S14
        SWinfo["S15"]=item.S15
        SWinfo["S16"]=item.S16
        SWinfo["update_time"]=item.update_time
        SWinfo_json.append(SWinfo)
    info["SWinfo"]=SWinfo_json
    info["page"] = page
    info["count"]=count
    return HttpResponse(simplejson.dumps(info))

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








