import json
import pprint

import psycopg
import requests
from bs4 import BeautifulSoup

words_ignore = ["Fall", "Spring", "Credits"]
major_degrees = [
    "applied-engineering-sciences",
    "biosystems-engineering",
    "chemical-engineering",
    "civil-engineering",
    "computational-data-science",
    "computer-engineering",
    "computer-science",
    "electrical-engineering",
    "environmental-engineering",
    "materials-science-engineering",
    "mechanical-engineering",
]

with psycopg.connect("dbname=college user=postgres password=Learn8128") as conn:
    with conn.cursor() as cur:
        cur.execute("""DELETE FROM scrapperdata""")
        conn.commit()
    conn.close()

for degree in major_degrees:
    r = requests.get(
        "https://engineering.msu.edu/academics/majors-degrees/"
        + degree
        + "-bs#curriculum"
    )

    soup = BeautifulSoup(r.content, "html.parser")
    data = []
    fixed_data = []
    spring = []
    fall = []
    data_dict = {}

    check = True
    count = 0

    for table in soup.find_all("table"):
        fall = []
        spring = []
        if count == 4:
            break
        count += 1
        for td in table.find_all("td"):
            try:
                if td.text.strip() in words_ignore:
                    continue
                else:
                    data.append(td.text.strip().replace("\n", " "))
            except Exception as e:
                print(e)
                continue

    for i in range(0, len(data), 2):
        if data[i] == "Total":
            if fall and spring:
                fixed_data.append(fall)
                fixed_data.append(spring)

            fall = []
            spring = []
            continue
        if check:
            fall.append(data[i])
            fall.append(data[i + 1])
            check = False
        else:
            spring.append(data[i])
            spring.append(data[i + 1])
            check = True
    college_year = ["freshman", "sophmore", "junior", "senior"]
    count = 0
    degree_dicitonary = {}

    for i in range(0, len(fixed_data), 2):
        fall_dict = {}
        spring_dict = {}
        for fall in range(0, len(fixed_data[i]), 2):
            count1 = 2
            while True:
                if fixed_data[i][fall] in fall_dict:
                    if count1 == 2:
                        fixed_data[i][fall] += " #" + str(count1)
                    else:
                        fixed_data[i][fall] = fixed_data[i][fall][:-1] + str(count1)
                    count1 += 1
                else:
                    break
            fall_dict[fixed_data[i][fall]] = fixed_data[i][fall + 1]
        for spring in range(0, len(fixed_data[i + 1]), 2):
            count2 = 2
            while True:
                if fixed_data[i + 1][spring] in spring_dict:
                    if count2 == 2:
                        fixed_data[i + 1][spring] += " #" + str(count2)
                    else:
                        fixed_data[i + 1][spring] = fixed_data[i + 1][spring][
                            :-1
                        ] + str(count2)
                    count2 += 1
                else:
                    break
            spring_dict[fixed_data[i + 1][spring]] = fixed_data[i + 1][spring + 1]

        degree_dicitonary[college_year[count]] = {
            "fall": fall_dict,
            "spring": spring_dict,
        }
        count += 1
    degree_dictionary_json = json.dumps(degree_dicitonary)

    with psycopg.connect("dbname=college user=postgres password=Learn8128") as conn:
        with conn.cursor() as cur:
            cur.execute(
                """INSERT INTO scrapperdata (degree, data) VALUES (%s, %s)""",
                (degree, degree_dictionary_json),
            )
            conn.commit()
        conn.close()
