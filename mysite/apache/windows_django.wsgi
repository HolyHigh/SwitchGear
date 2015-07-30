
import os
import sys

#get project directory
root = os.path.join(os.path.dirname(__file__), '..')
sys.path.insert(0, root)

os.environ['DJANGO_SETTINGS_MODULE'] = 'mysite.settings'

import django.core.handlers.wsgi
application = django.core.handlers.wsgi.WSGIHandler()
