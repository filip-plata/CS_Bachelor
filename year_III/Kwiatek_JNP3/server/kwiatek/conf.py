from appconf import AppConf


class MyAppConf(AppConf):
    IOT_PROXY_HOST = "localhost"
    IOT_PROXY_PORT = "5683"
    ALARM_COOLDOWN = 3600
