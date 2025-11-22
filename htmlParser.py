from bs4 import BeautifulSoup
import json

with open("test1.htm", "r", encoding="utf-8") as f:
    soup = BeautifulSoup(f, "html.parser")

json_blocks = soup.find_all("script", type="application/ld+json")
job_posting = None

#help from chat to get the correct place parsed from html
for block in json_blocks:
    try:
        data = json.loads(block.string)

        # Case 1: JSON is a dict and directly contains a JobPosting
        if isinstance(data, dict) and data.get("@type") == "JobPosting":
            job_posting = data
            break

        # Case 2: JSON contains @graph (most common)
        if "@graph" in data:
            for item in data["@graph"]:
                if item.get("@type") == "JobPosting":
                    job_posting = item
                    break
    except:
        continue

if job_posting is None:
    raise Exception("No jobBenefits in JSON block.")

loc = job_posting["jobLocation"]["address"]
skills = job_posting["skills"]
for s in skills:
    print("-", s["value"])

salary = job_posting["baseSalary"]["value"]["value"]
currency = job_posting["baseSalary"]["currency"]
print (salary, currency)