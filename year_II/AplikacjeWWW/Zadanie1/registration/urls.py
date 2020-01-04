from django.conf.urls import url
from . import views


urlpatterns = [
    url(r'^register-api/', views.register_api, name='register-api'),
    url(r'^login-api/', views.login_api, name='login-api'),
]
