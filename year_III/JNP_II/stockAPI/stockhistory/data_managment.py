import logging

import requests

from locale import atof
from bs4 import BeautifulSoup
from datetime import datetime

from django.db import IntegrityError, Error, transaction

from stockhistory import settings
from stockhistory.models import Company, StockPriceTick


logger = logging.getLogger(__name__)


class DownloadFailure(Exception):
    pass


class ParseError(Exception):
    pass


class DataArchiver:
    def __init__(self, stock_xchg):
        self.stock_xchg = stock_xchg

    def convert_into_models(self, data):
        stock_prices = []
        companies = []

        for data_dict in data:
            # create new tick data
            data_dict['company_id'] = data_dict['code']
            data_dict['stock_exchange'] = self.stock_xchg
            companies.append(Company.from_dict(data_dict))
            stock_prices.append(StockPriceTick.from_dict(data_dict))

        return stock_prices, companies

    @staticmethod
    def _persist_companies(companies):
        for company in companies:
            try:
                company.save()
                logger.info("Saved new company: %s", company.code)
            except IntegrityError:
                logger.debug("Company with code %s already exists", company.code)

    @staticmethod
    @transaction.atomic
    def _persist_ticks(stock_prices):
        try:
            StockPriceTick.objects.all().delete()
            StockPriceTick.objects.bulk_create(stock_prices)
        except Error:
            logger.error("Could not save stock data; time: %s",
                         datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

    def persist_data(self, data):
        stock_prices, companies = self.convert_into_models(data)

        self._persist_companies(companies)
        self._persist_ticks(stock_prices)


class DataExtractingManager:
    def __init__(self, archiver):
        self.archiver = archiver

    @staticmethod
    def get_data_table(html_page):
        return BeautifulSoup(html_page, "html.parser").find("table")

    # Unstable code - depends on Bankier website
    @staticmethod
    def process_row(row):
        data = row.find_all('td')
        # Parsing string from polish formatting
        result = {'code': data[0].text.strip(),
                  'price': atof(data[1].text.replace(",", "."))}
        return result
    # end unstable code

    def parse_data(self, html_page):
        try:
            table_tag = self.get_data_table(html_page)
        except Exception:
            raise ParseError("Could not parse table")

        result = []

        for row in table_tag.find_all('tr'):
            try:
                data_dict = self.process_row(row)
            except (ParseError, Exception):
                logger.debug("Parsing %s failure", str(row))
            else:
                result.append(data_dict)

        return result

    def extract_data(self, data):
        try:
            parsed_data = self.parse_data(data)
        except (ParseError, Exception):
            logger.error("Could not parse html data")
        else:
            self.archiver.persist_data(parsed_data)


class DownloadManager:
    def __init__(self, extractor):
        self.URL = settings.DOWNLOAD_URL
        self.extractor = extractor

    def download_page(self):
        return requests.get(self.URL).content

    def obtain_current_stock_data(self):
        page = self.download_page()
        self.extractor.extract_data(page)

    def run(self):
        self.obtain_current_stock_data()
