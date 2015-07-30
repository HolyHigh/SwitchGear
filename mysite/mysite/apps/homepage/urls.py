__author__ = 'joe'
from  django.conf.urls import patterns,url
import  views
urlpatterns=patterns('',

    url(r'^index/$',views.index),
    url(r'^index_mp/$',views.index_mp),
#    url(r'^userpage/$',views.showUserCenter),
    url(r'^boot/$',views.boot),
    url(r'^register/$',views.register),

#    url(r'^mregister/$',views.mregister),
    url(r'^checkusername/$',views.checkusername),
#
#    url(r'^pwd/$',views.showPwd),
#    url(r'^checkoldpwd/$',views.checkOldpassword),
#    url(r'^changepwd/$',views.alertPassword),
#    url(r'^mobilepwd/$',views.changePwdMobile),
#
    url(r'^login/$',views.login),
    url(r'^login_mp/$',views.login_mp),
#    url(r'^mlogin/$',views.mlogin),
    url(r'^logout/$',views.logout),
#
    url(r'^introduce/$',views.introduction),
    url(r'^contact/$',views.contact),
    url(r'^exception/$',views.showException),
    url(r'^showException_mp/$',views.showException_mp),

)