# Create your views here.
#coding=utf-8

import datetime
import json
import urllib,urllib2
from xml.dom import minidom
from math import log
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.db.models import Avg
from django.shortcuts import render_to_response,HttpResponse,HttpResponseRedirect
from django.template import RequestContext,Context
from django.utils import simplejson
from mysite.apps.CVoltage.models import cvoltage, station,cabinet_station,exception,UserInfo,groupInfo,setRange
from django.contrib.auth.models import User
from mysite.apps.CVoltage.models import switch
from socket import *
from time import ctime


def index(request):
    #request.session.set_test_cookie()
    if request.user.is_authenticated():
        if request.user.is_superuser:
            stations =station.objects.all()
        else:
            user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
            stations=station.objects.filter(gname=user_login.gname)
#        stations = station.objects.all()
        stations = stations.order_by('sid')
        return render_to_response('CVoltage/index.html',locals(),RequestContext(request))
    else:
        response=HttpResponse()
        response.write("<script>alert('请先登录'),window.location.href='/homepage/index/'</script>")
        return response


def info1(request):
    if request.user.is_superuser:
        cvoltages = cvoltage.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        cvoltages=cvoltage.objects.filter(sid__in=stations)
#    cvoltages = cvoltage.objects.all()
    cvoltages = cvoltages.order_by('sid')
    for item in cvoltages:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        cvoltages=cvoltages.filter(sid=station.objects.get(sid=sid))
        for item in cvoltages:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'dnum'in request.GET and request.GET['dnum']:
        dnum=request.GET['dnum']
        cvoltages=cvoltages.filter(cid=cabinet_station.objects.get(cid=dnum))
        for item in cvoltages:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    count=cvoltages.count()
    paginator=Paginator(cvoltages,10)
    try:
        page=int(request.GET.get("page",1))
        if page<1:
            page=1
    except ValueError:
        page=1
    try:
        cvoltageinfo=paginator.page(page)
    except PageNotAnInteger:
        cvoltageinfo=paginator.page(1)
    except EmptyPage:
        cvoltageinfo=paginator.page(paginator.num_pages)
    max_U=220
    min_U=0
    max_I=50
    min_I=0
    return render_to_response('CVoltage/info1.html',locals(),RequestContext(request))

def infoCV(request):
    if request.user.is_superuser:
        cvoltages = cvoltage.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        cvoltages=cvoltage.objects.filter(sid__in=stations)
    #    cvoltages = cvoltage.objects.all()
    cvoltages = cvoltages.order_by('sid')
    for item in cvoltages:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        cvoltages=cvoltages.filter(sid=station.objects.get(sid=sid))
        for item in cvoltages:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    else:
        sid="-1"
    if 'dnum'in request.GET and request.GET['dnum']:
        dnum=request.GET['dnum']
        cvoltages=cvoltages.filter(cid=cabinet_station.objects.get(cid=dnum))
        for item in cvoltages:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")

    if 'page' in request.GET and request.GET['page']:
        pagenum = request.GET['page']
    else:
        pagenum = 1
    page = Paginator(cvoltages, 10)
    CVinfo_json={}
    i = 0
    for item in page.page(pagenum).object_list:
        i = i + 1
        CVinfo={}
        CVinfo["sid"]=item.sid.sid
        CVinfo["cid"]=item.cid.cid
        CVinfo["Ua"]=item.Ua
        CVinfo["Ub"]=item.Ub
        CVinfo["Uc"]=item.Uc
        CVinfo["Ia"]=item.Ia
        CVinfo["Ib"]=item.Ib
        CVinfo["Ic"]=item.Ic
        CVinfo["PFa"]=item.PFa
        CVinfo["PFb"]=item.PFb
        CVinfo["PFc"]=item.PFc
        CVinfo["update_time"]=item.update_time
        CVinfo_json[i]=CVinfo
    infojson = {"count" : page.count, "numpages" : page.num_pages, "page":pagenum,"sid":sid}
    jsonstr = {"incident":CVinfo_json, "info":infojson}
    return HttpResponse(simplejson.dumps(jsonstr))

def info1_mp(request):
    if request.user.is_superuser:
        cvoltages = cvoltage.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        cvoltages=cvoltage.objects.filter(sid__in=stations)
#    cvoltages = cvoltage.objects.all()
    cvoltages = cvoltages.order_by('sid')
    for item in cvoltages:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'sid'in request.GET and request.GET['sid']:
        sid=request.GET['sid']
        cvoltages=cvoltages.filter(sid=station.objects.get(sid=sid))
        for item in cvoltages:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    if 'dnum'in request.GET and request.GET['dnum']:
        dnum=request.GET['dnum']
        cvoltages=cvoltages.filter(cid=cabinet_station.objects.get(cid=dnum))
        for item in cvoltages:
            item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    count=cvoltages.count()
    paginator=Paginator(cvoltages,10)
    try:
        page=int(request.GET.get("page",1))
        if page<1:
            page=1
    except ValueError:
        page=1
    try:
        cvoltageinfo=paginator.page(page)
    except PageNotAnInteger:
        cvoltageinfo=paginator.page(1)
    except EmptyPage:
        cvoltageinfo=paginator.page(paginator.num_pages)
    i=0
    CVinfo_json=[]
    info={}
    for item in cvoltageinfo:
        i=i+1
        CVinfo={}
        CVinfo["sid"]=item.sid.sid
        CVinfo["cid"]=item.cid.cid
        CVinfo["Ua"]=item.Ua
        CVinfo["Ub"]=item.Ub
        CVinfo["Uc"]=item.Uc
        CVinfo["Ia"]=item.Ia
        CVinfo["Ib"]=item.Ib
        CVinfo["Ic"]=item.Ic
        CVinfo["PFa"]=item.PFa
        CVinfo["PFb"]=item.PFb
        CVinfo["PFc"]=item.PFc
        CVinfo["update_time"]=item.update_time
        CVinfo_json.append(CVinfo)
    info["CVinfo"]=CVinfo_json
    info["page"] = page
    info["count"]=count
    return HttpResponse(simplejson.dumps(info))

def update_cvoltage(request):
    threshold=setRange
    max_U=220
    min_U=0
    max_I=2
    min_I=0
    response = HttpResponse()
    try:
        if request.method == 'POST':
            req = simplejson.loads(request.body)
            print req
            type=req['TP']
            cmdID=req['CID']
            dateTime=req['TM']
            departmentID=req['DID']
#            departmentName=req['DepartmentName']
            switchgearID=req['SID']
            SGS=req['SGS']
            if (len(SGS)==1):
                switchID=SGS[0].get('SID','null')
                Ua=SGS[0].get('Ua','noData')
                Ub=SGS[0].get('Ub','0')
                Uc=SGS[0].get('Uc','0')
                Ia=SGS[0].get('Ia','0')
                Ib=SGS[0].get('Ib','0')
                Ic=SGS[0].get('Ic','0')
                PFa=SGS[0].get('PFa','0')
                PFb=SGS[0].get('PFb','0')
                PFc=SGS[0].get('PFc','0')

                BreakerPos=SGS[0].get('00','noData')
                HandPos=SGS[0].get('01','0')
                TestPos=SGS[0].get('02','0')
                GroundKnife=SGS[0].get('03','0')
                Energy=SGS[0].get('04','0')
                Switch7=SGS[0].get('05','0')
                EA=SGS[0].get('06','0')
                EB=SGS[0].get('07','0')
                EC=SGS[0].get('08','0')
                Lock=SGS[0].get('09','0')
                Sensor1=SGS[0].get('10','0')
                Sensor2=SGS[0].get('11','0')
                Load1=SGS[0].get('12','0')
                Load2=SGS[0].get('13','0')
                Loop1=SGS[0].get('14','0')
                Loop2=SGS[0].get('15','0')

            elif(len(SGS)==2):
                switchID=SGS[0]['SID']
                Ua=SGS[0]['Ua']
                Ub=SGS[0]['Ub']
                Uc=SGS[0]['Uc']
                Ia=SGS[0]['Ia']
                Ib=SGS[0]['Ib']
                Ic=SGS[0]['Ic']
                PFa=SGS[0]['PFa']
                PFb=SGS[0]['PFb']
                PFc=SGS[0]['PFc']
                switchID2=SGS[1]['SID']
                BreakerPos=SGS[1]['00']
                HandPos=SGS[1]['01']
                TestPos=SGS[1]['02']
                GroundKnife=SGS[1]['03']
                Energy=SGS[1]['04']
                Switch7=SGS[1]['05']
                EA=SGS[1]['06']
                EB=SGS[1]['07']
                EC=SGS[1]['08']
                Lock=SGS[1]['09']
                Sensor1=SGS[1]['10']
                Sensor2=SGS[1]['11']
                Load1=SGS[1]['12']
                Load2=SGS[1]['13']
                Loop1=SGS[1]['14']
                Loop2=SGS[1]['15']

            if(departmentID !=''and switchgearID !='' ):
                try:
                    print "start"
                    d1=station.objects.get(sid=departmentID)

                except station.DoesNotExist:
                    print "departmentID DoesNotExist"
                    response.write("<script>alert('所序号不在数据库中'),window.location.href='/CVoltage/index/'</script>")
                    return response

#                station1=station(sid=departmentID)
#                station1.save()
                else:
                    try:
                        if(Ua=='noData' and BreakerPos != 'noData'):
                            sw_set=switch.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                            if sw_set.exists():
                                sw_set.update(S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                            else:
                                switch1=switch(sid=station.objects.get(sid=departmentID),cid=cabinet_station.objects.get(cid=switchgearID),S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                switch1.save()
                            print "update Switch Info!"
                        elif(Ua !='noData' and BreakerPos == 'noData'):
                            if(float(Ua)>max_U or float(Ub>max_U) or float(Uc)>max_U):
                                exception1=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电压过大" )
                                exception1.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                print "voltage_high exception!"
                            elif(float(Ua)<min_U or float(Ub)<min_U or float(Uc)<min_U):
                                exception3=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电压过小" )
                                exception3.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                print "voltage_low exception!"

                            elif(float(Ia)>max_I or float(Ib)>max_I or float(Ic)>max_I):
                                exception2=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电流过大" )
                                exception2.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                print "current exception!"
                            elif(float(Ia)<min_I or float(Ib)<min_I or float(Ic)<min_I):
                                exception4=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电流过小" )
                                exception4.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                print "current_low exception!"
                            else:
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                print "update UI Info without error!"
                        else:
                            if(float(Ua)>max_U or float(Ub)>max_U or float(Uc)>max_U):
                                exception1=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电压过大" )
                                exception1.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                sw_set=switch.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if sw_set.exists():
                                    sw_set.update(S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                else:
                                    switch1=switch(sid=station.objects.get(sid=departmentID),cid=cabinet_station.objects.get(cid=switchgearID),S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                    switch1.save()

                                print "exception exist-voltage exception"
                            elif(float(Ua)<min_U or float(Ub)<min_U or float(Uc)<min_U):
                                exception3=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电压过小" )
                                exception3.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                sw_set=switch.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if sw_set.exists():
                                    sw_set.update(S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                else:
                                    switch1=switch(sid=station.objects.get(sid=departmentID),cid=cabinet_station.objects.get(cid=switchgearID),S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                    switch1.save()
                                print "exception exist-voltage_low exception"

                            elif(float(Ia)>max_I or float(Ib)>max_I or float(Ic)>max_I):
                                exception2=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电流过大" )
                                exception2.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                sw_set=switch.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if sw_set.exists():
                                    sw_set.update(S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                else:
                                    switch1=switch(sid=station.objects.get(sid=departmentID),cid=cabinet_station.objects.get(cid=switchgearID),S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                    switch1.save()
                                print "exception exist-current exception"
                            elif(float(Ia)<min_I or float(Ib)<min_I or float(Ic)<min_I):
                                exception4=exception(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID),Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime,other_msg="电流过小" )
                                exception4.save()
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                sw_set=switch.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if sw_set.exists():
                                    sw_set.update(S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                else:
                                    switch1=switch(sid=station.objects.get(sid=departmentID),cid=cabinet_station.objects.get(cid=switchgearID),S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                    switch1.save()
                                print "exception exist-current_low exception"
                            else:
                                cv_set=cvoltage.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if cv_set.exists():
                                    cv_set.update( Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime)
                                else:
                                    cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                                    cvoltage1.save()
                                sw_set=switch.objects.filter(sid=d1,cid=cabinet_station.objects.get(cid=switchgearID))
                                if sw_set.exists():
                                    sw_set.update(S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                else:
                                    switch1=switch(sid=station.objects.get(sid=departmentID),cid=cabinet_station.objects.get(cid=switchgearID),S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                                    switch1.save()
                                print "update UI and Switch information without error!"

                    except cabinet_station.DoesNotExist:
                        cabinet_station1=cabinet_station(cid=switchgearID,sid=station.objects.get(sid=departmentID))
                        cabinet_station1.save()

                        cvoltage1 = cvoltage(sid=station.objects.get(sid=departmentID),cid = cabinet_station.objects.get(cid=switchgearID), Ua = Ua, Ub = Ub, Uc = Uc,Ia = Ia,Ib = Ib,Ic = Ic,PFa = PFa,PFb = PFb,PFc = PFc,update_time=dateTime )
                        cvoltage1.save()

                        switch1=switch(sid=station.objects.get(sid=departmentID),cid=cabinet_station.objects.get(cid=switchgearID),S1=BreakerPos,S2=HandPos,S3=TestPos,S4=GroundKnife,S5=Energy,S6=Switch7,S7=EA,S8=EB,S9=EC,S10=Lock,S11=Sensor1,S12=Sensor2,S13=Load1,S14=Load2,S15=Loop1,S16=Loop2,update_time=dateTime)
                        switch1.save()
                        print "insert success!"
            if(switchgearID==''):
                print "switchgearID is null"


    except Exception,ex:
        print Exception,":",ex
        print "error"
        response.write("error")
        return response
    response.write("sucess!")

    return response


def warning(request):
    return render_to_response('CVoltage/warning.html',RequestContext(request))


def update_switch(request):
    response = HttpResponse()
    try:
        if request.method == 'POST':
            req = simplejson.loads(request.body)
            snum = req['snum']
            breaker_location = req['breaker_location']
            manual_location = req['manual_location']
            energy_status = req['energy_status']

            switch1 = switch(snum = snum, breaker_location = breaker_location, manual_location = manual_location, energy_status = energy_status )
            switch1.save()
    except Exception,ex:
        print Exception,":",ex
        print "error"
        response.write("error")
        return response
    response.write("success")
    return response



def control_switch(request):
    DT=datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    if request.method == 'POST':
        Type="Ctrl"
        cmdID="456"
        suo_id=request.POST['suoid']
        gui_id=request.POST['guiid']
        kaiguan_id=request.POST['kaiguan']
        Channel=1
        suoid = suo_id.encode("utf-8")
        guiid =gui_id.encode("utf-8")
#        switchid = kaiguan_id.encode("utf-8")
        switchId=int(kaiguan_id)
        info={"TP":Type,"CID":cmdID,"TM":DT,"DID":suoid,"SID": guiid,"CH":Channel,"MD":switchId,}
    HOST='112.124.123.169'
    PORT=60000
    BUFSIZ=1024
    ADDR=(HOST,PORT)
    client=socket(AF_INET,SOCK_DGRAM)
    client.sendto(json.dumps(info),ADDR)
    data,ADDR=client.recvfrom(BUFSIZ)
    client.close()
    return HttpResponseRedirect("/CVoltage/index/")

def control_query(request):
    DT=datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    if request.method == 'POST':
        Type="query"
        cmdID="456"
        suo_id=request.POST['suo_id']
        gui_id=request.POST['gui_id']
        suoid = suo_id.encode("utf-8")
        guiid =gui_id.encode("utf-8")
        up_time=request.POST['up_time']
        time_interval=request.POST['time_interval']
        upTime=int(up_time)*60
        timeInterval=int(time_interval)
        info={"TP":Type,"CID":cmdID,"TM":DT,"DID":suoid,"SID": guiid,"TTL":upTime,"INT":timeInterval,}
    HOST='112.124.123.169'
    PORT=60000
    BUFSIZ=1024
    ADDR=(HOST,PORT)
    client=socket(AF_INET,SOCK_DGRAM)
    client.sendto(json.dumps(info),ADDR)
    data,ADDR=client.recvfrom(BUFSIZ)
    client.close()
    return HttpResponseRedirect("/CVoltage/index/")

def sendMsg(request):
    if request.method=='POST':
        suoid=request.POST['suo-id']
        guiid=request.POST['gui-id']
        phone_num=request.POST['phoneNum']
        suoid = suoid.encode("utf-8")
        guiid =guiid.encode("utf-8")
    url = 'http://www.106008.com/WebAPI/SmsAPI.asmx/SendSmsExt'
    values = {
        'user':'chenjun',
        'pwd':'HFUT123hfut',
        'mobiles':phone_num,
        'contents':"【东方旭电气】您好，经智能开关柜系统检测，发现配电所（"+suoid+"），"+"开关柜（"+guiid+"）运行出现故障，请您及时安排人员到现场进行检修",
        'chid':'0',
        'sendtime':'',
        }
    data = urllib.urlencode(values)
    req = urllib2.Request(url, data)
    response = urllib2.urlopen(req)
    the_page = response.read()  #the_page是字符串
    dom1 = minidom.parseString(the_page)
    result = dom1.getElementsByTagName("Result")
    result1 = result[0].childNodes[0].nodeValue

    return render_to_response('CVoltage/sendMsg.html',locals(),RequestContext(request))
