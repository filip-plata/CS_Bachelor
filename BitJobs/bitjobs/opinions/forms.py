from opinions.models import Opinion
from django import forms
from django.forms.widgets import TextInput
from django.utils.translation import ugettext_lazy as _


class OpinionAddForm(forms.ModelForm):
    class Meta:
        model = Opinion
        fields = ['description', 'rating']

        labels = {
            'description': _('Opinion'),
            'rating': _('Rating'),

        }

        widgets = {
        }

        help_texts = {

        }