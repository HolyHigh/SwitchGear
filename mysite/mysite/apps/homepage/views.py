# Create your views here.
#coding:utf-8
from django.http import HttpResponse,HttpResponseRedirect
from django.template import RequestContext,Context
from django.shortcuts import render_to_response
from django.utils import simplejson
from django.contrib import auth
from django.contrib.auth.models import User
from django.db.models import Avg
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from mysite.apps.CVoltage.models import exception,station,UserInfo,groupInfo

from mysite import settings
#from AIoTPlatform.apps.homepage import config_file
#import suds

def index(request):
    '''
    platform homepage
    '''
    #request.session.set_test_cookie()
    return render_to_response('homepage/index.html',RequestContext(request))
def index_mp(request):
    if request.user.is_authenticated():
        return HttpResponse(simplejson.dumps({'result':'success'}))
    else:
        return HttpResponse(simplejson.dumps({'result':'fail'}))

def boot(request):
    '''
    platform homepage
    '''
    #request.session.set_test_cookie()
    return render_to_response('homepage/boot.html',RequestContext(request))


def introduction(request):
    '''
    platform introduction
    '''
#    request.session.set_test_cookie()
    return  render_to_response('homepage/introduce.html',RequestContext(request))

def contact(request):
    '''
    contact information
    '''
    request.session.set_test_cookie()
    return  render_to_response('homepage/contact.html',RequestContext(request))

def showException(request):
    if request.user.is_superuser:
        exceptions = exception.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        exceptions=exception.objects.filter(sid__in=stations)
#    exceptions = exception.objects.all()
    exceptions=exceptions.order_by('-update_time')
    for item in exceptions:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    count=exceptions.count()
    paginator=Paginator(exceptions,20)
    try:
        page=int(request.GET.get("page",1))
        if page<1:
            page=1
    except ValueError:
        page=1
    try:
        exceptionInfo=paginator.page(page)
    except PageNotAnInteger:
        exceptionInfo=paginator.page(1)
    except EmptyPage:
        exceptionInfo=paginator.page(paginator.num_pages)
    return render_to_response('homepage/exception.html',locals(),RequestContext(request))

def showException_mp(request):
    if request.user.is_superuser:
        exceptions = exception.objects.all()
    else:
        user_login=UserInfo.objects.get(user=User.objects.get(username=request.user.username))
        stations=station.objects.filter(gname=user_login.gname)
        exceptions=exception.objects.filter(sid__in=stations)
#    exceptions = exception.objects.all()
    exceptions=exceptions.order_by('-update_time')
    for item in exceptions:
        item.update_time=item.update_time.strftime("%Y-%m-%d %H:%M:%S")
    count=exceptions.count()
    paginator=Paginator(exceptions,20)
    try:
        page=int(request.GET.get("page",1))
        if page<1:
            page=1
    except ValueError:
        page=1
    try:
        exceptionInfo=paginator.page(page)
    except PageNotAnInteger:
        exceptionInfo=paginator.page(1)
    except EmptyPage:
        exceptionInfo=paginator.page(paginator.num_pages)
    i=0
    CVinfo_json=[]
    info={}
    for item in exceptionInfo:
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
        CVinfo["reason"]=item.other_msg
        CVinfo["update_time"]=item.update_time
        CVinfo_json.append(CVinfo)
    info["CVinfo"]=CVinfo_json
    info["page"] = page
    info["count"]=count
    return HttpResponse(simplejson.dumps(info))

def register(request):
    '''
    deal with user register
    '''
    if request.method == "POST":
        request.session.set_test_cookie()
        user=User.objects.create_user(username=request.POST['username'],password=request.POST['password1'])
        try:
            import sys
            reload(sys)
            sys.setdefaultencoding('utf-8')
            user.save()
            response=HttpResponse()
            response.write("<script>alert('注册成功,可登录系统'),window.location.href='/homepage/index/'</script>")
            return response
        except:
            response=HttpResponse()
            response.write("<script>alert('注册失败,请重新注册'),window.location.href='/homepage/register/'</script>")
            return response

    else:
        return render_to_response('homepage/register.html',RequestContext(request))

def checkusername(request):
    '''
    check whether username is OK for register
    '''
    if request.method=='GET':
        username=request.GET['username']
        user=User.objects.filter(username__iexact  = username)
        if user:
            return HttpResponse(simplejson.dumps({'result':'用户名已存在！'}))
        else:
            return HttpResponse(simplejson.dumps({'result':'用户名可用'}))


#for user login
def login(request):
    if request.method=="POST":
        response=HttpResponse()
#        if not request.session.test_cookie_worked():
#            response.write("<script>alert('登陆失败!请确认您的浏览器已打开cookie功能后重新登陆'),window.location.href='/homepage/index/'</script>")
#            return response
#        request.session.delete_test_cookie()
        user=auth.authenticate(username=request.POST['username'],password=request.POST['password'])
        if user is not None and user.is_active:
#            request.session['username'] = user.username
            auth.login(request,user)


            return render_to_response('homepage/index.html',RequestContext(request)) #login success
        else:
            response.write("<script>alert('登录失败，请重新登录'),window.location.href='/homepage/boot/'</script>")
            return response #login error
    else:
        return render_to_response('homepage/index.html',RequestContext(request))   #login error

def login_mp(request):
    if request.method=="GET":
        response=HttpResponse()
        #        if not request.session.test_cookie_worked():
        #            response.write("<script>alert('登陆失败!请确认您的浏览器已打开cookie功能后重新登陆'),window.location.href='/homepage/index/'</script>")
        #            return response
        #        request.session.delete_test_cookie()
        user=auth.authenticate(username=request.GET['username'],password=request.GET['password'])
        if user is not None and user.is_active:
        #            request.session['username'] = user.username
            auth.login(request,user)
            return HttpResponse(simplejson.dumps({'result':'success'})) #login success
        else:
            return HttpResponse(simplejson.dumps({'result':'fail'}))


def logout(request):
    auth.logout(request)
    return HttpResponseRedirect('/homepage/index/')


