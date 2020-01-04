from __future__ import unicode_literals

from django.db import migrations


def forward_func(apps, schema_editor):
    Wallet = apps.get_model('cc', 'Wallet')
    Currency = apps.get_model('cc', 'Currency')
    db_alias = schema_editor.connection.alias
    ltc = Currency.objects.using(db_alias).create(label='Litecoin', ticker='LTC',
                                                  api_url='http://alicja:wkrainie@localhost:9432')
    Wallet.objects.using(db_alias).create(label='master_wallet', currency=ltc)


def reverser_func(apps, schema_editor):
    Wallet = apps.get_model('cc', 'Wallet')
    Currency = apps.get_model('cc', 'Currency')
    db_alias = schema_editor.connection.alias

    ltc = Currency.objects.using(db_alias).get(ticker='LTC')
    Wallet.objects.using(db_alias).filter(label='master_wallet', currency=ltc).delete()
    Currency.objects.using(db_alias).filter(ticker='LTC').delete()


class Migration(migrations.Migration):
    dependencies = [
        ('moneyflow', '0007_auto_20170508_1737')
    ]

    operations = [
        migrations.RunPython(forward_func, reverser_func)
    ]
