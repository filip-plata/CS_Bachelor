#!/usr/bin/env bash

# pipenv run celery multi start worker -A stockAPI.celery -B
printenv | grep -v "no_proxy" >> /etc/environment
service cron start

uwsgi --ini uwsgi.ini
