__author__ = 'joe'
#coding=utf-8
from django.conf.urls import patterns, url

urlpatterns=patterns('mysite.apps.Switch.views',
    url(r'^index/$','index',name='index'),
    url(r'^info2/$','info2',name='info2'),
    url(r'^info2_mp/$','info2_mp',name='info2_mp'),
    url(r'^infoSwitch/$','infoSwitch',name='infoSwitch'),
    url(r'^Scontral/$','switch_contral',name='switch_contral'),

)
