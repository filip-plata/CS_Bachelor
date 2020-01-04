FROM python:3.6-alpine

COPY . /notify
WORKDIR /notify

RUN pip install pipenv
RUN pipenv install --system

CMD  python __main__.py
