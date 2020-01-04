# Languages and programming tools II

A do it yourself project, which had to have microservices.

### React stock

Simple system for monitoring stock data according to custom strategies, with focus on short term speculation.

When events of interest start to happen, it notifies end user via various means.

## System overview

System consists of a few parts:

* DataAPI, responsible for providing simple interface for both historic and real time stock events
* Notification service, which handles notifying end users about interesting events
* Strategies manager services, which oversees Strategies
* Strategies services themselves, which process new stock data
* Credentials service, which stores data about users
