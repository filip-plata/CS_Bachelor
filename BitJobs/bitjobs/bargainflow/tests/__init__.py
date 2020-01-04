from rest_framework.test import APIRequestFactory
from rest_framework.test import APITestCase
from rest_framework import status


Client = APIRequestFactory
TestCase = APITestCase

__all__ = ["Client", "TestCase", "status"]
