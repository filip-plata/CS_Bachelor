from django.contrib import admin

from stockhistory.models import StockExchange, StockPriceTick


class StockPriceTickAdmin(admin.ModelAdmin):
    list_display = ('company_code', 'price')

    def company_code(self, obj):
        return obj.company.code


admin.site.register(StockExchange)
admin.site.register(StockPriceTick, StockPriceTickAdmin)
