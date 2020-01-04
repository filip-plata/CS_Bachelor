from django.contrib.auth.models import User
from django.urls import reverse
from rest_framework import status
from rest_framework.test import APITestCase


class UserInfoAccess(APITestCase):
    def setUp(self):
        User.objects.create_user(username='testuser', password='12345', email="example@no.com")

    def test_obtaining_email(self):
        """
        Ensure we obtain email information.
        """
        url = reverse('user-detail', kwargs={'pk': 1})
        response = self.client.get(url, format='json')
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data['email'], "example@no.com")

    def test_creating_user(self):
        """
        Ensure we can create a new user object.
        """
        url = reverse('user-list')
        response = self.client.post(url, {'username': 'testing', 'email': 'test@test.com',
                                          'password': '1234'}, format='json')
        self.assertEqual(response.status_code, status.HTTP_201_CREATED)
        self.assertEqual(User.objects.count(), 2)
