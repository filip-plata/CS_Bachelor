import threading
import logging

from logging import StreamHandler

from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate

from config import ProductionConfig


def create_app():
    app = Flask(__name__)
    app.config.from_object(ProductionConfig)

    return app


app = create_app()
db = SQLAlchemy(app)
migrate = Migrate(app, db)
handler = StreamHandler()
handler.setLevel(logging.INFO)
app.logger.addHandler(handler)

from thresholdalert.views import view
app.register_blueprint(view)

if not app.config['DEBUG']:
    from thresholdalert.events import consume
    threading.Thread(target=consume, daemon=True).start()
