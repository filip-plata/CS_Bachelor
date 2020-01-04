from django.contrib import admin
from wybory2000.models import Region, Candidate, VoteCandidate, \
        VoteResultSummary

class RegionAdmin(admin.ModelAdmin):
    search_fields = ['code', 'name', 'region_type']
    list_display = ['name', 'code']
    ordering = ['-name']


class CandidateAdmin(admin.ModelAdmin):
    pass


class VoteCandidateAdmin(admin.ModelAdmin):
    list_filter = ['candidate']
    list_display = ['borough', 'nazwisko', 'votes']
    search_fields = ['borough']

    def nazwisko(self, obj):
        return obj.candidate.surname


class VoteResultSummaryAdmin(admin.ModelAdmin):
    list_display = ['borough', 'region', 'ballot_papers',
                    'votes_valid', 'votes_invalid']
    search_fields = ['borough']


admin.site.register(Region, RegionAdmin)
admin.site.register(Candidate, CandidateAdmin)
admin.site.register(VoteCandidate, VoteCandidateAdmin)
admin.site.register(VoteResultSummary, VoteResultSummaryAdmin)
