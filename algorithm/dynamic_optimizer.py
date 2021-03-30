import random
from tqdm import tqdm
import numpy as np
import matplotlib.pyplot as plt
import math
import fenwick_tree as fenwick
import scipy.stats as stats

def normal(x,mu,sigma):
    return ( 2.*np.pi*sigma**2. )**-.5 * np.exp( -.5 * (x-mu)**2. / sigma**2. )

def sin(x, period):
    return np.sin(x*period)

def g_optimize(q, m):
    """
    kinda computes something that looks reasonable using dynamic programming,
    local search, and fenwick trees
    """
    
    n = len(q) # number of nodes = n
    m += 1 # number of guard entries = m
    a = 0 # look ahead of next guard val
    C = 1 # skip val that makes the guard entry go zoom
    
    S = [(n*i)//m for i in range(0, m+1)]
    # S contains the indices of the guard entries
    
    T = []
    # The ith index represents the frequency sum on the left of
    # the ith guard entry (from guard 1 to m)
    
    for i in range(1, m+1):
        
        freq = sum([q[j] for j in range(S[i-1], S[i]+a)]) 
        print(freq)
        T.append(freq)
    
    print(S, T)
    
    for k in tqdm(range(n)):
        
        for i in range(1,m):
            
            if (S[i] - S[i-1]) <= C or (S[i+1]-S[i]) <= C:
                continue
            
            l_freq = T[i-1]
            r_freq = T[i]
            
            if l_freq > r_freq:
                
                T[i-1] -= q[S[i]-1]
                T[i] += q[S[i]-1]
                
                S[i] -= C
                
            elif l_freq < r_freq:
                
                T[i-1] += q[S[i]]
                T[i] -= q[S[i]]
                
                S[i] += C
    
    return S

n = 10000
m = 40

#psycho piece-wise gaussian and linear distribution
#q = [5000000*normal(i, n//2, n//8) for i in range(0, n)]
#q += [0.0000001*i for i in range(n//2, n)]
#q = [20*(sin(i,0.0030)+1) for i in range(0, n)]
#q = [(math.log(i+1)) for i in range(0, n)]
#q = [i*(sin(i,0.004)+1) for i in range(0, n)]
q = [i**2 for i in range(0, n)]

#q = [i for i in range(0, n)]

#q[2000] += 1000
#q[4000] += 1000
#q[8000] += 1000

q = [int(x) for x in q]

plt.plot([i for i in range(0,n)],q)
plt.show()

plt.figure()

S = g_optimize(q,m)

spacing_list = []
for i in range(1, len(S)):
    spacing_list.append(S[i] - S[i-1])

maximum = max(spacing_list)

for i in range(0, len(S)-1):
    spacing_list[i] = maximum - spacing_list[i]


    
print("\n")
print(S)
print("\n")
print(spacing_list)

plt.plot([i for i in range(0,n)],q)
plt.figure()
plt.plot([i*n//m for i in range(1,m+2)],spacing_list)
plt.show()
