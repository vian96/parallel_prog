import pandas as pd
import plotly.express as px

import sys

for i in [1,2,3]:
    df = pd.read_csv(f'out{i}.csv')
    df['s'] = df['time'][df['n']==1].values[0] /  df['time']
    df['e'] = df['s'] /  df['n']
    
    #df1 = pd.read_csv('stats.csv')
    #df1['type'] += ' T single'
    #df2 = pd.read_csv('stats_nt.csv')
    #df2['type'] += ' noT single'
    #df3 = pd.read_csv('stats_nt_mt.csv')
    #df3['type'] += ' noT multi'
    #df4 = pd.read_csv('stats_mt.csv')
    #df4['type'] += ' T multi'

    #df = pd.concat([df2, df1, df3, df4])
    #df = pd.read_csv('csv_simd.csv')
    #fig = px.bar(df, x='n', y='t', color='type', barmode='group')
    #fig = px.bar(df, x='n', y='time') #, color='type', barmode='group')
    #fig.write_html(f'stats{i}.html', auto_open=False)

    fig = px.bar(df, x='n', y='s') #, color='type', barmode='group')
    fig.write_html(f'speed{i}.html', auto_open=False)
    fig = px.bar(df, x='n', y='e') #, color='type', barmode='group')
    fig.write_html(f'eff{i}.html', auto_open=False)

    #df = pd.read_csv('stats_mt.csv')
    #fig = px.scatter(df, x='n', y='t', color='type')
    #fig.write_html('stats_mt.html', auto_open=True)
