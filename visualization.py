import plotly
import csv
from plotly.graph_objs import Scatter, Layout

source_data_file = open("data.csv")
reader_source_data = csv.reader(source_data_file, delimiter=',')

data_links = []
data_name = []
data_objects = []
for row in reader_source_data:
    print(len(row))
    if len(row) > 2:
        data_links.append(row[3])
        data_name.append(row[1])
    if len(row) == 2:
        data_objects.append(row[1])

source_data_file.close()
print(data_links)
print(data_objects)

plotly.offline.plot({
    "data": [Scatter(x=data_name, y=data_links)],
    "layout": Layout(title="simle links data")

})



