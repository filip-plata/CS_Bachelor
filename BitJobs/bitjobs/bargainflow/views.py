from rest_framework import viewsets
from rest_framework.decorators import permission_classes
from rest_framework.permissions import IsAuthenticated

from bargainflow.permissions import CommissionPermission
from bargainflow.models import Commission, CommissionBid
from bargainflow.serializers import CommissionBidSerializer, \
    CommissionSerializer
import json
from django.http import HttpResponse
from django.conf import settings


class CommissionViewSet(viewsets.ModelViewSet):
    """
    Commissions endpoint.
    """
    queryset = Commission.objects.all()
    serializer_class = CommissionSerializer
    permission_classes = (CommissionPermission,)


class CommissionBidViewSet(viewsets.ModelViewSet):
    """
    Commission bids endpoint.
    """
    queryset = CommissionBid.objects.all()
    serializer_class = CommissionBidSerializer


def get_commissions(request):
    if request.is_ajax():
        q = request.GET.get('term', '')
        commissions = Commission.objects.filter(description__icontains=q)[:settings['AUTOCOMPLETE_LENGTH']]
        results = []
        for comm in commissions:
            commission_json = {'id': comm.id, 'label': comm.title,
                               'value': comm.title}
            results.append(commission_json)
        data = json.dumps(results)
    else:
        data = 'fail'
    mimetype = 'application/json'

    return HttpResponse(data, mimetype)

