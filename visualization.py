import dash
from dash import dcc
from dash import html
import pandas as pd
import plotly
import plotly.graph_objects as go
import datetime
from plotly.graph_objs import Scatter, Layout
from plotly.subplots import make_subplots
from dash.dependencies import Input, Output

from pyorbital.orbital import Orbital


satellite = Orbital('TERRA')
# cf.go_offline()
cf.set_config_file(offline=False, world_readable=True)
app = dash.Dash(__name__)
app.layout = html.Div(
    html.Div([
        html.H4('Garbage collector work visualization'),
        html.Div(id='live-update-text'),
        dcc.Graph(id='live-update-graph'),
        dcc.Interval(
            id='interval-component',
            interval=1*1000, # in milliseconds
            n_intervals=0
        )
    ])
)


@app.callback(Output('live-update-graph', 'figure'),
              Input('interval-component', 'n_intervals'))
def update_graph_live(n):
    satellite = Orbital('TERRA')
    data = pd.read_csv('data.csv')

    fig = plotly.subplots.make_subplots(rows=2, cols=1, vertical_spacing=0.2)
    fig['layout']['margin'] = {
        'l': 30, 'r': 10, 'b': 30, 't': 10
    }
    fig['layout']['legend'] = {'x': 0, 'y': 1, 'xanchor': 'left'}
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
        'x': format_date ,
        'name': 'Objects quantity x Time',
        'mode': 'lines+markers',
        'type': 'scatter'
    }, 2, 1)

    return fig


if __name__ == '__main__':
    app.run_server(debug=True)
