from mock import MagicMock
import json
import httpretty

from tests import TestCase
from thresholdalert.events import TickProcessor, NotifyClient
from thresholdalert import events
from thresholdalert.models import LowValueStockAlert, HighValueStockAlert
from thresholdalert.models import AlertNotification, StockAlertOccurrence


class ProcessingTests(TestCase):

    def setUp(self):
        super(ProcessingTests, self).setUp()

        def test_notify(notification):
            self.notification = notification

        events.RabbitNotify.notify = MagicMock(test_notify)
        self.tick_processor = None
        self.tick_processor = None
        self.notification = None
        self.latest_stock_data = None
        self.low_value_alert = None
        self.high_value_alert = None
        self.http_tick = None
        self.data_url = "http://stocktick.pl/ticks"

    def test_processing_occurrence_should_trigger_notification(self):
        tick = [{'company': 'ASSECO', 'price': 55.}, {'company': 'GPW', 'price': 41.}]
        self.given_tick_processor()
        self.given_low_value_alert()

        self.when_given_tick(tick)

        self.then_alert_occurrence_was_generated_for(self.low_value_alert)

    def test_another_notification_not_send_when_previous_is_recent(self):
        tick = [{'company': 'ASSECO', 'price': 55.}, {'company': 'GPW', 'price': 41.}]
        self.given_tick_processor()
        self.given_low_value_alert()

        self.when_given_tick(tick)
        self.when_given_tick(tick)

        self.then_alert_occurrence_was_generated_for(self.low_value_alert)
        self.then_number_of_alerts_is(1)

    def test_when_no_occurrence_no_notification(self):
        tick = [{'company': 'ASSECO', 'price': 55.}, {'company': 'GPW', 'price': 39.}]
        self.given_tick_processor()
        self.given_high_value_alert()

        self.when_given_tick(tick)

        self.then_no_alert_occurrences()

    @httpretty.activate
    def test_tick_http_server_integration(self):
        tick = [{'company': 'ASSECO', 'price': 55.}, {'company': 'GPW', 'price': 41.}]
        self.given_tick_processor()
        self.given_low_value_alert()

        self.when_given_tick_http(tick)

        self.then_tick_was_downloaded()

    # Given methods start

    def given_tick_processor(self):
        self.tick_processor = TickProcessor.build()
        self.tick_processor.notifier = NotifyClient(MagicMock())

    def given_low_value_alert(self):
        self.low_value_alert = LowValueStockAlert(
            low_price_alert_value=56.0, stock_code='ASSECO', name='test_alert_low', user_id=1)
        self.db.session.add(self.low_value_alert)
        self.db.session.commit()

    def given_high_value_alert(self):
        self.high_value_alert = HighValueStockAlert(
            high_price_alert_value=40.0, stock_code='GPW', name='test_alert_high', user_id=1)
        self.db.session.add(self.high_value_alert)
        self.db.session.commit()

    # Given methods end

    # When methods start

    def when_given_tick(self, tick):
        self.latest_stock_data = tick
        events.obtain_latest_stock_data = \
            MagicMock(return_value=self.latest_stock_data)
        self.tick_processor.process_tick({"url": 'data-uri'})

    def when_given_tick_http(self, tick):
        self.latest_stock_data = tick
        httpretty.register_uri(
            httpretty.GET,
            self.data_url,
            body=json.dumps(tick)
        )
        self.http_tick = events.obtain_latest_stock_data(self.data_url)

    # When methods end

    # Then methods start

    def then_alert_occurrence_was_generated_for(self, alert):
        self.assertIsNotNone(
            StockAlertOccurrence.query.filter(
                StockAlertOccurrence.stock_alert_id == alert.id).first())

    def then_no_alert_occurrences(self):
        self.assertIsNone(StockAlertOccurrence.query.first())

    def then_number_of_alerts_is(self, count):
        self.assertEqual(AlertNotification.query.count(), count)

    def then_tick_was_downloaded(self):
        self.assertEqual(self.latest_stock_data, self.http_tick)

    # Then methods end
