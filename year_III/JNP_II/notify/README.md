# Notify microservice

The goal of this service is to provide notification mechanism for alerts.

## Configuration

Three environemnt variables must be set:

CREDENTIALS_API, with URL of data API instance, default is localhost: 8050
RABBIT,  URL of rabbitmq broker, default is localhost
SMTP, URL of smtp server

Data API must provide endpoint /users/${user_id}, which must return JSON with 'email' field inside.

The notifier listens on 'notification' queue.
Payload must contain at least a valid JSON with fields: 'user_id', 'header' and 'body'.
