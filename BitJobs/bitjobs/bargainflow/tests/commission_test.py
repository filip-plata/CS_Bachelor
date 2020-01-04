from random import choice

from django.contrib.auth.models import User
from django.urls import reverse

from hypothesis.extra.django.models import models
from hypothesis.strategies import just
from hypothesis.extra.django.models import default_value

from bargainflow.models import Commission
from bargainflow.tests import Client, TestCase, status


class CommissionApiTest(TestCase):

    USERS_NUMBER = 10
    COMMISSIONS_NUMBER = 100
    AUTH_USER = None
    OTHER_AUTH_USER = None
    USER_SECRET = "testing"
    OTHER_USER_SECRET = "forward-march"

    def setUp(self):
        users = [models(User).example()
                 for _ in range(CommissionApiTest.USERS_NUMBER)]


        test_commissions = [models(Commission, date_added=default_value,
                                   orderer=just(choice(users)),
                                   status=default_value,
                                   price=default_value).example()
                            for _ in range(CommissionApiTest.COMMISSIONS_NUMBER)]

        CommissionApiTest.AUTH_USER = User.objects.create_user(
                CommissionApiTest.USER_SECRET,
                'testing@test.com',
                CommissionApiTest.USER_SECRET)

        CommissionApiTest.OTHER_AUTH_USER = User.objects.create_user(
                CommissionApiTest.OTHER_USER_SECRET,
                'testing@test.com',
                CommissionApiTest.OTHER_USER_SECRET)

    def test_listing_commissions(self):
        url = reverse('bargainflow:commission-list')
        raw_response = self.client.get(url)
        api_response = raw_response.data
        self.assertEqual(raw_response.status_code, status.HTTP_200_OK)
        self.assertEqual(Commission.objects.count(), len(api_response))

    def test_create_commission_no_auth(self):
        user_ids = [user.id for user in User.objects.all()]
        data_dict = {'description': 'very important task',
                     'status': 'O', 'tags': [],
                     'orderer': choice(user_ids)}

        url = reverse('bargainflow:commission-list')
        raw_response = self.client.post(url, data_dict, format='json')

        self.assertEqual(raw_response.status_code, status.HTTP_403_FORBIDDEN)

    def test_create_commission_ok(self):
        user_ids = [user.id for user in User.objects.all()]
        data_dict = {'description': 'very important task',
                     'tags': ['dark-net'], 'title': 'Title'}

        url = reverse('bargainflow:commission-list')

        self.client.login(username=CommissionApiTest.USER_SECRET,
                          password=CommissionApiTest.USER_SECRET)
        raw_response = self.client.post(url, data_dict, format='json')

        self.assertEqual(raw_response.status_code, status.HTTP_201_CREATED)

        self.client.logout()

    def test_update_commission_status_ok(self):

        commission = Commission.objects.\
            create(orderer=CommissionApiTest.AUTH_USER,
                   description="Description", tags=[])
        c_id = commission.id
        data_dict = {'id': c_id}

        url = reverse('bargainflow:commission-detail', kwargs={'pk': c_id})

        self.client.login(username=CommissionApiTest.USER_SECRET,
                          password=CommissionApiTest.USER_SECRET)

        transitions = ['B', 'A', 'B', 'A', 'D', 'F']

        for t in transitions:
            data_dict['status'] = t
            raw_response = self.client.patch(url,
                                             data_dict,
                                             format='json')
            self.assertEqual(raw_response.status_code,
                             status.HTTP_200_OK)

        self.client.logout()

    def test_update_commission_status_wrong_auth(self):

        commission = Commission.objects.\
            create(orderer=CommissionApiTest.AUTH_USER,
                   description="Description", tags=[])
        c_id = commission.id
        data_dict = {'id': c_id, 'status': 'B'}

        url = reverse('bargainflow:commission-detail', kwargs={'pk': c_id})

        self.client.login(username=CommissionApiTest.OTHER_USER_SECRET,
                          password=CommissionApiTest.OTHER_USER_SECRET)

        raw_response = self.client.patch(url, data_dict, format='json')

        self.assertEqual(raw_response.status_code,
                         status.HTTP_403_FORBIDDEN)

        self.client.logout()

    def test_update_wrong_state_transition(self):

        commission = Commission.objects.\
            create(orderer=CommissionApiTest.AUTH_USER,
                   description="Description", tags=[])
        c_id = commission.id
        data_dict = {'id': c_id}

        url = reverse('bargainflow:commission-detail', kwargs={'pk': c_id})

        self.client.login(username=CommissionApiTest.USER_SECRET,
                          password=CommissionApiTest.USER_SECRET)

        transitions = [('A', False), ('F', False), ('B', True),
                       ('O', False), ('F', False), ('A', True),
                       ('O', False), ('F', False), ('D', True),
                       ('F', True)]

        for (t, is_ok) in transitions:
            data_dict['status'] = t
            raw_response = self.client.patch(url,
                                             data_dict,
                                             format='json')

            if is_ok:
                self.assertEqual(raw_response.status_code,
                                 status.HTTP_200_OK)
            else:
                self.assertEqual(raw_response.status_code,
                                 status.HTTP_403_FORBIDDEN)

        self.client.logout()
