import os
import thresholdalert
import unittest
import tempfile


class TestCase(unittest.TestCase):

    def setUp(self):
        thresholdalert.app.testing = True
        self.app = thresholdalert.app.test_client()
        self.db = thresholdalert.db
        self.db.create_all()

    def tearDown(self):
        self.db.session.remove()
        self.db.drop_all()
