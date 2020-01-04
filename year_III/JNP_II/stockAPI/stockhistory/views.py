from stockhistory.models import StockPriceTick
from stockhistory.serializers import StockPriceTickSerializer
from rest_framework import viewsets


class RetrieveLatestTick(viewsets.ModelViewSet):
    queryset = StockPriceTick.objects.all()
    serializer_class = StockPriceTickSerializer
