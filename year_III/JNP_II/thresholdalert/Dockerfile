FROM python:3.6
MAINTAINER Filip Plata "fp371335@students.mimuw.edu.pl"

COPY . /thresholdalert
WORKDIR /thresholdalert
RUN pip3 install -r requirements.txt

ENV PORT 8010
EXPOSE 8010

ENTRYPOINT ["gunicorn", "--bind", "0.0.0.0:8000", "wsgi"]
