from rest_framework import serializers
from stockhistory.models import StockPriceTick


class StockPriceTickSerializer(serializers.ModelSerializer):

    class Meta:
        model = StockPriceTick
        fields = ['company', 'price']
