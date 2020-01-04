from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.CountryResultsPage.as_view(), name='country_results'),
    url(r'^region/(?P<object_id>((\d){0}))/$', views.CountryResultsPage.as_view(),
        name='region_results'),
    url(r'^region/(?P<object_id>((\d+)))/$', views.RegionResultsView.as_view(),
        name='region_results'),
    url(r'^gminy/', views.BoroughBrowser.as_view(), name='borough-browser'),
    url(r'^api/boroughs-get/', views.borough_api, name='boroughs-get'),
    url(r'^api/votes-update/', views.votes_api, name='votes-update'),
    url(r'^api/region/(?P<code>(\d*))/summary/', views.RegionSummaryAPI.as_view(),
            name='api-region-summary'),
    url(r'^api/region/(?P<code>(\d*))/candidates/', views.RegionCandidatesAPI.as_view(),
            name='api-region-candidates'),
    url(r'^api/region/(?P<code>(\d*))/subregions/', views.SubregionsAPI.as_view(),
            name='api-subregions'),
    url(r'^api/legend/', views.SubregionsLegendAPI.as_view(),
            name='api-subregions-legend'),
]
