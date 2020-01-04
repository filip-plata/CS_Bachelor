from rest_framework import routers
from stockhistory.views import RetrieveLatestTick


router = routers.SimpleRouter()
router.register(r'ticks', RetrieveLatestTick)
urlpatterns = router.urls
