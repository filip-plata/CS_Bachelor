from django.test import TestCase

from django.contrib.auth.models import User
from bargainflow.models import Commission
from .models import Opinion, CannotCreateOpinion


class OpinionTests(TestCase):
    n = 3
    u = []
    c = []

    @classmethod
    def setUpTestData(cls):
        for i in range(0, cls.n):
            cls.u.append(User(id=i, username=str(i)))
            cls.u[i].save()
        for i in range(0, cls.n):
            cls.c.append(Commission(orderer=cls.u[i],
                                    contractor=cls.u[(i+1) % cls.n],
                                    description=str(i)))
            cls.c[i].save()

    def test_add_correct(self):
        len = Opinion.objects.all().count()
        new_opinions_count = 2

        Opinion.add_opinion(self.u[0], self.c[0], "cow", 'P')
        Opinion.add_opinion(self.u[-1], self.c[-2], "sheep", 'N')

        len_after = Opinion.objects.all().count()
        self.assertEqual(len_after, len + new_opinions_count)

    def test_add_not_part(self):
        try:
            Opinion.add_opinion(self.u[0], self.c[1], "cow", 'P')
            self.fail("u[0] not in c[1]")
        except CannotCreateOpinion as e:
            pass

    def test_add_again(self):
        try:
            Opinion.add_opinion(self.u[0], self.c[0], "cow", 'P')
            Opinion.add_opinion(self.u[0], self.c[0], "sheep", 'N')
            self.fail("u[0] already gave opinion on c[0]")
        except CannotCreateOpinion as e:
            pass

    def test_current_given(self):
        self.assertEqual(Opinion.list_current_given(self.u[0]).count(), 0)
        Opinion.add_opinion(self.u[0], self.c[0], "cow", 'P')
        self.assertEqual(Opinion.list_current_given(self.u[0]).count(), 1)

    def test_current_about(self):
        self.assertEqual(Opinion.list_current_about(self.u[1]).count(), 0)
        Opinion.add_opinion(self.u[0], self.c[0], "cow", 'P')
        self.assertEqual(Opinion.list_current_about(self.u[1]).count(), 1)

    def test_possible_give(self):
        self.assertEqual(list(Opinion.list_possible_give(self.u[0])),
                         [self.c[0], self.c[-1]])
        Opinion.add_opinion(self.u[0], self.c[0], "cow", 'P')
        self.assertEqual(list(Opinion.list_possible_give(self.u[0])),
                         [self.c[-1]])

    def test_possible_about(self):
        self.assertEqual(list(Opinion.list_possible_about(self.u[1])),
                         [self.c[1], self.c[0]])
        Opinion.add_opinion(self.u[0], self.c[0], "cow", 'P')
        self.assertEqual(list(Opinion.list_possible_about(self.u[1])),
                         [self.c[1]])
