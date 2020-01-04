from flask import Blueprint
from flask import request

from jsonschema import ValidationError, validate

from thresholdalert import app, db
from thresholdalert.models import LowValueStockAlert, HighValueStockAlert, BaseStockAlert

view = Blueprint('view', __name__)


def create_any_alert(alert_class, data):
    db.session.add(alert_class(**data))
    db.session.commit()
    return app.response_class(status=201)


@app.route('/')
def hello_world():
    return 'Hello World!'


@app.route('/alerts/<alert_id>', methods=['DELETE'])
def delete_alert(alert_id):
    obj = BaseStockAlert.query.filter(BaseStockAlert.id == alert_id).first()
    if obj is None:
        return app.response_class(status=404)
    db.session.delete(obj)
    db.session.commit()

    return app.response_class(status=200)


@app.route('/alerts', methods=['POST'])
def create_alert():
    alert_contents = {"low-value-alert.alerts": LowValueStockAlert,
                      "high-value-alert.alerts": HighValueStockAlert}

    for content_type, alert_class in alert_contents.items():
        if request.content_type == "application/" + content_type:
            json_schema = alert_class.get_schema()
            try:
                data = request.get_json(force=True)
                validate(data, json_schema)
                return create_any_alert(alert_class, data)
            except ValidationError:
                app.logger.error('Invalid json schema - use format:\n \n%s\n',
                                 json_schema)
                return app.response_class(status=400, response="Invalid data schema")
    else:
        return app.response_class(status=400, response="Invalid content type")
