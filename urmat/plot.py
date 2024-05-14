import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import sys

n = int(sys.argv[1])
out = []
for i in range(n):
    out.append(pd.read_csv(f'out{i}.csv'))

df = pd.concat(out)
#print(df.pivot(index='x', columns='t', values='u'))

plt.imshow(df.pivot(index='x', columns='t', values='u'), aspect='auto')
plt.xlabel('time (index)')
plt.ylabel('x (index)')

plt.savefig('plt.png')

