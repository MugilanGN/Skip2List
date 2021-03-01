import random
from tqdm import tqdm
import numpy as np

import matplotlib.pyplot as plt
#from scipy import interpolate

#random.seed(2)

def random_level(p):
    count = 0
    rand = 1
    
    while rand != 0:
        rand = random.randint(0,p-1)
        count += 1
        
    return count

def g_optimize(q, m):
    
    n = len(q) #number of nodes = n
    m += 1 #number of guard entries = m
    a = 1 #awareness of guard to surroundings
    
    maximum = max(q)
    minimum = min(q)
    
    rang = (maximum - minimum)/2
    
    Gs = [i*(n//m) for i in range(0, m+1)]
    
    print(Gs)
    
    count = 0

    for k in range(n//m):
        
        for i in range(1,m):
            
            l_freq = 0.1
            for j in range(Gs[i-1]+1, Gs[i]+a):
                l_freq += -1/q[j]
            
            r_freq = 0.1                  
            for j in range(Gs[i]+a, Gs[i+1]):
                r_freq += -1/q[j]
            
            if l_freq > r_freq:
                Gs[i] -= 1
                pass
            elif l_freq < r_freq:
                Gs[i] += 1
                pass
    
        count += 1
        
        print(count)
    
    return Gs

n = 1000
m = 30

def normal(x,mu,sigma):
    return ( 2.*np.pi*sigma**2. )**-.5 * np.exp( -.5 * (x-mu)**2. / sigma**2. )

q = [i**2 for i in range(0, n)]

plt.plot([i for i in range(0,n)],q)
plt.show()

Gs = g_optimize(q,m)

print(Gs)

spacing_list = []
for i in range(1, len(Gs)):
    spacing_list.append(Gs[i] - Gs[i-1])
print(spacing_list)
plt.plot([i for i in range(-1,m)],spacing_list)
plt.show()
