FROM golang:latest

RUN go get github.com/ibm-security-innovation/crosscoap/...

CMD crosscoap -listen 0.0.0.0:5683 -backend http://127.0.0.1:48088/backend/iot -accesslog /tmp/iot.log
