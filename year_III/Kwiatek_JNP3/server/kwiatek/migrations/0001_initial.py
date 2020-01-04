# -*- coding: utf-8 -*-
# Generated by Django 1.11 on 2017-12-07 20:25
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Alarms',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('timestamp', models.DateTimeField(auto_now_add=True)),
                ('is_automatic', models.BooleanField(default=False)),
                ('reason', models.TextField()),
            ],
        ),
        migrations.CreateModel(
            name='Flower',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=40, unique=True)),
                ('last_automatic_alarm', models.DateTimeField(blank=True, null=True)),
            ],
        ),
        migrations.CreateModel(
            name='FlowerType',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('type_name', models.CharField(max_length=40)),
                ('min_temp', models.FloatField()),
                ('max_temp', models.FloatField()),
                ('min_moisture', models.FloatField()),
                ('max_moisture', models.FloatField()),
                ('min_light_int', models.FloatField()),
                ('max_light_int', models.FloatField()),
            ],
        ),
        migrations.CreateModel(
            name='Gateway',
            fields=[
                ('ip', models.GenericIPAddressField()),
                ('rx_number', models.CharField(max_length=16, primary_key=True, serialize=False)),
            ],
        ),
        migrations.CreateModel(
            name='LightMeasurement',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('timestamp', models.DateTimeField(auto_created=True)),
                ('intensity', models.FloatField()),
            ],
            options={
                'abstract': False,
            },
        ),
        migrations.CreateModel(
            name='MoistureMeasurement',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('timestamp', models.DateTimeField(auto_created=True)),
                ('moisture', models.FloatField()),
            ],
            options={
                'abstract': False,
            },
        ),
        migrations.CreateModel(
            name='Node',
            fields=[
                ('rx_number', models.CharField(max_length=16, primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=32)),
            ],
        ),
        migrations.CreateModel(
            name='NodesConfig',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('data_interval_milliseconds', models.PositiveIntegerField(default=60000)),
            ],
        ),
        migrations.CreateModel(
            name='TemperatureMeasurement',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('timestamp', models.DateTimeField(auto_created=True)),
                ('temperature', models.FloatField()),
                ('node', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='kwiatek.Node')),
            ],
            options={
                'abstract': False,
            },
        ),
        migrations.AddField(
            model_name='node',
            name='config',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='kwiatek.NodesConfig'),
        ),
        migrations.AddField(
            model_name='node',
            name='gateway',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='kwiatek.Gateway'),
        ),
        migrations.AddField(
            model_name='moisturemeasurement',
            name='node',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='kwiatek.Node'),
        ),
        migrations.AddField(
            model_name='lightmeasurement',
            name='node',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='kwiatek.Node'),
        ),
        migrations.AddField(
            model_name='flower',
            name='gateway',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, to='kwiatek.Gateway'),
        ),
        migrations.AddField(
            model_name='flower',
            name='light_intensity_node',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='light', to='kwiatek.Node'),
        ),
        migrations.AddField(
            model_name='flower',
            name='moisture_node',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='moisture', to='kwiatek.Node'),
        ),
        migrations.AddField(
            model_name='flower',
            name='temperature_node',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='temp', to='kwiatek.Node'),
        ),
        migrations.AddField(
            model_name='flower',
            name='type',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='kwiatek.FlowerType'),
        ),
    ]
