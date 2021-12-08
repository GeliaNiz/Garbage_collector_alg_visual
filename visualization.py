import plotly
import pandas as pd
from plotly.subplots import make_subplots
import plotly.graph_objects as go
import csv
from sys import getsizeof
from plotly.graph_objs import Scatter, Layout

source_data = pd.read_csv('data.csv')

print(source_data)

fig = make_subplots(rows=2, cols=1, vertical_spacing=0.5)


fig.append_trace(go.Bar(x=source_data['Object name'], y=source_data['Links'],
                    ), row=1, col=1)

fig.append_trace(go.Scatter(x=source_data['Total objects'], y=source_data['Total memory in use']),
                 row=2, col=1)

fig.update_xaxes(title_text="Объекты", row=1, col=1)
fig.update_xaxes(title_text="Общее кол-во не удаленных объектов", row=2, col=1)

fig.update_yaxes(title_text="Общее кол-во ссылок на объекты", row=1, col=1)
fig.update_yaxes(title_text="Общее кол-во используемой памяти, байт", row=2, col=1)

fig.update_layout(height=600, width=1100, title_text="Visualization of GC work")
fig.show()




