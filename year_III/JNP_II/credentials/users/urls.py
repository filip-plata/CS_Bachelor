from rest_framework import routers
from users.views import RetrieveUserView


router = routers.SimpleRouter()
router.register(r'users', RetrieveUserView)
urlpatterns = router.urls
