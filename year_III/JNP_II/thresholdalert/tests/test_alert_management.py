import json

from tests import TestCase
from thresholdalert.models import BaseStockAlert, LowValueStockAlert, HighValueStockAlert


class AlertsTests(TestCase):

    HOME_PATH = "/"
    ALERTS_PATH = "/alerts"
    ALERTS_DELETE = "/alerts"

    LOW_VALUE_ALERT_CONTENT_TYPE = "application/low-value-alert.alerts"
    HIGH_VALUE_ALERT_CONTENT_TYPE = "application/high-value-alert.alerts"

    def setUp(self):
        super(AlertsTests, self).setUp()

        self.response = None
        self.low_value_alert = None

    def test_smoke_main_page(self):
        self.when_accessing_main_page()

        self.then_response_is_OK()

    def test_creating_alert_invalid_content_type(self):
        self.when_creating_alert_invalid_content_type()

        self.then_response_is_BAD_REQUEST()

    def test_creating_alert_invalid_json(self):
        self.when_creating_alert_invalid_json()

        self.then_response_is_BAD_REQUEST()

    def test_creating_alert_invalid_type(self):
        self.when_creating_alert_wrong_type()

        self.then_response_is_BAD_REQUEST()

    def test_creating_high_price_alert_valid(self):
        self.when_creating_valid_high_value_alert()

        self.then_response_is_CREATED()
        self.then_high_value_alert_is_created()

    def test_creating_low_price_alert_valid(self):
        self.when_creating_valid_low_value_alert()

        self.then_response_is_CREATED()
        self.then_low_value_alert_is_created()

    def test_deleting_existing_alert(self):
        self.given_alert()

        self.when_deleting_alert()

        self.then_response_is_OK()
        self.then_no_alerts_defined()

    def test_deleting_non_existing_alert(self):
        self.when_deleting_phantom_alert()

        self.then_response_is_NOT_FOUND()

    # Given methods start

    def given_alert(self):
        self.low_value_alert = LowValueStockAlert(
            user_id=1, low_price_alert_value=10., stock_code="CYFR", name="alert")
        self.db.session.add(self.low_value_alert)
        self.db.session.commit()

    # Given methods end

    # When methods start

    def when_creating_alert_invalid_content_type(self):
        self.send_create_alert_request("application/INVALID", None)

    def when_creating_alert_invalid_json(self):
        self.send_create_alert_request(self.LOW_VALUE_ALERT_CONTENT_TYPE,
                                       {"invalid": "json", "name": "error"})

    def when_creating_alert_wrong_type(self):
        self.send_create_alert_request("application/middle-value-alert.alerts",
                                       {"middle_price_alert_value": 21.0,
                                        "stock_code": 'PGE',
                                        "name": 'test_alert_low',
                                        "user_id": 4})

    def when_creating_valid_low_value_alert(self):
        self.send_create_alert_request(self.LOW_VALUE_ALERT_CONTENT_TYPE,
                                       {"low_price_alert_value": 21.0,
                                        "stock_code": 'PGE',
                                        "name": 'test_alert_low',
                                        "user_id": 4})

    def when_creating_valid_high_value_alert(self):
        self.send_create_alert_request(self.HIGH_VALUE_ALERT_CONTENT_TYPE,
                                       {"high_price_alert_value": 40.0,
                                        "stock_code": 'GPW',
                                        "name": 'test_alert_high',
                                        "user_id": 1})

    def when_deleting_alert(self):
        self.send_delete_alert(self.low_value_alert.id)

    def when_deleting_phantom_alert(self):
        self.send_delete_alert(2)

    def when_accessing_main_page(self):
        self.response = self.app.get(self.HOME_PATH, follow_redirects=True)

    # When methods end

    # Then methods start

    def then_response_is_BAD_REQUEST(self):
        self.assertEqual(self.response.status_code, 400)

    def then_response_is_NOT_FOUND(self):
        self.assertEqual(self.response.status_code, 404)

    def then_response_is_OK(self):
        self.assertEqual(self.response.status_code, 200)

    def then_response_is_CREATED(self):
        self.assertEqual(self.response.status_code, 201)

    def then_no_alerts_defined(self):
        self.assertEqual(BaseStockAlert.query.count(), 0)

    def then_low_value_alert_is_created(self):
        self.assertEqual(LowValueStockAlert.query.count(), 1)

    def then_high_value_alert_is_created(self):
        self.assertEqual(HighValueStockAlert.query.count(), 1)

    # Then methods end

    # Util methods start

    def send_create_alert_request(self, content_type, data):
        self.response = self.app.post(
            self.ALERTS_PATH, follow_redirects=True,
            content_type=content_type, data=json.dumps(data))

    def send_delete_alert(self, id_):
        self.response = self.app.delete("{}/{}".format(self.ALERTS_DELETE, id_))

    # Util methods end
