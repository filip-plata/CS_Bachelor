from django.views.generic import FormView
from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.views.decorators.http import require_POST
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import redirect
from django.http import HttpResponseRedirect, HttpResponse
from registration.forms import UserForm, RegistrationForm


@csrf_exempt
@require_POST
def register_api(request):
    login = request.POST.get('login', None)
    password = request.POST.get('password', None)

    print(login)
    print(password)

    if None in (login, password):
        return HttpResponse(status=403)

    try:
        User.objects.create_user(username=login,
                                 password=password)
    except Exception as e:
        print(e)
        return HttpResponse(status=403)

    return HttpResponse()


@csrf_exempt
@require_POST
def login_api(request):
    login = request.POST.get('login', None)
    password = request.POST.get('password', None)

    status = 200

    if authenticate(username=login, password=password) is None:
        status = 403

    return HttpResponse(status=status)
