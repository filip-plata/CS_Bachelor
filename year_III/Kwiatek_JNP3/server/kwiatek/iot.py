"""Interface to IOT network monitoring flowers"""
import logging

from aiocoap import Context
from aiocoap import Message
import aiocoap as aio

from kwiatek.models import Flower, Gateway


logger = logging.getLogger(__name__)


def _send_alarm(uri, retry=True):
    uri = "coap://{}/alarm".format(uri)
    protocol = Context.create_client_context()
    request = Message(code=aio.GET, uri=uri)

    try:
        protocol.request(request)
    except Exception as _:
        logger.error('Could not send alarm to gateway: %s', uri)
    else:
        if retry:
            _send_alarm(uri, retry=False)


def raise_alarm():
    gateways = Gateway.objects.all()

    for gate in gateways:
        _send_alarm(gate.ip)


def update_nodes_config(node):
    context = Context.create_client_context()
    payload = node.config.data_interval_milliseconds.to_bytes(
        2, byteorder='big')

    request = Message(code=aio.PUT, payload=payload)
    request.opt.uri_host = node.gateway.uri
    request.opt.uri_path = ("update_nodes", str(node.rx_number))

    context.request(request)


def get_gateway_rx(node_rx):
    assert len(node_rx) == 8
    return node_rx[:6] + '00'
