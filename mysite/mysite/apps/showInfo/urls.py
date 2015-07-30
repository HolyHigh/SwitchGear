__author__ = 'joe'
#coding=utf-8
from django.conf.urls import patterns, url

urlpatterns=patterns('mysite.apps.showInfo.views',
    url(r'^index/$','index',name='index'),

    url(r'^showStation/$','showStation',name='showStation'),
    url(r'^showSwitchgear/$','showSwitchgear',name='showSwitchgear'),
    url(r'^showGear/$','showGear',name='showGear'),
    url(r'^showSwitch/$','showSwitch',name='showSwitch'),
    url(r'^Scontral/$','switch_contral',name='switch_contral'),

)
