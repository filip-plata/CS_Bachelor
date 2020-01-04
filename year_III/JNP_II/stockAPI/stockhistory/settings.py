import os
from django.conf import settings

DATA_DIR = getattr(settings, 'DATA_DIR', os.path.join(settings.BASE_DIR, "data"))

DOWNLOAD_URL = getattr(settings, 'DOWNLOAD_URL',
                       'https://www.bankier.pl/gielda/notowania/akcje')

# delay in seconds
STOCK_DATA_DELAY = 900
