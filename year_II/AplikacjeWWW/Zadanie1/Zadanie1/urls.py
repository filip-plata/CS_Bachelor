"""Zadanie1 URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.10/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url, include
from django.contrib import admin
from django.conf import settings
from django.views.static import serve

urlpatterns = []

if settings.DEBUG:
    urlpatterns += [
        url(r'^media/(?P<path>.*)$', serve, {
            'document_root': settings.MEDIA_ROOT,
        }),
        url(r'^static/(?P<path>.*)$', serve, {
            'document_root': settings.STATIC_ROOT,
        }),
    ]

if settings.DEBUG:
    urlpatterns += [
        url(r'^$', serve, {
            'path': 'index.html',
            'document_root': settings.STATIC_ROOT,
        }),
        url(r'^user/rejestracja/', serve, {
            'path': 'register.html',
            'document_root': settings.STATIC_ROOT,
        }),
        url(r'^user/logowanie/', serve, {
            'path': 'login.html',
            'document_root': settings.STATIC_ROOT,
        }),
        url(r'^gminy/', serve, {
            'path': 'boroughs.html',
            'document_root': settings.STATIC_ROOT,
        }),
        url(r'^region/', serve, {
            'path': 'region.html',
            'document_root': settings.STATIC_ROOT,
        }),
    ]

urlpatterns += [
    url(r'^admin/', admin.site.urls),
    url(r'^user/', include('registration.urls')),
    url(r'', include('wybory2000.urls')),
]
