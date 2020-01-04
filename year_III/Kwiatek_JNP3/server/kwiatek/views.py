from rest_framework import viewsets
from rest_framework.parsers import JSONParser
from rest_framework.response import Response
from rest_framework.status import HTTP_200_OK

from kwiatek.models import *
from kwiatek.serializers import *

from kwiatek.iot_parser import SensorMeasurementParser
from kwiatek.utils import get_client_ip


class TemperatureMeasurementViewSet(viewsets.ModelViewSet):
    parser_classes = (SensorMeasurementParser,)
    serializer_class = TemperatureMeasurementSerializer

    def get_queryset(self):
        queryset = TemperatureMeasurement.objects.all()
        flower = self.request.query_params.get('flower')

        if flower:
            queryset = queryset.filter(node=Flower.objects.get(
                name=flower).temperature_node).order_by('timestamp')

        return queryset


class LightMeasurementViewSet(viewsets.ModelViewSet):
    parser_classes = (SensorMeasurementParser,)
    serializer_class = LightMeasurementSerializer

    def get_queryset(self):
        queryset = LightMeasurement.objects.all()
        flower = self.request.query_params.get('flower')

        if flower:
            queryset = queryset.filter(node=Flower.objects.get(
                name=flower).light_intensity_node).order_by('timestamp')

        return queryset


class MoistureMeasurementViewSet(viewsets.ModelViewSet):
    parser_classes = (SensorMeasurementParser,)
    serializer_class = MoistureMeasurementSerializer

    def get_queryset(self):
        queryset = MoistureMeasurement.objects.all()
        flower = self.request.query_params.get('flower')

        if flower:
            queryset = queryset.filter(node=Flower.objects.get(
                name=flower).moisture_node).order_by('timestamp')

        return queryset


class GatewayViewSet(viewsets.ModelViewSet):
    queryset = Gateway.objects.all()
    serializer_class = GatewaySerializer

    def put(self, request):
        try:
            gateway = Gateway.objects.get(rx_number=request.data.get('rx_number'))
            gateway.ip = get_client_ip(request)
            gateway.save()
        except Gateway.DoesNotExist:
            Gateway(**request.data).save()
        return Response(status=HTTP_200_OK)


class AlarmViewSet(viewsets.ModelViewSet):
    queryset = Alarms.objects.all().order_by('timestamp')
    serializer_class = AlarmSerializer


class FlowerViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Flower.objects.all()
    serializer_class = FlowerSerializer
