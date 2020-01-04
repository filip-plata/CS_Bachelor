from random import choice
from django.urls import reverse
from django.contrib.auth.models import User
from wybory2000.tests import TestCase
from wybory2000.models import Region, VoteCandidate, Candidate, VoteResultSummary


class VotesUpdateTestCase(TestCase):
    def setUp(self):
        personal_data = [('Jan', 'Łopuszański'),
                         ('Aleksander', 'Kwaśniewski')]
        boroughs_data = [('00000000', 'Iłża'), ('00000001', 'Warszawa')]
        summary_data = [('00000000', 0, 0, 0)]

        boroughs = [Region(code=_code, name=_name)
                    for _code, _name in boroughs_data]
        candidates = [Candidate(first_name=_first, surname=_second)
                      for _first, _second in personal_data]
        summary = [VoteResultSummary(borough=code, ballot_papers=bal,
                                votes_invalid=inv, votes_valid=val)
                                for code, bal, inv, val in summary_data]
        candidate_votes = []

        Region.objects.bulk_create(boroughs)
        Candidate.objects.bulk_create(candidates)
        VoteResultSummary.objects.bulk_create(summary)

        candidates = Candidate.objects.all()

        for borough in boroughs:
            for candidate in candidates:
                candidate_votes.append(
                    VoteCandidate(borough=borough.code,
                                  candidate=candidate,
                                  votes=choice(range(1000))))

        VoteCandidate.objects.bulk_create(candidate_votes)

        self.url = reverse('votes-update')
        self.send_post = lambda data: self.client.post(self.url, data)

        User.objects.create_user(username="test", password="test")

    def test_logged_in(self):
        data = {'region-code': "00000000", 'candidate-id': 1, 'votes': -100}

        response = self.send_post(data)
        # Redirect to login view
        self.assertEqual(response.status_code, 403)

    def test_wrong_methods(self):
        wrong_methods = ['get', 'delete']

        for method in wrong_methods:
            method_call = getattr(self.client, method)
            self.client.login(username="test", password="test")
            response = method_call(self.url)
            self.client.logout()

            self.assertEqual(response.status_code, 405)

    def test_negative_votes_number(self):
        data = {'region-code': "00000000", 'candidate-id': 1, 'votes': -100}

        self.client.login(username="test", password="test")
        response = self.send_post(data)
        self.client.logout()
        self.assertEqual(response.status_code, 403)

    def test_all_ok(self):
        votes = choice(range(5000))
        data = {'region-code': "00000000", 'candidate-id': 1, 'votes': votes,
                'login': "test", 'password': "test"}

        self.client.login(username="test", password="test")
        response = self.send_post(data)
        self.client.logout()
        self.assertEqual(response.status_code, 200)

        vote_model = VoteCandidate.objects.filter(
            borough=data['region-code'],
            candidate=Candidate.objects.get(id=1))[0]

        self.assertEqual(votes, vote_model.votes)
