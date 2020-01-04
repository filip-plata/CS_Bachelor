import os
import json
import logging
from functools import partial
from pprint import pformat
from datetime import datetime

import pika
import requests
from jsonschema import validate, ValidationError

from requests.packages.urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter

from thresholdalert import db, app
from thresholdalert.models import BaseStockAlert, AlertNotification
from thresholdalert.jsonschema import tick_message_schema


logger = logging.getLogger('gunicorn.error')


class RabbitNotify:
    def __enter__(self):
        credentials = pika.PlainCredentials('rabbitmq', 'rabbitmq')
        parameters = pika.ConnectionParameters(
            os.environ.get('RABBIT_HOST', 'localhost'),
            5672, '/', credentials)
        connection = pika.BlockingConnection(parameters)
        self.notify_channel = connection.channel()
        self.notify_channel.queue_declare(queue='notification')
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._close()

    def notify(self, notification_json):
        logger.info("Sending notification to user '{}'".format(notification_json['user_id']))
        self.notify_channel.basic_publish(
            exchange='',
            routing_key='notification',
            body=json.dumps(notification_json))

    def _close(self):
        self.notify_channel.close()


def parse(msg):
    try:
        logger.info('Received payload: %s', msg)
        data = msg.decode('utf8')
        payload = json.loads(data)
        validate(payload, tick_message_schema)
    except UnicodeDecodeError:
        logger.error('Invalid message encoding - use utf8')
    except ValueError:
        logger.error('Invalid json format: %s', msg)
    except ValidationError:
        logger.error('Invalid json schema - use format:\n \n%s\n',
                     tick_message_schema)
    else:
        return payload


def consume():
    tick_processor = TickProcessor.build()

    def callback(_ch, _method, _properties, body):
        logger.info("Received data: {}".format(body))
        tick_info = parse(body)
        logger.info("Processing tick information: {}".format(pformat(tick_info)))
        tick_processor.process_tick(tick_info)

    credentials = pika.PlainCredentials('rabbitmq', 'rabbitmq')
    parameters = pika.ConnectionParameters(os.environ.get('RABBIT_HOST', 'localhost'),
                                           5672,
                                           '/',
                                           credentials)
    connection = pika.BlockingConnection(parameters)
    channel = connection.channel()

    channel.queue_declare(queue='threshold-alert')

    channel.basic_consume(callback,
                          queue='threshold-alert',
                          no_ack=True)

    channel.start_consuming()


def obtain_latest_stock_data(tick_url):
    try:
        retries = Retry(total=10,
                        backoff_factor=0.1,
                        status_forcelist=[ 500, 502, 503, 504 ])

        with requests.Session() as s:
            s.mount('http://', HTTPAdapter(max_retries=retries))
            return s.get(tick_url).json()
    except requests.RequestException:
        logger.warning("Failed to obtain stock tick data from url: '{}'", tick_url)
        raise
    except IOError:
        logger.exception("Exception while obtaining tick data")
        raise


def save_alert_occurrences(occurrences):
    db.session.add_all(occurrences)
    db.session.commit()


def is_recent(alert_occurrence):
    recent_notification = AlertNotification.query.filter(
        AlertNotification.alert_id == alert_occurrence.stock_alert_id)\
        .order_by(AlertNotification.timestamp.desc()).first()
    if recent_notification is None or datetime.utcnow() - recent_notification.timestamp > \
            alert_occurrence.stock_alert.alert_interval:
        return False
    return True


class NotifyClient:

    def __init__(self, backend_class):
        self.backend_class = backend_class

    def perform_notify(self, new_alerts):
        with self.backend_class() as n:
            for alert in new_alerts:
                if not is_recent(alert):
                    n.notify(alert.alert_notification())
                    db.session.add(AlertNotification(is_sent=True, alert_id=alert.stock_alert.id))
                    db.session.commit()


class AlertOccurrenceDetector:

    @staticmethod
    def get_alerts():
        return BaseStockAlert.query.all()

    @staticmethod
    def _process_alert(data, alert):
        try:
            current_price = data[alert.stock_code]
            return alert.check_for_alert_occurrence(current_price)
        except KeyError:
            return None

    def detect_alert_situations(self, data):
        alerts = self.get_alerts()
        price_map = {}

        for tick in data:
            price_map[tick['company']] = float(tick['price'])

        alert_occurrences = [x for x in map(partial(self._process_alert, price_map), alerts) if x is not None]
        if alert_occurrences:
            logger.info("Alert occurrence detected")
        save_alert_occurrences(alert_occurrences)

        return alert_occurrences


class TickProcessor:

    def __init__(self, notifier, alert_detector):
        self.notifier = notifier
        self.alert_detector = alert_detector

    def process_tick(self, tick_info):
        try:
            data = obtain_latest_stock_data(tick_info['url'])
        except IOError:
            logger.error("Dropped tick %s", tick_info)
        else:
            new_alerts = self.alert_detector.detect_alert_situations(data)
            self.notifier.perform_notify(new_alerts)

    @staticmethod
    def build():
        return TickProcessor(NotifyClient(RabbitNotify), AlertOccurrenceDetector())
