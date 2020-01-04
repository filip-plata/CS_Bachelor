from django import forms
from django.utils.translation import ugettext_lazy as _


class WithdrawForm(forms.Form):
    address = forms.CharField(
                    help_text=_("Address"), label=_("Address"))
    amount = forms.DecimalField(
                    help_text=_("Amount"), label=_("Amount"))
