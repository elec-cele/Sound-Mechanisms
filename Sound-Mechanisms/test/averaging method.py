# -*- coding: utf-8 -*-
"""
Created on Wed May 17 16:27:19 2023

@author: s
"""
import matplotlib.pyplot as plt
import random

data = []
for n in range(2):
    data += [1 for i in range(random.randint(500,1000))]
    data += [0 for i in range(random.randint(800,1000))]
    
    
    
avgd = []
k = 50
i = 0

accumulate = data[0]
accumulate_step = 1 / k
nmax = len(data)
for n in data:
    if n <= 1e-5:
        accumulate = 0
        avgd.append(0)
    else:
        samps_after = min(nmax - i, k)
        index_after = i + samps_after
        a = 1 / samps_after
        nextsum = a * sum(data[i:index_after])
        if nextsum <= accumulate:
            accumulate = min(1,nextsum)
            avgd.append(accumulate)
        else:
            accumulate += a
            avgd.append(min(accumulate,1))

    i += 1


plt.figure(figsize=(20,10))
plt.plot(data)
plt.plot(avgd)
plt.show()