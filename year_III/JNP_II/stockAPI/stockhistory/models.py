from django.db import models

from decimal import Decimal

from stockhistory.utils import project_dict


class StockExchange(models.Model):
    class Meta:
        unique_together = ['codename', 'country']
    codename = models.CharField(max_length=16)
    country = models.CharField(max_length=8)


class Company(models.Model):
    class Meta:
        unique_together = ['code', 'stock_exchange']
    code = models.CharField(max_length=64, unique=True)
    stock_exchange = models.ForeignKey(StockExchange, null=False, on_delete=models.CASCADE)

    @classmethod
    def from_dict(cls, dictionary):
        return cls(**project_dict(dictionary, ['code', 'stock_exchange']))


class StockData(models.Model):
    company = models.ForeignKey(Company, null=False, to_field='code', max_length=64,
                                on_delete=models.CASCADE)
    datetime = models.DateTimeField(null=False, db_index=True)
    value = models.DecimalField(max_digits=20,decimal_places=5,
                                default=Decimal('0.00000'), null=False)
    transaction_volume = models.PositiveIntegerField(null=False)

    @classmethod
    def from_dict(cls, dictionary):
        return cls(**project_dict(dictionary, map(lambda x: x.field_name,
                                                  cls._meta.get_fields())))

    class Meta:
        index_together = [
            ['company', 'datetime'],
        ]


class StockPriceTick(models.Model):
    company = models.OneToOneField(Company, null=False, to_field='code',
                                   max_length=64, on_delete=models.CASCADE)
    price = models.DecimalField(max_digits=20,decimal_places=5,
                                default=Decimal('0.00000'), null=False)

    @classmethod
    def from_dict(cls, dictionary):
        return cls(**project_dict(dictionary, ['company_id', 'price']))
