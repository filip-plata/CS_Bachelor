from django.conf.urls import url, include
from rest_framework import routers
from kwiatek import views

router = routers.DefaultRouter(trailing_slash=False)
router.register(r'ls', views.LightMeasurementViewSet,
                'Light')
router.register(r'ms', views.MoistureMeasurementViewSet,
                'Moisture')
router.register(r'ts', views.TemperatureMeasurementViewSet,
                'Temperature')

router.register(r'gt', views.GatewayViewSet, 'gateways')

router.register(r'alarms', views.AlarmViewSet)
router.register(r'flowers', views.FlowerViewSet)

# Wire up our API using automatic URL routing.
# Additionally, we include login URLs for the browsable API.
urlpatterns = [
    url(r'^api-auth/', include('rest_framework.urls',
                               namespace='rest_framework')),
    url(r'^', include(router.urls)),
]
