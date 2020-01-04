from rest_framework import serializers
from kwiatek.models import TemperatureMeasurement, Measurement, \
    LightMeasurement, MoistureMeasurement, Flower, Node, \
    Gateway, Alarms


class MeasurementSerializer(serializers.ModelSerializer):
    time = serializers.SerializerMethodField()

    def get_time(self, obj):
        return int(obj.timestamp.strftime("%s"))

    class Meta:
        model = Measurement
        fields = ['time', 'node']


class TemperatureMeasurementSerializer(MeasurementSerializer):
    class Meta:
        model = TemperatureMeasurement
        fields = MeasurementSerializer.Meta.fields + ['temperature']


class LightMeasurementSerializer(MeasurementSerializer):
    class Meta:
        model = LightMeasurement
        fields = MeasurementSerializer.Meta.fields + ['intensity']


class MoistureMeasurementSerializer(MeasurementSerializer):
    class Meta:
        model = MoistureMeasurement
        fields = MeasurementSerializer.Meta.fields + ['moisture']


class FlowerSerializer(serializers.ModelSerializer):
    flower_health = serializers.SerializerMethodField()

    def get_flower_health(self, obj):
        return obj.flower_health()

    class Meta:
        model = Flower
        fields = ['name', 'flower_health']


class NodeSerializer(serializers.ModelSerializer):
    class Meta:
        model = Node
        fields = '__all__'


class GatewaySerializer(serializers.ModelSerializer):
    class Meta:
        model = Gateway
        fields = ['rx_number']
        read_only_fields = ['ip']


class AlarmSerializer(serializers.ModelSerializer):
    time = serializers.SerializerMethodField(read_only=True)

    def get_time(self, obj):
        return obj.timestamp.strftime('%Y-%m-%d %H:%M')

    class Meta:
        model = Alarms
        read_only_fields = ['pk']
        fields = ['is_automatic', 'reason', 'time', 'pk']
