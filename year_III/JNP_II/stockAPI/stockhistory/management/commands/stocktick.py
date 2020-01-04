from django.core.management.base import BaseCommand
from stockhistory.tasks import obtain_stock_tick


class Command(BaseCommand):
    help = 'Download latest stock tick from market'

    def handle(self, *args, **options):
        obtain_stock_tick()
