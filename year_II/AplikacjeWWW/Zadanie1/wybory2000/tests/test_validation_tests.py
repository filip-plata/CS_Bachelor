from random import choice
from django.urls import reverse
from django.core.exceptions import ValidationError

from wybory2000.tests import TestCase
from wybory2000.models import Region, VoteCandidate, VoteResultSummary


class VotesUpdateTestCase(TestCase):
    def setUp(self):
        self.result_summary = VoteResultSummary(
                                    borough="Gmina", ballot_papers=5,
                                    votes_valid=3, votes_invalid=2)
        self.result_summary.save()

    def test_vote_result_summary_validation(self):
        try:
            self.result_summary.borough = ""
            self.result_summary.save()
            self.fail("Blank borough name passed!")
        except ValidationError:
            pass

        try:
            self.result_summary.votes_valid = 4
            self.result_summary.save()
            self.fail("More votes than ballot papers!")
        except ValidationError:
            pass

        try:
            self.result_summary.votes_invalid = -1
            self.result_summary.save()
            self.fail("Negative number of invalid votes!")
        except ValidationError:
            pass
