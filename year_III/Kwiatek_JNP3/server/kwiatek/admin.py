from django.contrib import admin
from kwiatek.models import LightMeasurement, MoistureMeasurement, \
    TemperatureMeasurement, Flower, FlowerType, Node, Gateway, \
    NodesConfig


class TemperatureMeasurementAdmin(admin.ModelAdmin):
    pass


class LightMeasurementAdmin(admin.ModelAdmin):
    pass


class MoistureMeasurementAdmin(admin.ModelAdmin):
    pass


class FlowerAdmin(admin.ModelAdmin):
    pass


class FlowerTypeAdmin(admin.ModelAdmin):
    pass


class NodeAdmin(admin.ModelAdmin):
    pass


class NodesConfigAdmin(admin.ModelAdmin):
    pass


class GatewayAdmin(admin.ModelAdmin):
    pass


admin.site.register(TemperatureMeasurement,
                    TemperatureMeasurementAdmin)
admin.site.register(LightMeasurement,
                    LightMeasurementAdmin)
admin.site.register(MoistureMeasurement,
                    MoistureMeasurementAdmin)
admin.site.register(Flower, FlowerAdmin)
admin.site.register(FlowerType, FlowerTypeAdmin)
admin.site.register(Node, NodeAdmin)
admin.site.register(NodesConfig, NodesConfigAdmin)
admin.site.register(Gateway, GatewayAdmin)
