# -*- coding: utf-8 -*-
# Generated by Django 1.10.6 on 2017-04-08 18:59
from __future__ import unicode_literals

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('bargainflow', '0003_auto_20170326_1632'),
    ]

    operations = [
        migrations.AddField(
            model_name='commission',
            name='title',
            field=models.CharField(default='xd', max_length=40, verbose_name='Title'),
            preserve_default=False,
        ),
        migrations.AlterField(
            model_name='commission',
            name='date_added',
            field=models.DateField(default=datetime.datetime(2017, 4, 8, 20, 59, 20, 404183)),
        ),
        migrations.AlterField(
            model_name='commission',
            name='description',
            field=models.TextField(verbose_name='Offer description'),
        ),
    ]
