#!/usr/bin/python3

import csv
import os
import sys
import django
import logging
import itertools
from django.core.management import call_command


_candidates = []
cand_start = 11


if __name__ == "__main__":
    os.environ.setdefault("DJANGO_SETTINGS_MODULE", "Zadanie1.settings")
    django.setup()
    # Cannot import those models without setting up django
    from wybory2000.models import VoteCandidate, VoteResultSummary, Region, Candidate


def _extended_region_code(row, length):
    constituency_code = str(row[1]).zfill(2)
    region_code = row[2][:length]
    result = [region_code[:2], constituency_code, region_code[2:length]]

    return "".join(result)


def clear_database():
    # clears database
    call_command('flush', interactive=False)


def generate_color():
    colors = ['#DC143C', '#00008B', '#B8860B',
              '#8FBC8F', '#FFD700', '#008000',
              '#ADD8E6', '#800000', '#CD853F',
              '#4169E1', '#EE82EE', '#9ACD32']

    pool = itertools.cycle(colors)

    for color in pool:
        yield color


def create_candidate_data(candidate_list):
    gen = generate_color()

    for candidate in candidate_list:
        splited = candidate.split()
        Candidate.objects.create(first_name=" ".join(splited[:-1]),
                                 surname=splited[-1],
                                 color=next(gen))

    global _candidates
    _candidates = list(Candidate.objects.all())


def create_vote_candidate_data(row, header):
    """
    Returns a list of vote objects.
    """

    def _get_candidate(idx, header, _candidates):
        candidate_surname = header[cand_start + idx].split()[-1]

        for candid in _candidates:
            if candid.surname == candidate_surname:
                return candid

        return None

    votes_data = []

    for idx, candidate_votes in enumerate(row[cand_start:]):
        candidate = _get_candidate(idx, header, _candidates)
        votes_object = VoteCandidate(votes=candidate_votes,
                                     borough=_extended_region_code(row, 6),
                                     candidate=candidate)
        votes_data.append(votes_object)

    return votes_data


def create_vote_summary_data(row):
    return VoteResultSummary(borough=_extended_region_code(row, 6),
                             ballot_papers=row[7],
                             votes_invalid=row[9],
                             votes_valid=row[10])


def create_vote_data(csv_stream):
    votes_summary = []
    votes_candidates_data = []

    header = next(csv_stream)  # remove header line

    for row in csv_stream:
        votes_candidates_data.extend(create_vote_candidate_data(row, header))
        votes_summary.append(create_vote_summary_data(row))

    VoteResultSummary.objects.bulk_create(votes_summary)
    VoteCandidate.objects.bulk_create(votes_candidates_data)


def create_regions_data(csv_stream):

    def _create_constituency_data(row):
        return Region(code=_extended_region_code(row, 2),
                      region_type=("Okręg " + row[1]))

    def _create_borough_data(row):
        return Region(name=row[3], code=_extended_region_code(row, 6),
                      region_type="Gmina")

    def _create_county_data(row):
        return Region(name=row[4], code=_extended_region_code(row, 4),
                      region_type="Powiat")

    def _create_province_data():
        # Some data must be sadly hardcoded
        province_codes = {"02": "Dolnośląskie", "04": "Kujawsko-Pomorskie",
                          "06": "Lubelskie", "08": "Lubuskie", "10": "Łódzkie",
                          "12": "Małopolskie", "14": "Mazowieckie", "16": "Opolskie",
                          "18": "Podkarpackie", "20": "Podlaskie", "22": "Pomorskie",
                          "24": "Śląskie", "26": "Świętokrzyskie", "28": "Warmińsko-Mazurskie",
                          "30": "Wielkopolskie", "32": "Zachodniopomorskie"}

        Region.objects.create(code="", name="Kraj", region_type="Kraj")

        for code, province in province_codes.items():
            Region.objects.create(code=code, name=province,
                                  region_type="Województwo")

    _create_province_data()

    csv_stream, csv_stream_borough = itertools.tee(csv_stream)

    constituencies = []
    counties = []
    boroughs = []

    #hack, but quick
    constituencies_codes = set()
    counties_codes = set()

    for row in csv_stream_borough:
        constituency_object = _create_constituency_data(row)
        county_object = _create_county_data(row)
        borough_object = _create_borough_data(row)

        boroughs.append(borough_object)

        if county_object.code not in counties_codes:
            counties.append(county_object)

        if constituency_object.code not in constituencies_codes:
            constituencies.append(constituency_object)

        #as up, hack
        counties_codes.add(county_object.code)
        constituencies_codes.add(constituency_object.code)

    Region.objects.bulk_create(boroughs)
    Region.objects.bulk_create(counties)
    Region.objects.bulk_create(constituencies)


def setup_meta_data(csv_stream):
    create_candidate_data(next(csv_stream)[cand_start:])
    create_regions_data(csv_stream)


def import_csv(filename="pkw2000.csv"):
    clear_database()

    logging.info("Database data destroyed")

    with open(filename, newline='') as data_file:
        data_reader = csv.reader(data_file, delimiter=",")

        setup_meta_data(data_reader)

        logging.info("Regions and candidates data is set")

        # rewinding to 0
        data_file.seek(0)
        data_reader = csv.reader(data_file, delimiter=",")

        create_vote_data(data_reader)


if __name__ == "__main__":
    import_csv()
