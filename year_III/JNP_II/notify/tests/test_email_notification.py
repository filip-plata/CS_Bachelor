from unittest.mock import MagicMock
import httpretty

from notify import EmailNotifyBackend, MessageListener, DataAPIClient

import smtpd
import asyncore
import smtplib
import threading


class CustomSMTPServer(smtpd.SMTPServer):

    def __init__(self, *args, **kwargs):
        super(CustomSMTPServer, self).__init__(*args, **kwargs)
        self.msg_count = 0
        self.last_rcpttos = None

    def process_message(self, peer, mailfrom, rcpttos, data, **kwargs):
        self.msg_count += 1
        self.last_rcpttos = rcpttos


class SMTPManager:
    def __init__(self, port):
        self.port = port

    def __enter__(self):
        self.smtp = CustomSMTPServer(('127.0.0.1', self.port), None)
        self.thread = threading.Thread(target=asyncore.loop, kwargs={'timeout': 1})
        self.thread.daemon = True
        self.thread.start()
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.smtp.close()
        return False


def test_sending_email():
    # given
    port = 1025
    receiver = 'engel@mimuw.edu.pl'

    with SMTPManager(port) as m:
        server = smtplib.SMTP('localhost', port)
        email_backend = EmailNotifyBackend(
          {'SMTP_PORT': port, 'SMTP_URL': 'localhost',
           'SMTP_USER': 'notify@stockalert.pl'}, None)
        email_backend.obtain_email = MagicMock(return_value=receiver)

        # when
        email_backend.notify(1, 'Important')
        email_backend.notify(1, 'Even More Important')

        # then
        assert m.smtp.msg_count == 2
        assert m.smtp.last_rcpttos == [receiver]


def test_receiving_invalid_format_data():
    # given
    payload = 'invalid data'.encode('utf-8')
    email_backend = MagicMock()
    listener = MessageListener(email_backend)

    # when
    listener.consume(payload)

    # then
    assert not email_backend.notify.called


def test_receiving_invalid_json_scheme_data():
    # given
    payload = '{"user_id": 1, "header": "important"}'.encode('utf-8')
    email_backend = MagicMock()
    listener = MessageListener(email_backend)

    # when
    listener.consume(payload)

    # then
    assert not email_backend.notify.called


def test_receiving_not_utf_8_data():
    # given
    payload = 'invalid data'.encode('utf-16')
    email_backend = MagicMock()
    listener = MessageListener(email_backend)

    # when
    listener.consume(payload)

    # then
    assert not email_backend.notify.called


def test_receiving_valid_json():
    # given
    payload = '{"user_id": 1, "header": "important", "body": "Alert!"}'.encode('utf-8')
    email_backend = MagicMock()
    listener = MessageListener(email_backend)

    # when
    listener.consume(payload)

    # then
    assert email_backend.notify.called
    email_backend.notify.assert_called_with(1, 'important', 'Alert!')


@httpretty.activate
def test_resolving_user():
    url = "http://credentials"
    user_id = 1
    user_email = "test@example.com"
    data_client = DataAPIClient(url)
    httpretty.register_uri(
    httpretty.GET,
        data_client.base_url() + str(user_id),
        body='{"email": "' + user_email + '"}'
    )

    user_data = data_client.resolve_user(1)

    assert 'email' in user_data
    assert user_data['email'] == user_email
