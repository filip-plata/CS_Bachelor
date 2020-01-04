from channels import Group


def ws_connect(message):
    Group("votes-update").add(message.reply_channel)
    message.reply_channel.send({"accept": True})


def ws_disconnect(message):
    Group("votes-update").discard(message.reply_channel)
