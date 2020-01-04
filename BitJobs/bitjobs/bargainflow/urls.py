from django.conf.urls import url, include

from rest_framework.routers import DefaultRouter

from bargainflow import views

router = DefaultRouter()

router.register(r'commissions',
                views.CommissionViewSet,
                base_name='commission')

router.register(r'commission-bids',
                views.CommissionBidViewSet,
                base_name='commission_bid')


urlpatterns = [
    url(r'autocomplete-search', views.get_commissions,
        name="autocomplete-commission"),
    url(r'^', include(router.urls, namespace='bargainflow')),
]
