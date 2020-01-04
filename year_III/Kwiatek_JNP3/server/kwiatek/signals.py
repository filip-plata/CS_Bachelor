from django.db.models.signals import post_save
from kwiatek.models import Measurement, Flower, Alarms
from kwiatek.iot import raise_alarm, update_nodes_config
from kwiatek.utils import inform_admin


def flower_health_alarm(sender, **kwargs):
    if any([not fl.is_healthy() for fl in Flower.objects.all()]):
        prev = Alarms.objects.all().order_by('-timestamp')[:2]
        if len(prev) == 1 or \
                (len(prev) == 2 and ((prev[0].timestamp - prev[1].timestamp).total_seconds() >= 60)):
            Alarms.objects.create(is_automatic=True)


def check_configuration_propagated(sender, **kwargs):
    tolerance = 0.05

    node = sender.node
    node_config = node.config
    last_measurements = [meas.timestamp for meas in
                         list(sender.__class__.objects.filter(node=node).
                              order_by('timestamp')[:2])]

    if len(last_measurements) == 2:
        rel_error = (last_measurements[1] - last_measurements[0]).total_seconds() * 1000 / \
                    node_config.data_interval_milliseconds

        if rel_error > tolerance:
            update_nodes_config(node)


def alarm_triggered(sender, **kwargs):
    raise_alarm()
    inform_admin()


post_save.connect(alarm_triggered, Alarms)

post_save.connect(flower_health_alarm, Measurement)
# connect all subclasses of base content item too
for subclass in Measurement.__subclasses__():
    post_save.connect(flower_health_alarm, subclass)

post_save.connect(check_configuration_propagated, Measurement)
# connect all subclasses of base content item too
for subclass in Measurement.__subclasses__():
    post_save.connect(check_configuration_propagated, subclass)
