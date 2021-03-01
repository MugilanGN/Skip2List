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

def g_optimize(h, q, m):
    
    m += 1
        
    #Gs = sorted(random.sample(range(1, len(q)), m))
    
    Gs = [i*(len(q)//m) for i in range(1, m)]
    
    Gs = [0] + Gs + [len(q) - 1]
    
    print(Gs)
    
    count = 0
    
    old_Gs = []
    
    while True:
        
        #if count % 2:
        #z = range(m-1,0,-1)
        #else:
        z = range(1,m)
    
        for j in z:
            
            G = Gs[j]

            l_freq = 0.1
            r_freq = 0.1

            for i in range(Gs[j-1]+1, G):
                l_freq += q[i]

            for i in range(G, Gs[j+1]):
                r_freq += q[i]

            old = 1/(l_freq) + 1/(r_freq)
                
            l_shift = 1/(l_freq - q[G-1]) + 1/(r_freq + q[G-1])
            r_shift = 1/(l_freq + q[G]) + 1/(r_freq - q[G])

            if old < l_shift and old < r_shift:        
                break
                
            if G== 99 or G == 0:
                break

            else:
                if l_shift < r_shift:
                    G -= 1
                elif l_shift > r_shift:
                    G += 1
                else:
                    break
            
            Gs[j] = G
        
        #print(Gs, old_Gs)
        
        if Gs == old_Gs:
            break
        
        old_Gs = Gs.copy()
    
        count += 1
        
        #print(Gs)
        
        print(count)
    
    return Gs

n = 10000
    
h = [0] + [random_level(2) for i in range(0,n)]
q = [i**2 for i in range(0,n)]

# += 100

m = 2

Gs = g_optimize(h,q,m)

print(Gs)

spacing_list = []

for i in range(1, len(Gs)):
    spacing_list.append(Gs[i] - Gs[i-1])

print(spacing_list)

plt.plot([i for i in range(-1,m)],spacing_list)
plt.show()


#print(q)
  
              #current_rank = 1/(l_freq) + 1/(r_freq)
            
            #l_rank = 1/(l_freq - q[G-1]) - 1/(r_freq + q[G-1])
            #r_rank = 1/(l_freq + q[G]) - 1/(r_freq - q[G])
            
            #l_rank = (r_freq + q[G-1]) - (l_freq - q[G-1])
            #r_rank = (l_freq + q[G]) - (r_freq - q[G])
              
            
            #if current_rank <= l_rank and current_rank <= r_rank:
                #break #local minima found

            #if l_rank < r_rank:
                #G -= 1
            #elif l_rank > r_rank:
                #G += 1