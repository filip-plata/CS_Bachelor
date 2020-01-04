# -*- coding: utf-8 -*-
import json

from django.views.generic import TemplateView, View
from django.views.decorators.http import require_POST
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import authenticate
from wybory2000.models import Candidate, VoteCandidate, VoteResultSummary,\
    VotingRegionManager, Region
from django.http import HttpResponse
from django.db import transaction
from django.db.models import Q
from django.db.models.functions import Length
from django.http import JsonResponse


class RegionResultsView(TemplateView):
    template_name = 'wybory2000/region_results.html'


class CountryResultsPage(RegionResultsView):
    template_name = 'wybory2000/index.html'


class BoroughBrowser(TemplateView):
    template_name = 'wybory2000/borough_browser.html'


@csrf_exempt
def borough_api(request):
    if request.is_ajax():
        q = request.GET.get('term', '')
        boroughs = Region.objects.annotate(code_len=Length('code')).filter(
                Q(name__icontains=q) & Q(code_len=8))[:20]
        results = []
        for borough in boroughs:
            borough_json = {}
            borough_json['id'] = borough.code
            borough_json['label'] = borough.name
            borough_json['value'] = borough.name

            results.append(borough_json)

        data = json.dumps(results)
    else:
        data = 'fail'

    mimetype = 'application/json'

    return HttpResponse(data, mimetype)


@csrf_exempt
@require_POST
def votes_api(request):
    borough_code = request.POST.get('region-code', None)
    candidate_id = request.POST.get('candidate-id', None)
    votes = request.POST.get('votes', None)
    login = request.POST.get('login', None)
    password = request.POST.get('password', None)

    if None in (borough_code, candidate_id, votes,
                login, password):
        return HttpResponse(status=403)

    if authenticate(
            username=login,
            password=password) is None:
        return HttpResponse(status=401)

    try:
        borough = VoteCandidate.objects.filter(
                        borough=borough_code,
                        candidate=candidate_id)[0]
        votes_summary = VoteResultSummary.objects.filter(
                            borough=borough_code)[0]

        with transaction.atomic():
            votes_difference = int(votes) - borough.votes
            borough.votes = int(votes)
            votes_summary.votes_valid += votes_difference
            votes_summary.ballot_papers += votes_difference

            borough.save()
            votes_summary.save()
    except:
        return HttpResponse(status=403)

    return HttpResponse()


class RegionSummaryAPI(View):

    def get(self, request, *args, **kwargs):
        region_id = self.kwargs['code']
        regman = VotingRegionManager(region_id)

        return JsonResponse(regman.get_votes_summary_json())


class RegionCandidatesAPI(View):

    def get(self, request, *args, **kwargs):
        region_id = self.kwargs['code']
        regman = VotingRegionManager(region_id)

        return JsonResponse(regman.get_results_candidate_json())


class SubregionsAPI(View):

    def get(self, request, *args, **kwargs):
        region_id = self.kwargs['code']
        regman = VotingRegionManager(region_id)

        return JsonResponse(regman.get_subregions_with_statistics_json())


class SubregionsLegendAPI(View):

    def get(self, request, *args, **kwargs):
        return JsonResponse(Candidate.legend_json())
