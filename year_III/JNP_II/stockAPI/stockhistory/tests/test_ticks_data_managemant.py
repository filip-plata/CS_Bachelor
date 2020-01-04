import os

from bs4 import BeautifulSoup
from django.test import TestCase
from unittest.mock import Mock

from schema import Schema

from stockhistory.data_managment import DataExtractingManager, DataArchiver, DownloadManager
from stockhistory.models import StockExchange, Company, StockPriceTick


class DataExtractorTest(TestCase):
    def setUp(self):
        stock_data_html_path = os.path.join(os.path.dirname(__file__),
                                            'stock-data-webpage.html')
        archiver = Mock()
        self.extractor = DataExtractingManager(archiver)
        with open(stock_data_html_path, 'r') as res:
            self.data_html = res.read()

    def test_parsing_html_row(self):
        """Parsing a data row works"""
        row = """
        <tr>
        <td class="colWalor textNowrap">
            <a title="Magna Polonia S.A." href="/inwestowanie/profile/quote.html?symbol=06MAGNA">06MAGNA</a>
        </td>
        <td class="colKurs change ">0,29</td>
        <td class="colZmiana change ">0,00</td>
        <td class="colZmianaProcentowa change ">0,00%</td>
        <td class="colLiczbaTransakcji">1</td>
        <td class="colObrot">0</td>
        <td class="colOtwarcie">0,29</td>
        <td class="calMaxi">0,29</td>
        <td class="calMini">0,29</td>
        <td class="colAktualizacja">30.04 11:00</td>
    
        </tr>"""
        result = self.extractor.process_row(BeautifulSoup(row, "html.parser"))

        self.assertDictEqual(result, {'code': "06MAGNA", 'price': 0.29})

    def test_parsing_html_page(self):
        """Parsing whole html page"""
        parse_result = self.extractor.parse_data(self.data_html)

        self.assertIsInstance(parse_result, list)
        self.assertTrue(len(parse_result) > 0)

        Schema([{'code': str, 'price': float}]).validate(parse_result)


class DataArchiverTest(TestCase):
    def setUp(self):
        self.stock = StockExchange(codename="GPW", country="pl")
        self.stock.save()
        self.archiver = DataArchiver(self.stock)

    def test_persisting_data(self):
        data = [{'code': 'C1', 'price': 1.05}, {'code': 'C2', 'price': 2.07}, {'code': 'C3', 'price': 71}]

        self.archiver.persist_data(data)

        self.assertEqual(Company.objects.all().count(), 3)


class DownloadManagerTest(TestCase):
    def setUp(self):
        self.stock = StockExchange(codename="GPW", country="pl")
        self.stock.save()
        self.download_manager = DownloadManager(DataExtractingManager(DataArchiver(self.stock)))

    def test_whole_downloading_flow(self):
        self.download_manager.run()
        self.assertTrue(StockPriceTick.objects.all())
        self.assertTrue(Company.objects.all())
