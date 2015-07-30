__author__ = 'joe'
from django.contrib import admin
from mysite.apps.CVoltage.models import groupInfo,UserInfo

class groupInfoAdmin(admin.ModelAdmin):
    list_display =('gname','other_msg',)
    search_fields =('gname',)
    list_filter = ('gname',)
    ordering = ('gname',)

class UserInfoAdmin(admin.ModelAdmin):
    list_display =('user','gname',)
    search_fields =('user','gname',)
    list_filter = ('gname','user',)
    ordering = ('gname',)

admin.site.register(groupInfo,groupInfoAdmin)
admin.site.register(UserInfo,UserInfoAdmin)