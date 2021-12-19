import dash
from dash import dcc
from dash import html
import cufflinks as cf
import pandas as pd
import plotly
import plotly.graph_objects as go
import datetime
from plotly.graph_objs import Scatter, Layout
from plotly.subplots import make_subplots
from dash.dependencies import Input, Output

from pyorbital.orbital import Orbital


cf.set_config_file(offline=False, world_readable=True)
app = dash.Dash(__name__)
app.layout = html.Div(
    html.Div([
        html.H4('Garbage collector work visualization'),
        html.Div(id='live-update-text'),
        dcc.Graph(id='live-update-graph'),
        dcc.Interval(
            id='interval-component',
            interval=1*100, # in milliseconds
            n_intervals=0
        )
    ])
)

@app.callback(Output('live-update-graph', 'figure'),
              Input('interval-component', 'n_intervals'))
def update_graph_live(n):
    data = pd.read_csv('data.csv')

    fig = plotly.subplots.make_subplots(rows=3, cols=1, row_heights=[400, 500, 500], vertical_spacing=0.1)
    fig['layout']['margin'] = {
        'l': 10, 'r': 10, 'b': 10, 't': 10
    }
    fig['layout']['legend'] = {'x': 0, 'y': 1, 'xanchor': 'left'}
    fig['layout'].update(height=800, width=1400)
    print(data)
    format_date = []
    for i in data["Date"]:
        format_date.append(datetime.datetime.utcfromtimestamp(i).strftime("%Y-%m-%d %H:%M:%S"))
    fig.append_trace({
        'x': data["Object name"],
        'y': data["Links"],
        'name': 'Objects and links',
        'type': 'bar'
    }, 1, 1)
    fig.append_trace({
        'y': data['Total objects'],
        'x': format_date,
        'name': 'Objects quantity x Time',
        'mode': 'lines+markers',
        'type': 'scatter'
    }, 2, 1)
    fig.append_trace({
        'x': data['Total memory in use'],
        'y': data['Links deleted'],
        'name': 'Memory in use x Links deleted',
        'type': 'bar'
    }, 3, 1)

    return fig


if __name__ == '__main__':
    app.run_server(debug=True)
