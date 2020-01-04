from django import forms
from django.contrib.auth.models import User
from django.contrib.auth import authenticate
from django.contrib.auth.forms import UserCreationForm as RegistrationForm


class UserForm(forms.Form):
    login = forms.CharField(max_length=30)
    hasło = forms.CharField(max_length=30, widget=forms.PasswordInput())

    def get_user(self):
        data = self.cleaned_data

        username = data['login']
        password = data['hasło']

        return authenticate(username=username, password=password)
