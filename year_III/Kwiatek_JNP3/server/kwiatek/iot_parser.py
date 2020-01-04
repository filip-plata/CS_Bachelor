import struct

from rest_framework.parsers import BaseParser

from kwiatek.iot import get_gateway_rx
from kwiatek.models import Node, NodesConfig, Gateway


class SensorMeasurementParser(BaseParser):
    media_type = "text/plain"

    measurement_mapping = {
        'm': 'moisture',  # moisture
        'l': 'intensity',  # light
        't': 'temperature',  # temperature
    }

    def parse(self, stream, media_type=None, parser_context=None):
        measurement = struct.unpack('f', stream.read(4))[0]
        print(measurement)
        m_type = stream.read(1).decode('ascii')
        h = stream.read(8)
        val = struct.unpack("l", h)[0]
        rx_address = "{:08x}".format(val)
        print(rx_address)
        gateway_address = get_gateway_rx(rx_address)
        print(gateway_address)

        try:
            Node.objects.get(rx_number=rx_address)
        except Node.DoesNotExist:
            try:
                Node.objects.create(
                    rx_number=rx_address, config=NodesConfig.objects.create(),
                    gateway=Gateway.objects.get(rx_number=gateway_address)
                )
            except Gateway.DoesNotExist:
                # Should not happen
                pass

        return {"node": rx_address, self.measurement_mapping[m_type]: measurement}
