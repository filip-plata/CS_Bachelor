# -*- coding: utf-8 -*-
import json

from channels import Group

from django.db import models
from django.db.models import Sum, F, Q
from django.core.validators import MinValueValidator
from django.core import serializers
from django.core.exceptions import ValidationError
from django.db.models.functions import Length
from django.forms.models import model_to_dict
from django.db.models import signals
from django.urls import reverse
from abc import abstractmethod


class _VotingRegion:

    @abstractmethod
    def get_code(self):
        pass

    @property
    def str_name(self):
        return str(self)

    def is_smallest(self):
        return len(self.get_code()) == 8

    def subregion_length(self):
        # We skip County region
        if len(self.get_code()) == 4:
            return 8
        return 2 + len(self.code)

    def get_results_candidate(self):
        """
        Shall return candidate with his results from region.
        """

        obj = VoteCandidate.objects.filter(borough__startswith=self.get_code())\
        .select_related('candidate_surname').annotate(surname=F('candidate__surname'))\
        .select_related('candidate_name').annotate(name=F('candidate__first_name')) \
        .values('candidate').annotate(candidate_result=Sum('votes'))

        # stupid but works, django ORM is a disaster comapred to SQL

        votes_sum = 0

        for cand_dict in obj:
            votes_sum += cand_dict['candidate_result']

        for cand_dict in obj:
            cand = Candidate.objects.get(pk=cand_dict['candidate'])
            cand_dict['surname'] = cand.surname
            cand_dict['name'] = cand.first_name
            cand_dict['color'] = cand.color
            cand_dict['id'] = cand.id
            cand_dict['percent_results'] = 100.0 * cand_dict['candidate_result'] / votes_sum

        return obj

    def get_subregions(self):
        """
        Should return all subregions in current region
        with data about number of votes for this region.
        """
        return Region.objects.annotate(code_len=Length('code')).filter(
                Q(code__startswith=self.get_code()) &
                Q(code_len=self.subregion_length()))

    def get_votes_summary(self):
        """
        Should return data about valid votes, invalid votes
        for current region.
        """

        obj = VoteResultSummary.objects.filter(borough__startswith=self.get_code())\
        .aggregate(votes_total_sum=Sum(F('votes_valid') + F('votes_invalid')), votes_valid_sum=Sum('votes_valid'),
                   votes_invalid_sum=Sum('votes_invalid'), ballot_papers=Sum('ballot_papers'))

        return obj

    def get_subregions_with_statistics(self):
        """
        Adds statistics for subregions. It is not the most
        efficient way to do so, but it short to code and fast enough.
        """
        subregions = self.get_subregions()
        return list(map(lambda x: (x, x.get_votes_summary(), x.get_results_candidate()), subregions))

    def get_absolute_url(self):
        from django.core.urlresolvers import reverse
        return reverse('region_results', args=[str(self.get_code())])


class Candidate(models.Model):
    class Meta:
        verbose_name = 'Kandydat'
        verbose_name_plural = 'Kandydaci'
    first_name = models.CharField(max_length=25)
    surname = models.CharField(max_length=25)
    color = models.CharField(max_length=7)

    def legend_json():
        candidates = [model_to_dict(c) for c in Candidate.objects.all()]
        return {'data': candidates, 'size': len(candidates)}

    def __str__(self):
        return "{} {}".format(self.first_name, self.surname)


class Region(models.Model, _VotingRegion):
    class Meta:
        verbose_name = 'Region'
        verbose_name_plural = 'Regiony'
    code = models.CharField("Kod", max_length=8, primary_key=True)
    name = models.CharField("Nazwa", max_length=35)
    region_type = models.CharField("Typ", max_length=35)

    def __str__(self):
        return "{} {}".format(self.region_type, self.name)

    def get_code(self):
        return self.code


class VoteResultSummary(models.Model):
    class Meta:
        verbose_name = 'Statystyki regionu'
        verbose_name_plural = 'Statystyki regionów'
    borough = models.CharField("Gmina", max_length=8, blank=False)

    def __str__(self):
        return self.region

    def clean(self):
        if self.ballot_papers < self.votes_total:
            raise ValidationError("Less ballot papers than votes")

    def save(self, *args, **kwargs):
        self.full_clean()
        super(VoteResultSummary, self).save(*args, **kwargs)

    @property
    def votes_total(self):
        return self.votes_valid + self.votes_invalid

    @property
    def region(self):
        return Region.objects.get(code=self.borough).name

    ballot_papers = models.PositiveIntegerField("Karty wydane")
    votes_valid = models.PositiveIntegerField("Głosy ważne")
    votes_invalid = models.PositiveIntegerField("Głosy nieważne")


class VoteCandidate(models.Model):
    class Meta:
        verbose_name = 'Głosy kandydata'
        verbose_name_plural = 'Głosy kandydatów'

    def __str__(self):
        return "Kod: {}, Kandydat: {}".format(
                            self.borough,
                            self.candidate.surname)

    def save(self, *args, **kwargs):
        self.full_clean()
        super(VoteCandidate, self).save(*args, **kwargs)

    borough = models.CharField("Gmina", max_length=8, blank=False)
    candidate = models.ForeignKey(Candidate, on_delete=models.CASCADE)
    votes = models.PositiveIntegerField("Głosy", validators=[MinValueValidator(0)])


def notify_votes_changed(sender, **kwargs):
    Group("votes-update").send({
        "text": kwargs["instance"].borough,
    })


signals.post_save.connect(notify_votes_changed,
                sender=VoteCandidate)


class VotingRegionManager(_VotingRegion):

    def __init__(self, code, *args, **kwargs):
        self.code = code

    def get_code(self):
        return self.code

    def get_results_candidate_json(self):
        res_cands = self.get_results_candidate()
        result = {'data': list(res_cands), 'size': str(len(res_cands))}
        return result

    def get_votes_summary_json(self):
        summary = self.get_votes_summary()

        json_dict = {'votes_total': summary['votes_total_sum'],
                     'votes_valid': summary['votes_valid_sum'],
                     'votes_invalid': summary['votes_invalid_sum'],
                     'ballot_papers': summary['ballot_papers'],
                     'code': self.code,
                     'str_name': Region.objects.get(pk=self.code).str_name,
                     'url': reverse('region_results', args=[self.code]),}
        return json_dict

    def get_subregions_with_statistics_json(self):
        subregions = self.get_subregions()
        stats = [{'results': VotingRegionManager(reg.get_code()).get_results_candidate_json(),
                'summaries': VotingRegionManager(reg.get_code()).get_votes_summary_json()}
                for reg in subregions]

        result = {'data': stats, 'size': str(len(stats))}
        return result
