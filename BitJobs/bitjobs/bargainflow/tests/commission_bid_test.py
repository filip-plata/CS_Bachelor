from random import choice

from django.contrib.auth.models import User
from django.urls import reverse

from hypothesis.extra.django.models import models
from hypothesis.strategies import just
from hypothesis.extra.django.models import default_value

from bargainflow.models import CommissionBid
from bargainflow.tests import Client, TestCase, status


class CommissionBidTest(TestCase):
    pass
