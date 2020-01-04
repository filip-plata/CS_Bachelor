""" Notify microservice

Usage:
  notify.py

"""
import smtplib
import pika
import logging
import os
import atexit

from docopt import docopt

from notify.notify import EmailNotifyBackend, do_notifying, DataAPIClient, MessageListener


def get_config():
    return {
        'SMTP_URL': os.environ['SMTP_URL'],
        'SMTP_PORT': os.environ['SMTP_PORT'],
        'SMTP_USER': os.environ['SMTP_USER'],
        'SMTP_PASSWORD': os.environ['SMTP_PASSWORD'],
        'CREDENTIALS_API': os.environ.get('CREDENTIALS_API', 'localhost:8050'),
        'RABBIT': os.environ.get('RABBIT', 'localhost')
    }


def main():
    docopt(__doc__)
    config = get_config()
    logging.info("Starting configuration:\n%s", config)

    email_backend = EmailNotifyBackend(config,
        DataAPIClient(config['CREDENTIALS_API']))

    credentials = pika.PlainCredentials('rabbitmq', 'rabbitmq')
    parameters = pika.ConnectionParameters(config['RABBIT'],
                                           5672,
                                           '/',
                                           credentials)
    connection = pika.BlockingConnection(parameters)
    atexit.register(lambda: connection.close())
    channel = connection.channel()

    do_notifying(channel, email_backend)
