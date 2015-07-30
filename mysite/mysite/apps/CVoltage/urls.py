__author__ = 'joe'
#coding=utf-8
from django.conf.urls import patterns, url

urlpatterns=patterns('mysite.apps.CVoltage.views',
    url(r'^index/$','index',name='index'),
    url(r'^info1/$','info1',name='info1'),
    url(r'^info1_mp/$','info1_mp',name='info1_mp'),
    url(r'^infoCV/$','infoCV',name='infoCV'),
    url(r'^updateCV/$','update_cvoltage',name='update_cvoltage'),
    url(r'^updateSW/$','update_switch',name='update_switch'),
    url(r'^control_switch/$','control_switch',name='control_switch'),
    url(r'^control_query/$','control_query',name='control_query'),
    url(r'^warning/$','warning',name='warning'),
    url(r'^sendMsg/$','sendMsg',name='sendMsg'),

)
