from django.apps import AppConfig


class KwiatekConfig(AppConfig):
    name = 'kwiatek'

    def ready(self):
        from kwiatek.signals import *
        super(KwiatekConfig, self).ready()
