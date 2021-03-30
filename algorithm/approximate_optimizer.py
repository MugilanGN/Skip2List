import random
from tqdm import tqdm
import numpy as np
import matplotlib.pyplot as plt
import math
import fenwick_tree as fenwick

def normal(x,mu,sigma):
    return ( 2.*np.pi*sigma**2. )**-.5 * np.exp( -.5 * (x-mu)**2. / sigma**2. )

def sin(x, period):
    return np.sin(x*period)

def g_optimize(q, m):
    """kinda computes something that looks reasonable"""
    
    n = len(q) # number of nodes = n
    m += 1 # number of guard entries = m
    a = 1 # awareness of guard to surroundings
    
    S = [i*(n//m) for i in range(0, m+1)]
    # S is an array of the indices of the guard entries
    # it starts off uniformly spaced as that is optimal
    
    q = [0] + q
    tree = [0 for i in range(n+1)]

    for index in range(1, n+1):
        fenwick.update(index, q[index], q, tree)
    
    print(S)
    
    for k in tqdm(range(n)):
        
        for i in range(1,m):
            """
            l_freq = 0
            for j in range(S[i-1]+1, S[i]+a):
                l_freq += q[j]
                  
            r_freq = 0
            for j in range(S[i]+a, S[i+1]):
                r_freq += q[j]
            """
            
            l_freq = fenwick.get_range_sum(S[i-1]+1, S[i]+a-1, tree)
            r_freq = fenwick.get_range_sum(S[i]+a, S[i+1]-1, tree)
            
            if l_freq > r_freq:
                S[i] -= 1
            elif l_freq < r_freq:
                S[i] += 1
    
    return S

n = 10000
m = 40

#psycho piece-wise gaussian and linear distribution
#q = [normal(i, n//4, n/8) for i in range(0, n//2)]
#q += [0.0000001*i for i in range(n//2, n)]

q = [i*(sin(i,0.004)+1) for i in range(0, n)]
#q = [(math.log(i+1)) for i in range(0, n)]
#q = [5000*normal(i, n//2, n/8) for i in range(0, n)]


#q[2000] += 1000
#q[4000] += 1000
#q[8000] += 1000

plt.plot([i for i in range(0,n)],q)
plt.show()

plt.figure()

S = g_optimize(q,m)

spacing_list = []
for i in range(1, len(S)):
    spacing_list.append(S[i] - S[i-1])

print("\n")
print(S)
print("\n")
print(spacing_list)

plt.plot([i for i in range(0,n)],q)
plt.figure()
plt.plot([i*n//m for i in range(1,m+2)],spacing_list)
plt.show()
