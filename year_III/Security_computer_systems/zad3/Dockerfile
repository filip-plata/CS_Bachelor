FROM debian:latest

RUN apt-get update -y
RUN apt-get install apache2 -y

RUN echo "solab01" >> /etc/hosts
RUN echo "vsolab01" > /var/www/html/index.html

EXPOSE 80

CMD apachectl -D FOREGROUND
