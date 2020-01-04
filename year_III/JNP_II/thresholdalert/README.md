# Anomalies detecting microservice

## Summary

A simple microservice for detecting too low or
too high prices on stock market. It requires
working notification microservice, to which
it passes events of notice.

Processing is triggered by rabbit mq message
send to channel 'threshold-alert' of the following
format:

{'url': $URL}

where $URL is a URL from which latest stock
data can be downloaded.

Stock data must be in format {'$STOCK_CODE': $PRICE}.

Also it is possible to create and delete alerts
via sending requests to

/alerts

endpoint with required json data.
