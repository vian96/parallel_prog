import pandas as pd
import plotly.express as px

df1 = pd.read_csv('stats.csv')
df1['type'] += ' T single'
df2 = pd.read_csv('stats_nt.csv')
df2['type'] += ' noT single'
df3 = pd.read_csv('stats_nt_mt.csv')
df3['type'] += ' noT multi'
df4 = pd.read_csv('stats_mt.csv')
df4['type'] += ' T multi'

df = pd.concat([df2, df1, df3, df4])
fig = px.bar(df, x='n', y='t', color='type', barmode='group')
fig.write_html('stats.html', auto_open=True)

#df = pd.read_csv('stats_mt.csv')
#fig = px.scatter(df, x='n', y='t', color='type')
#fig.write_html('stats_mt.html', auto_open=True)
