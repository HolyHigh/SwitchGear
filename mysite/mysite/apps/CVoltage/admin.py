__author__ = 'joe'
from django.contrib import admin
from mysite.apps.CVoltage.models import cvoltage,switch,station,cabinet_station,exception,setRange

class cvlotageAdmin(admin.ModelAdmin):
    list_display =('update_time','sid','cid','Ua','Ub','Uc','Ia','Ib','Ic','PFa','PFb','PFc')
    search_fields =('sid','cid',)
    list_filter = ('sid','cid',)
    ordering = ('update_time',)

class switchAdmin(admin.ModelAdmin):
    list_display =('update_time','S1','S2','S3','S4','S5','S6','S7','S8','S9','S10','S11','S12','S13','S14','S15','S16','sid','cid',)
    search_fields =('sid','cid',)
    list_filter = ('sid','cid',)
    ordering = ('update_time',)

class stationAdmin(admin.ModelAdmin):
    list_display =('sid','sname')
    search_fields =('sid','sname',)
    list_filter = ('sid','sname',)
    ordering = ('sid',)
class cabinet_stationAdmin(admin.ModelAdmin):
    list_display = ('cid','sid','type_voltage',)
    search_fields =('sid','cid',)
    list_filter = ('sid','cid',)
    ordering = ('sid','cid',)
class exceptionAdmin(admin.ModelAdmin):
    list_display =('update_time','Ua','Ub','Uc','Ia','Ib','Ic','PFa','PFb','PFc','other_msg','sid','cid',)
    search_fields =('sid','cid',)
    list_filter = ('sid','cid',)
    ordering = ('update_time',)

class setRangeAdmin(admin.ModelAdmin):
    list_display =('max_U','min_U','max_I','min_I','sid','cid',)
    search_fields =('sid','cid',)
    list_filter = ('sid','cid',)
    ordering = ('sid','cid',)
admin.site.register(cvoltage,cvlotageAdmin)
admin.site.register(switch,switchAdmin)
admin.site.register(station,stationAdmin)
admin.site.register(cabinet_station,cabinet_stationAdmin)
admin.site.register(exception,exceptionAdmin)
admin.site.register(setRange,setRangeAdmin)