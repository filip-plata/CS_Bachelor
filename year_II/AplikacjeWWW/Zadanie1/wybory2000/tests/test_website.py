import time
import json
import requests
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
from django.urls import reverse
from django.conf import settings
from django.contrib.auth.models import User
from django.test import LiveServerTestCase, Client

from wybory2000.models import Candidate, VoteCandidate


class WebsiteTestCase(LiveServerTestCase):
    def setUp(self):
        self.selenium = webdriver.Firefox()
        # Classes for success and error notification
        self.err = "error-notify"
        self.suc = "success-notify"
        self.user = User.objects.create_user(
                username="Filip11",
                email="plata12@wp.pl",
                password="filip123")
        settings.CSRF_COOKIE_SECURE = False
        settings.SESSION_COOKIE_SECURE = False
        self.client = Client()
        super(WebsiteTestCase, self).setUp()

    @property
    def notify(self):
        try:
            WebDriverWait(self.selenium, 10).until(
                EC.presence_of_element_located((By.XPATH,
                    '//div[@id="notification" and text() != ""]')))
        except TimeoutException:
            raise Exception('Unable to find text in this element after waiting 10 seconds')
        return self.selenium.find_element_by_id("notification")

    def register_request(self, usr, pwd1, pwd2):
        selenium = self.selenium
        selenium.get("http://127.0.0.1:8000/user/rejestracja/")

        user = selenium.find_element_by_id("id_username")
        password1 = selenium.find_element_by_id("id_password1")
        password2 = selenium.find_element_by_id("id_password2")

        submit = selenium.find_element_by_xpath("//button[@type='submit']")

        [el.clear() for el in (user, password1, password2)]
        user.send_keys(usr)
        password1.send_keys(pwd1)
        password2.send_keys(pwd2)

        submit.send_keys(Keys.ENTER)

        try:
            WebDriverWait(selenium, 10).until(
                EC.presence_of_element_located((By.XPATH,
                    '//*[@id="notification" and text() != ""]')))
        except TimeoutException:
            raise Exception('Unable to find text in this element after waiting 10 seconds')

        return selenium.find_element_by_id("notification")

    def login_user(self, usr, pwd):
        selenium = self.selenium
        selenium.get("http://127.0.0.1:8000/user/logowanie/")

        user = selenium.find_element_by_id("id_login")
        password = selenium.find_element_by_id("id_hasło")

        submit = selenium.find_element_by_xpath("//button[@type='button']")

        [el.clear() for el in (user, password)]
        user.send_keys(usr)
        password.send_keys(pwd)

        submit.send_keys(Keys.RETURN)

        try:
            WebDriverWait(selenium, 10).until(
                EC.presence_of_element_located((By.XPATH,
                    '//*[@id="notification" and text() != ""]')))
        except TimeoutException:
            raise Exception('Unable to find text in this element after waiting 10 seconds')

        return selenium.find_element_by_id("notification")

    def browse_borough(self, name):
        selenium = self.selenium
        selenium.get("http://127.0.0.1:8000/gminy/")

        search = selenium.find_element_by_id("boroughs")
        search.send_keys(name)

        try:
            WebDriverWait(selenium, 10).until(
                EC.presence_of_element_located((By.XPATH,
                                                '//a[@tabindex="-1"]')))
        except TimeoutException:
            raise Exception('Unable to find text in this element after waiting 10 seconds')

        search.send_keys(Keys.ENTER)

        try:
            WebDriverWait(selenium, 10).until(
                EC.presence_of_element_located((By.XPATH,
                                                "//section[@id='results-candidates']/table/tbody")))
        except TimeoutException:
            raise Exception('Unable to find text in this element after waiting 10 seconds')

    def change_votes(self, borough, cand_id, votes):
        data = { 'candidate-id': cand_id,
                 'region-code': borough,
                 'votes': votes,
                 'login': "Filip",
                 'password': "mplaton1",
        }
        requests.post("http://localhost:8000/api/votes-update/", data)

    def assert_element(self, xpath, msg):
        try:
            self.selenium.find_element_by_xpath(xpath)
        except Exception:
            self.fail(msg)

    def test_register_short_password(self):
        selenium = self.selenium
        self.register_request("Filip2", "filip", "filip")
        notification = self.notify
        self.assertEqual(notification.get_attribute("class"), self.err)

    def test_register_distinct_password(self):
        selenium = self.selenium
        self.register_request("Filip2", "filio", "filip")
        notification = self.notify
        self.assertEqual(notification.get_attribute("class"), self.err)

    def test_register_user_exists(self):
        selenium = self.selenium
        self.register_request("Filip", "filip12", "filip12")
        notification = self.notify
        self.assertEqual(notification.get_attribute("class"), self.err)

    def test_borough_browser(self):
        selenium = self.selenium

        self.browse_borough("Iłż")

        row_count = len(selenium.find_elements_by_xpath("//section[@id='results-candidates']/table/tbody/tr"))
        # There were 12 candidates
        self.assertEqual(12, row_count)

        self.browse_borough("Warszawa")
        row_count = len(selenium.find_elements_by_xpath("//section[@id='results-candidates']/table/tbody/tr"))
        # There were 12 candidates
        self.assertEqual(12, row_count)

    def test_modyfing_borough_votes(self):
        selenium = self.selenium
        votes_old = 334
        votes_new = 123

        borough_name = "Iłża"
        borough_code = "14352503"
        
        self.browse_borough(borough_name)

        self.change_votes(borough_code, 7, votes_new)

        try:
            if self.notify.get_attribute("class") != self.suc:
                print(self.notify.text)
                self.fail("Wrong notification class")

            time.sleep(1)
            cand_el = selenium.find_elements_by_xpath("((//section[@id='results-candidates']/table/tbody/tr)[7]/td)[3]")[0]

            self.assertEqual(str(votes_new), cand_el.text)
        except Exception as e:
            self.change_votes(borough_code, 7, votes_old)
            self.fail("Automatic push of data change failed")

        self.change_votes(borough_code, 7, votes_old)

    def test_html_main(self):
        selenium = self.selenium
        selenium.get("http://127.0.0.1:8000/")

        self.assert_element("//div[@id='notification']",
                            "Notification div must be present")
        self.assert_element("//nav", "nav must be present")
        self.assert_element("//header", "header must be present")
        self.assert_element("//div[@id='poland-map']", "Poland map must be present")
        self.assert_element("//footer", "footer must be present")

        try:
            WebDriverWait(selenium, 10).until(
                EC.presence_of_element_located((By.XPATH,
                                                "//section[@id='subregions']/table/tbody")))
        except TimeoutException:
            raise Exception('Unable to find text in this element after waiting 10 seconds')

        row_count = len(selenium.find_elements_by_xpath("//section[@id='subregions']/table/tbody/tr"))

        # Jest 16 województw
        self.assertEqual(16, row_count)

    def tearDown(self):
        self.selenium.quit()
        super(WebsiteTestCase, self).tearDown()
