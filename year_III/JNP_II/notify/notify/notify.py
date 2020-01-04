import json
import logging
import smtplib
import requests
from requests.packages.urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter

from email.mime.text import MIMEText

from jsonschema import validate, ValidationError

from pkg_resources import resource_string, resource_listdir


logging.basicConfig(
    format='%(asctime)s %(levelname)-8s %(message)s',
    level=logging.INFO,
    datefmt='%Y-%m-%d %H:%M:%S')

logger = logging.getLogger(__name__)


class UserNotFound(Exception):
    pass


class NotifyError(Exception):
    pass


class DataAPIClient:

    USERS_PATH = "/users/"

    def __init__(self, host):
        self.host = host

    def base_url(self):
        return self.host + self.USERS_PATH

    def resolve_user(self, user_id):
        url = self.base_url() + str(user_id)
        retries = Retry(total=10,
                        backoff_factor=0.1,
                        status_forcelist=[ 500, 502, 503, 504 ])
        with requests.Session() as s:
            s.mount('http://', HTTPAdapter(max_retries=retries))
            r = s.get(url)

        if r.status_code == 200:
            return r.json()
        if r.status_code == 404:
            raise UserNotFound()
        raise Exception("Unexpected error")


class EmailNotifyBackend:

    def __init__(self, config, api_client):
        print(resource_listdir('resources', ''))
        self.config = config
        self.template = self._read_template()
        self.notify_email = config['SMTP_USER']
        self.api_client = api_client

    def setup_SMTP(self):
        s = smtplib.SMTP(self.config['SMTP_URL'], self.config['SMTP_PORT'])
        s.ehlo()
        if 'SMTP_PASSWORD' in self.config:
            s.starttls()
            s.login(self.config['SMTP_USER'], self.config['SMTP_PASSWORD'])
        return s

    def notify(self, user_id, header, body=""):
        try:
            recipient = self.obtain_email(user_id)
            msg = self._get_msg(header, body)
            smtp = self.setup_SMTP()

            logger.info('Sending message to: %s', recipient)
            smtp.send_message(msg, self.notify_email, recipient)
            smtp.quit()
        except UserNotFound:
            logger.error('User id "%s" is invalid', user_id)
            raise NotifyError
        except Exception:
            logger.exception("Unknown exception when notifying")
            raise NotifyError

    def obtain_email(self, user_id):
        return self.api_client.resolve_user(user_id)['email']

    @staticmethod
    def _read_template():
        return resource_string('resources', 'email.template').decode()

    def _get_msg(self, header, body):
        msg = MIMEText(self.template.format(**{'header': header, 'body': body}))
        msg['Subject'] = header
        return msg


class MessageListener:

    message_schema = {
        "type": "object",
        "properties": {
            "user_id": {"type": "number"},
            "header": {"type": "string"},
            "body": {"type": "string"}
        },
        "required": ["user_id", "header", "body"]
    }

    def __init__(self, backend):
        self.backend = backend

    def obtain_rabbit_callback(self):
        def callback(ch, method, properties, body):
            self.consume(body)
        return callback

    def consume(self, msg):
        try:
            logger.info('Received payload: %s', msg)
            data = msg.decode('utf8')
            payload = json.loads(data)
            validate(payload, self.message_schema)
        except UnicodeDecodeError:
            logger.error('Invalid message encoding - use utf8')
        except ValueError:
            logger.error('Invalid json format: %s', msg)
        except ValidationError:
            logger.error('Invalid json schema - use format:\n \n%s\n',
                         self.message_schema)
        else:
            try:
                self.backend.notify(payload['user_id'], payload['header'], payload['body'])
                logger.info('Notified user with id: %d', payload['user_id'])
            except NotifyError:
                logger.error('Notifying user "%s" failed', payload['user_id'])


def do_notifying(channel, backend, queue='notification'):
    channel.queue_declare(queue=queue)
    channel.basic_consume(MessageListener(backend).obtain_rabbit_callback(),
                          queue=queue, no_ack=True)
    channel.start_consuming()
