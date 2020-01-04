"""
Module for storing data about user's money, payments and user's transaction history"""

from __future__ import unicode_literals

from cc.models import Wallet, Currency
from cc.tasks import refill_addresses_queue
from django.contrib.auth.models import User
from django.db import models
from django.db.models.signals import post_save

class Customer(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE,
                                related_name='user_ext')
    wallet = models.ForeignKey(Wallet, on_delete=models.CASCADE,
                               related_name='user_wallet')

    def create_customer_data(sender, instance, created, **kwargs):
        if created:
            refill_addresses_queue()
            wallet = Wallet(currency=Currency.objects.get(ticker='LTC'))
            wallet.get_address()
            wallet.save()
            Customer.objects.create(user=instance, wallet=wallet)

    post_save.connect(create_customer_data, sender=User)

