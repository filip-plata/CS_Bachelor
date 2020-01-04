#!/bin/sh
docker build -t bsk-apache .
docker run -d -p 80:80 --name bsk-container bsk-apache
