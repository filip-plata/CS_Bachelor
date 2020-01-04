import requests

base_str = "SkhUQzpKSFRD"
strings = []

for s in strings:
    r = requests.get("http://jhtc4bsk.jhtc.pl:40002/", auth=('Basic', s))
    if r.status_code != 403:
        print(r.content)
    
