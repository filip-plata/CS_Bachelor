import datetime
from abc import abstractmethod

from sqlalchemy.orm import relationship

from thresholdalert import db


class StockAlertOccurrence(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    stock_alert_id = db.Column(
        db.Integer, db.ForeignKey('base_stock_alert.id'), nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.datetime.utcnow)
    description = db.Column(db.Text, nullable=False)
    summary = db.Column(db.String(256), nullable=False)

    stock_alert = relationship(
        'BaseStockAlert', foreign_keys='StockAlertOccurrence.stock_alert_id')

    def alert_notification(self):
        return {'header': self.summary,
                'body': self.description,
                'user_id': self.stock_alert.user_id}


class BaseStockAlert(db.Model):
    __tablename__ = 'base_stock_alert'
    id = db.Column(db.Integer, primary_key=True)
    stock_code = db.Column(db.String(64), nullable=False)
    name = db.Column(db.String(128), unique=True, nullable=False)
    user_id = db.Column(db.Integer, nullable=False)
    alert_interval = db.Column(db.Interval, nullable=False, default=datetime.timedelta(days=1))

    type = db.Column(db.String(64), nullable=False)
    __mapper_args__ = {
        'polymorphic_identity': 'base_stock_alert',
        'polymorphic_on': type
    }

    @classmethod
    def get_schema(cls):
        return {
            "type": "object",
            "properties": {
                "stock_code": {"type": "string"},
                "name": {"type": "string"},
                "user_id": {"type": "integer"},
                "alert_interval": {"type": "string", "format": "date-time"}
            },
            "required": ["stock_code", "name", "user_id"]
        }

    def check_for_alert_occurrence(self, value):
        if self.is_alert_condition(value):
            return StockAlertOccurrence(
                stock_alert_id=self.id,
                summary=self.occurrence_summary(),
                description=self.occurrence_description(value))
        return None

    @abstractmethod
    def is_alert_condition(self, value):
        pass

    @abstractmethod
    def occurrence_summary(self):
        pass

    @abstractmethod
    def occurrence_description(self, value):
        pass


class AlertNotification(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    is_sent = db.Column(db.Boolean, default=False, nullable=False)
    alert_id = db.Column(db.ForeignKey(BaseStockAlert.id), nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.datetime.utcnow)


class LowValueStockAlert(BaseStockAlert):
    __tablename__ = 'low_value_stock_alert'
    id = db.Column(db.Integer, db.ForeignKey('base_stock_alert.id'), primary_key=True)
    low_price_alert_value = db.Column(db.Float, nullable=False)

    __mapper_args__ = {
        'polymorphic_identity': 'low_value_stock_alert',
    }

    @classmethod
    def get_schema(cls):
        result = super().get_schema()
        result["properties"]["low_price_alert_value"] = {"type": "number", "minimum": 0}
        result["required"].append("low_price_alert_value")
        return result

    def is_alert_condition(self, value):
        return value < self.low_price_alert_value

    def occurrence_description(self, value):
        return "Price reached level of {}".format(value)

    def occurrence_summary(self):
        return "Low price"


class HighValueStockAlert(BaseStockAlert):
    __tablename__ = 'high_value_stock_alert'
    id = db.Column(db.Integer, db.ForeignKey('base_stock_alert.id'), primary_key=True)
    high_price_alert_value = db.Column(db.Float, nullable=False)

    __mapper_args__ = {
        'polymorphic_identity': 'high_value_stock_alert',
    }

    @classmethod
    def get_schema(cls):
        result = super().get_schema()
        result["properties"]["high_price_alert_value"] = {"type": "number", "minimum": 0}
        result["required"].append("high_price_alert_value")
        return result

    def is_alert_condition(self, value):
        return value > self.high_price_alert_value

    def occurrence_description(self, value):
        return "Price reached level of {}".format(value)

    def occurrence_summary(self):
        return "High price"
