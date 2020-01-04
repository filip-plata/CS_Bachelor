import struct

from rest_framework.reverse import reverse
from rest_framework import status
from rest_framework.test import APITestCase
from kwiatek.models import Gateway, Node


class MeasurementTests(APITestCase):

    urls = 'kwiatek.urls'

    def setUp(self):
        Gateway.objects.create(rx_number="AAAA0000", ip="127.0.0.1")

    def test_gateway_handshake(self):
        rx_number = "ABCD0000"
        ip_addr = "127.0.0.3"

        url = reverse('gateways-list')
        data = {"rx_number": rx_number, "ip": ip_addr}
        response = self.client.post(url, data, format='json')

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)

        try:
            Gateway.objects.get(rx_number=rx_number)
        except Gateway.DoesNotExist:
            self.fail("Gateway was not created")

    def test_sending_initial_gateway(self):
        url = reverse('Light-list')
        rx_number = "AAAA0001"
        m_type = "l"

        payload = struct.pack("f", 0.89) + (m_type + rx_number).encode()

        response = self.client.post(url, payload, content_type='text/plain')

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)

        try:
            Node.objects.get(rx_number=rx_number)
        except Node.DoesNotExist:
            self.fail("Node object was not created")

    def test_changing_gateway_ip(self):
        rx_number = "AAAA0000"
        ip_addr = "127.0.0.1"

        url = reverse('gateways-list')
        data = {"rx_number": rx_number, "ip": ip_addr}
        response = self.client.put(url, data, format='json')

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        try:
            gt = Gateway.objects.get(rx_number=rx_number)
            self.assertEqual(gt.ip, ip_addr)
        except Gateway.DoesNotExist:
            self.fail("Gateway was not created")
