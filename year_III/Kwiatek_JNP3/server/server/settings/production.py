from .base import *

DEBUG = True

DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.postgresql_psycopg2',
        'NAME': 'kwiatek_iot',
        'USER': 'kwiatek_iot',
        'PASSWORD': 'kwiatek_iot',
        'HOST': 'stockalert.cuns27cc0cb3.eu-central-1.rds.amazonaws.com',
        'PORT': '5432',
    }
}

ALLOWED_HOSTS = ['*']
