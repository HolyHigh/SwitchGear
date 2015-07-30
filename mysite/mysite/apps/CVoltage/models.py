# Create your models here.
#coding=utf-8

from django.contrib import admin
from django.db import models
from django.contrib.auth.models import User

class groupInfo(models.Model):      #一个组代表一个地方的项目
    gid=models.CharField(max_length=100,verbose_name='组序号')
    gname=models.CharField(verbose_name='组名称',max_length=100)
    other_msg=models.CharField(max_length=800,blank='True',null='True')

    def __unicode__(self):
        return self.gid

    class Meta:
        verbose_name = "划分项目组"
        verbose_name_plural = "划分项目组"


class UserInfo(models.Model):         #在系统用户的字段增加一个所属组的字段
    user=models.ForeignKey(User,unique=True,verbose_name='用户名')
    gname=models.ForeignKey(groupInfo)

    def __unicode__(self):
        return self.gname.gname

    class Meta:
        verbose_name = "设置用户信息"
        verbose_name_plural = "设置用户信息"

class station(models.Model):                #配电所
    sid=models.CharField(max_length=100,verbose_name='所序号')
    sname=models.CharField(verbose_name='所名称',max_length=100)
    gname=models.ForeignKey(groupInfo)
    other_msg=models.CharField(max_length=800,blank='True',null='True')


    def __unicode__(self):
        return self.sid


    class Meta:
        verbose_name = "所的信息"
        verbose_name_plural = "所信息表"

class cabinet_station(models.Model):        #所和下面的开关柜对应的信息
    cid=models.CharField(max_length=100,verbose_name='柜序号')
    sid=models.ForeignKey(station)
    type_voltage=models.CharField(max_length=100,verbose_name='电压柜类型')

    def __unicode__(self):
        return self.cid

    class Meta:
        verbose_name = "柜与所的对应关系"
        verbose_name_plural = "柜与所的对应关系表"

class cvoltage(models.Model):               #一个开关柜对应的电压电流信息
    sid=models.ForeignKey(station)
    cid=models.ForeignKey(cabinet_station)
    Ua=models.FloatField(max_length=10,verbose_name='A相电压',blank='True',null='True')
    Ub=models.FloatField(max_length=10,verbose_name='B相电压',blank='True',null='True')
    Uc=models.FloatField(max_length=10,verbose_name='C相电压',blank='True',null='True')
    Ia=models.FloatField(max_length=10,verbose_name='A相电流',blank='True',null='True')
    Ib=models.FloatField(max_length=10,verbose_name='B相电流',blank='True',null='True')
    Ic=models.FloatField(max_length=10,verbose_name='C相电流',blank='True',null='True')
    PFa=models.FloatField(max_length=10,verbose_name='A相功率因素',blank='True',null='True')
    PFb=models.FloatField(max_length=10,verbose_name='B相功率因素',blank='True',null='True')
    PFc=models.FloatField(max_length=10,verbose_name='C相功率因素',blank='True',null='True')
    update_time=models.DateTimeField(verbose_name='更新时间',blank='True',null='True')
    other_msg=models.CharField(max_length=800,blank='True',null='True')



#    def __unicode__(self):
#        return self.sid,self.cid

    class Meta:
        verbose_name = "电流电压信息"
        verbose_name_plural = "电流电压信息表"

class switch(models.Model):             #一个开关柜对应的各路开关信息
    sid=models.ForeignKey(station)
    cid=models.ForeignKey(cabinet_station)
    S1=models.NullBooleanField(verbose_name='S1')
    S2=models.NullBooleanField(verbose_name='S2')
    S3=models.NullBooleanField(verbose_name='S3')
    S4=models.NullBooleanField(verbose_name='S4')
    S5=models.NullBooleanField(verbose_name='S5')
    S6=models.NullBooleanField(verbose_name='S6')
    S7=models.NullBooleanField(verbose_name='S7')
    S8=models.NullBooleanField(verbose_name='S8')
    S9=models.NullBooleanField(verbose_name='S9')
    S10=models.NullBooleanField(verbose_name='S10')
    S11=models.NullBooleanField(verbose_name='S11')
    S12=models.NullBooleanField(verbose_name='S12')
    S13=models.NullBooleanField(verbose_name='S13')
    S14=models.NullBooleanField(verbose_name='S14')
    S15=models.NullBooleanField(verbose_name='S15')
    S16=models.NullBooleanField(verbose_name='S16')
    update_time=models.DateTimeField(verbose_name='更新时间',blank='True',null='True')
    other_msg=models.CharField(max_length=800,blank='True',null='True')



#    def __unicode__(self):
#        return self.sid,self.cid

    class Meta:
        verbose_name = "开关信息"
        verbose_name_plural = "开关信息表"





class exception(models.Model):              #异常信息
    sid=models.ForeignKey(station)
    cid=models.ForeignKey(cabinet_station)
    Ua=models.FloatField(max_length=10,verbose_name='A相电压',blank='True',null='True')
    Ub=models.FloatField(max_length=10,verbose_name='B相电压',blank='True',null='True')
    Uc=models.FloatField(max_length=10,verbose_name='C相电压',blank='True',null='True')
    Ia=models.FloatField(max_length=10,verbose_name='A相电流',blank='True',null='True')
    Ib=models.FloatField(max_length=10,verbose_name='B相电流',blank='True',null='True')
    Ic=models.FloatField(max_length=10,verbose_name='C相电流',blank='True',null='True')
    PFa=models.FloatField(max_length=10,verbose_name='A相功率因素',blank='True',null='True')
    PFb=models.FloatField(max_length=10,verbose_name='B相功率因素',blank='True',null='True')
    PFc=models.FloatField(max_length=10,verbose_name='C相功率因素',blank='True',null='True')
    update_time=models.DateTimeField(verbose_name='更新时间',blank='True',null='True')
    other_msg=models.CharField(max_length=800,blank='True',null='True')

#    def __unicode__(self):
#        return self.cid,self.sid

    class Meta:
        verbose_name = "异常信息表"
        verbose_name_plural = "异常信息表"

class setRange(models.Model):           #设置电压电流范围
    sid=models.ForeignKey(station)
    cid=models.ForeignKey(cabinet_station)
    max_U=models.FloatField(max_length=10,verbose_name='最大电压')
    min_U=models.FloatField(max_length=10,verbose_name='最小电流')
    max_I=models.FloatField(max_length=10,verbose_name='最大电流')
    min_I=models.FloatField(max_length=10,verbose_name='最小电流')

    class Meta:
        verbose_name = "设置电压电流范围"
        verbose_name_plural = "设置电压电流范围"