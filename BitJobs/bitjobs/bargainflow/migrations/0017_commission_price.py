# -*- coding: utf-8 -*-
# Generated by Django 1.10.6 on 2017-05-08 18:06
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):
    dependencies = [
        ('bargainflow', '0016_remove_commission_price'),
    ]

    operations = [
        migrations.AddField(
            model_name='commission',
            name='price',
            field=models.DecimalField(decimal_places=6, default=0, max_digits=10),
        ),
    ]
