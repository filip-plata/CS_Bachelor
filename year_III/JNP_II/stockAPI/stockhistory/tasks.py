import os
import pika
import json
from celery.task import task
from stockhistory.data_managment import DownloadManager, DataExtractingManager, DataArchiver
from stockhistory.models import StockExchange


@task()
def obtain_stock_tick():
    DownloadManager(DataExtractingManager(DataArchiver(StockExchange.objects.get(country="pl")))).run()
    credentials = pika.PlainCredentials('rabbitmq', 'rabbitmq')
    parameters = pika.ConnectionParameters(os.environ.get('RABBIT_HOST', 'localhost'),
                                           5672,
                                           '/',
                                           credentials)
    connection = pika.BlockingConnection(parameters)
    channel = connection.channel()
    channel.queue_declare(queue='threshold-alert')
    channel.basic_publish(exchange='',
                          routing_key='threshold-alert',
                          body=json.dumps({'url': 'http://stock-api/ticks'}))
    connection.close()
