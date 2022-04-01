from dash_extensions.enrich import Input, Output, State, DashProxy, MultiplexerTransform
from dash import dcc, html

import plotly.figure_factory as ff

import numpy as np
import pandas as pd

import base64
import io

class SAirParameter:
    def __init__(self, id, label, valueName):
        self.id = id
        self.label = label
        self.valueName = valueName

airParameters = [
    SAirParameter("temp", "Temperature", "Temperature (Celsius)"),
    SAirParameter("humidity", "Humidity", "Humidity (Percent)"),
    SAirParameter("co", "CO", "CO gas (ppm)"),
    SAirParameter("co2", "CO2", "CO2 gas (ppm)"),
    SAirParameter("nh3", "NH3", "NH3 gas (ppm)"),
    SAirParameter("no2", "NO2", "NO2 gas (ppm)"),
    SAirParameter("tvoc", "TVOC", "Organic compounds (ppm)"),
]

class CHexbinVisualizer():
    def __init__(self, width=2048, height=1024, density=5, opacity=0.4, showPoints=True, pointsOpacity=0.2, pointsColor="red"):
        self.__width = width
        self.__height = height
        self.__density = density
        self.__opacity = opacity
        self.__showPoints = showPoints
        self.__pointsOpacity = pointsOpacity
        self.__pointsColor = pointsColor
        self.__dataFrame = None
        self.__parameter = airParameters[0]
        self.__fig = {}

    def GetHexbin(self):
        return self.__fig
    
    def SetDataFrame(self, df):
        self.__dataFrame = df
        self.__updateHexbin()

    def SetDisplayParameter(self, param):
        self.__parameter = param
        self.__updateHexbin()

    def __updateHexbin(self):
        self.__fig = ff.create_hexbin_mapbox(
            data_frame=self.__dataFrame,
            nx_hexagon=self.__density,
            opacity=self.__opacity,
            show_original_data=self.__showPoints,
            original_data_marker=dict(size=4, opacity=self.__pointsOpacity, color=self.__pointsColor),

            lat="lat", lon="lon",
            color_continuous_scale="YLOrRd",

            color=self.__parameter.id,
            labels=dict(color=self.__parameter.valueName),
            agg_func=np.mean
        )

        self.__fig.update_layout(mapbox_style="open-street-map", autosize=False, width=self.__width, height=self.__height)

## A visualization tool
hexbin = CHexbinVisualizer()

## Initialize app with same output usage for multiple callbacks 
app = DashProxy(prevent_initial_callbacks=True, transforms=[MultiplexerTransform()])

## Application page layout
app.layout = html.Div([
    # File upload button
    dcc.Upload(html.Button('Upload Data File'), id='upload-data'),

    # A display text for uploaded file
    html.Ul(id="choosen-file"),

    # Display parameter choose
    html.P("Choose parameter:"),
    dcc.RadioItems(
        id='air_param', 
        options=[{'value': i, 'label': param.label} for i, param in enumerate(airParameters)],
        value=0,
        labelStyle={'display': 'inline-block'}
    ),

    # A hexbin graph visualizer
    dcc.Graph(id="hexbin"),
])

@app.callback(  [Output("choosen-file", "children"), Output("hexbin", "figure")],
                Input('upload-data', 'contents'),
                State('upload-data', 'filename'),
                State('upload-data', 'last_modified'))
def UpdateDataFrame(contents, filename, last_modified):
    if contents is not None:
        content_type, content_string = contents.split(',')
        decoded = base64.b64decode(content_string)
        try:
            if 'csv' in filename:
                hexbin.SetDataFrame(pd.read_csv(io.StringIO(decoded.decode('utf-8'))))
                return [html.Div([f"Current file: {filename}"]), hexbin.GetHexbin()]
        except Exception as e:
            print(e)
    return [[], {}]

@app.callback(Output("hexbin", "figure"), Input("air_param", "value"))
def display_hexbin(airParamId):
    hexbin.SetDisplayParameter(airParameters[airParamId])
    return hexbin.GetHexbin()

app.run_server(debug=True)