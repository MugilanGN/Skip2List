import random
from tqdm import tqdm

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
        
    q2 = []
    for freq in q:
        q2.append([q.index(freq), freq])
    
    q2.sort(key=lambda x: x[1])
    
    if q2[-1][0] == len(q)-1:
        G_proto = q2[-m : -1] + [q2[-1]]
    else:
        G_proto = q2[-m+1 : ]
    
    Gs = [0] + [x for (x,y) in G_proto]
    
    print(Gs)
    
        
    print(Gs)
    
    count = 0

    for k in range(n//m):
        
        for i in range(1,m):

            l_freq = 0.1
            for j in range(Gs[i-1]+1, Gs[i]+a):
                l_freq += q[j]
            
            r_freq = 0.1                  
            for j in range(Gs[i]+a, Gs[i+1]):
                r_freq += q[j]
            
            if l_freq > r_freq:
                Gs[i] -= 1
                pass
            elif l_freq < r_freq:
                Gs[i] += 1
    
        count += 1
        
        print(count)
    
    return Gs

n = 100
m = 4

q = [1 for i in range(0,n)]

Gs = g_optimize(q,m)

print(Gs)

spacing_list = []
for i in range(1, len(Gs)):
    spacing_list.append(Gs[i] - Gs[i-1])
print(spacing_list)
plt.plot([i for i in range(-1,m)],spacing_list)
plt.show()
