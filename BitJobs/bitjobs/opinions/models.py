"""
Module for expressing opinions about commissions
"""

from datetime import datetime
from django.db import models
from django.db.models import Q
from django.contrib.auth.models import User
from bargainflow.models import Commission
from django.utils.translation import ugettext_lazy as _


class CannotCreateOpinion(Exception):
    def __init__(self, message):
        super(CannotCreateOpinion, self).__init__(message)


class Opinion(models.Model):
    RATINGS = [
        ('P', _('Positive')),
        ('N', _('Negative')),
        ('U', _('Undeclared')),
    ]

    opinion_giver = models.ForeignKey(User, on_delete=models.CASCADE,
                                      null=False)
    commission = models.ForeignKey(Commission)
    date_added = models.DateField(auto_now_add=True, null=False)
    description = models.TextField()
    rating = models.CharField(choices=RATINGS, max_length=1, default='U',
                              null=False)

    def __str__(self):
        return "Opinion giver: %s, Commission: %s, Description: %s" % \
               (self.opinion_giver.id, self.id, self.description[0:10])

    @staticmethod
    def add_opinion(opinion_giver, transaction, description, rating):

        if opinion_giver is not transaction.orderer \
                and opinion_giver is not transaction.contractor:

            raise CannotCreateOpinion(
                "%s did not participate in transaction %s"
                % (opinion_giver, transaction))

        opinions_about_transaction = \
            Opinion.objects.filter(commission=transaction)

        if opinion_giver.id in opinions_about_transaction.values_list(
                'opinion_giver__id', flat=True):

            raise CannotCreateOpinion(
                "%s has already commented on transaction %s"
                % (opinion_giver, transaction))

        Opinion.objects.create(
            opinion_giver=opinion_giver,
            commission=transaction,
            date_added=datetime.now(),
            description=description,
            rating=rating,
        )

    @staticmethod
    def remove_opinion(opinion_giver, transaction):
        Opinion.objects.filter(opinion_giver=opinion_giver,
                               commission=transaction).delete()

    # more of a user responsibility
    @staticmethod
    def list_current_given(user):
        return Opinion.objects.filter(opinion_giver=user)

    @staticmethod
    def list_current_about(user):
        return Opinion.objects.filter(
            Q(commission__orderer=user) | Q(commission__contractor=user))\
            .exclude(opinion_giver=user)

    @staticmethod
    def list_possible_give(user):
        commissions_with_user = Commission.objects.filter(
            Q(orderer=user) | Q(contractor=user))

        return commissions_with_user.exclude(
            id__in=Opinion.list_current_given(user)
            .values_list('commission__id', flat=True))

    @staticmethod
    def list_possible_about(user):
        commissions_with_user = Commission.objects.filter(
            Q(orderer=user) | Q(contractor=user))

        return commissions_with_user.exclude(
            id__in=Opinion.list_current_about(user)
            .values_list('commission__id', flat=True))
