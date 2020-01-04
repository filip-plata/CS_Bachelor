from django.db.models import Avg
from django.utils import timezone
from django.db import models


class NodesConfig(models.Model):
    data_interval_milliseconds = models.PositiveIntegerField(default=60000)


class Gateway(models.Model):
    ip = models.GenericIPAddressField(null=False)
    rx_number = models.CharField(max_length=16, primary_key=True)


class Node(models.Model):
    rx_number = models.CharField(max_length=16, primary_key=True)
    config = models.ForeignKey(NodesConfig, null=False)
    gateway = models.ForeignKey(Gateway, null=False)

    def __str__(self):
        return "code: {}".format(self.rx_number)


class Measurement(models.Model):
    class Meta:
        abstract = True

    timestamp = models.DateTimeField(auto_created=True, blank=True)
    node = models.ForeignKey(Node)

    def save(self, *args, **kwargs):
        """ On save, update timestamps """
        if not self.id:
            self.timestamp = timezone.now()
        return super(Measurement, self).save(*args, **kwargs)


class TemperatureMeasurement(Measurement):
    temperature = models.FloatField()


class LightMeasurement(Measurement):
    intensity = models.FloatField()


class MoistureMeasurement(Measurement):
    moisture = models.FloatField()


class FlowerType(models.Model):
    type_name = models.CharField(max_length=40)

    min_temp = models.FloatField()
    max_temp = models.FloatField()
    min_moisture = models.FloatField()
    max_moisture = models.FloatField()
    min_light_int = models.FloatField()
    max_light_int = models.FloatField()

    def __str__(self):
        return self.type_name


class Flower(models.Model):
    HEALTHY_THRESHOLD = 0.3

    name = models.CharField(max_length=40, unique=True)
    type = models.ForeignKey(FlowerType, null=False)
    temperature_node = models.ForeignKey(
        Node, blank=True, null=True,
        related_name='temp')
    light_intensity_node = models.ForeignKey(
        Node, blank=True, null=True,
        related_name='light')
    moisture_node = models.ForeignKey(
        Node, blank=True, null=True,
        related_name='moisture')
    gateway = models.ForeignKey(Gateway, blank=True, null=True)
    last_automatic_alarm = models.DateTimeField(null=True, blank=True)

    def flower_health(self):
        """
        Returns floating point number from range
        [0, 1] representing flower health.
        :return:
        """
        return MoistureMeasurement.objects.order_by('-timestamp')[:10].aggregate(Avg('moisture'))['moisture__avg']

    def is_healthy(self):
        return self.HEALTHY_THRESHOLD <= self.flower_health()

    def __str__(self):
        return self.name


class Alarms(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True, null=False)
    is_automatic = models.BooleanField(null=False, default=False)
    reason = models.TextField()
