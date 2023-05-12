import csv
from datetime import datetime


entries=[]
with open("capture.txt", "r") as fp:
    entries = fp.readlines()


for line in entries:
    if(line=='\n' or line=='Auth Failed\n' or line=='uth Failed\n'):
        continue
    entry = line.split("] ")
    entry[0] = entry[0][1:20]
    entry[1] = entry[1][:-1]
    format = '%Y-%m-%d %H:%M:%S'
    datetime_str = datetime.strptime(entry[0], format)
    entry[0] = "'" + datetime_str.strftime("%d-%b-%Y  %H:%M:%S")
    found=False
    rows=[]
    new_rows = []
    with open("Attendance.csv", "r") as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            rows.append(row)

    for row in rows:
        if(str(row[0]) == entry[1]):
            found=True
            remain = False
            new_row = []
            for item in row:
                if(len(item)==0 and not remain):
                    remain=True
                    new_row.append(entry[0])
                    continue
                new_row.append(item)
            # do something
            if(not remain):
                new_row[1] = row[2]
                new_row[2] = row[3]
                new_row[3] = row[4]
                new_row[4] = row[5]
                new_row[5] = entry[0]
            new_rows.append(new_row)
            continue
        new_rows.append(row)

    if(not found):
        new_row = []
        new_row.append(entry[1])
        new_row.append(entry[0])
        new_row.append('')
        new_row.append('')
        new_row.append('')
        new_row.append('')
        new_rows.append(new_row)


    with open("Attendance.csv", "w", newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(new_rows)


with open("capture.txt", 'w') as file:
    pass
    

        
    








# # date_time_str = now.strftime("%Y-%m-%d %H:%M:%S")
