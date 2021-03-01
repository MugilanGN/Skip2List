import random
import matplotlib.pyplot as plt
from scipy import interpolate

random.seed(2)

def random_level(p):
    count = 0
    rand = 1
    
    while rand != 0:
        rand = random.randint(0,p-1)
        count += 1
        
    return count

def guard_cost(N, h, q):
        
    l = 1
    r = 1
    p = 0.5
    
    N = [0] + N
    m = len(N) - 1
    
    max_h = max(h)
    
    total_cost = 0
    
    for i in range(1,m+1):
        
        G = 0
        
        for j in range(1, i):
            G += N[j]
            
        cost = 0
        
        for j in range(G+1, G + N[i] + 1):
            
            cost += (l*(max_h - h[j]) + r*(p**(h[j]))*(j-G))
         
        freq = (sum(q[G+1:G+N[i]+1])+1)#/(sum(q))
        #cost = cost*N[i]
        
        total_cost += 1/freq
    
    return (total_cost)
    
n = 100
    
h = [0] + [random_level(2) for i in range(0,n)]
q = [0] + [i for i in range(0,n)] 

print(q)

xpoints = [i for i in range(1,n)]
ypoints = [guard_cost([x,n-x],h,q) for x in range(1,n)]

print(h)
print(ypoints.index(min(ypoints)) + 1, n - ypoints.index(min(ypoints)) - 1)
print(min(ypoints))

plt.plot([0]+xpoints+[n], q)

plt.figure()

plt.plot(xpoints, ypoints)
plt.show()
