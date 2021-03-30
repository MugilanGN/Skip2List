from simanneal import Annealer
import random
import matplotlib.pyplot as plt
import numpy as np

def normal(x,mu,sigma):
    return ( 2.*np.pi*sigma**2. )**-.5 * np.exp( -.5 * (x-mu)**2. / sigma**2. )

def random_level(p):
    count = 0
    rand = 1
    
    while rand != 0:
        rand = random.randint(0,p-1)
        count += 1
        
    return count

class GuardOptimizer(Annealer):
    
    def __init__(self, q, state, m):

        self.steps = 10000
        
        self.q = q
        self.m = m
        self.n = len(q)
        
        super(GuardOptimizer, self).__init__(state)
        
    def energy(self):
        
        e = 0
        
        for i in range(1, self.m+1):
            
            pos = self.state[i]
            total_frequency = 0
            
            #we could possibly use a fenwick tree to compute this
            
            for j in range(self.state[i], self.state[i+1]):
                total_frequency += self.q[j]
            
            component = total_frequency * pos
            
            e += component
            
        return -e
    
    def move(self):
        
        region = 0    
        
        while region <= 1:
            node = random.randint(1, m)
            region = self.state[node+1] - self.state[node-1] - 1

        lower_bound = self.state[node-1] - self.state[node] + 1
        upper_bound = self.state[node+1] - self.state[node] - 1

        pos_change = random.randint(lower_bound, upper_bound)

        self.state[node] += pos_change
        
n = 100000
m = n//250

intial_positions = [i*(n//(m+1)) for i in range(0, m+2)]

#query_data = [normal(i, n//2, n//8) * 10**4 for i in range(0, n)]

#print(query_data)

query_data = [i**2 for i in range(0, n)]
               
plt.plot([i for i in range(0,n)],query_data)


plt.figure()


ahl = GuardOptimizer(query_data, intial_positions, m)

state, e = ahl.anneal()

print("\n")
print(state)

spacing_list = []

for i in range(1, len(state)):
    spacing_list.append(state[i] - state[i-1])
    
print(spacing_list)

plt.plot([i for i in range(0,len(state)-1)],spacing_list)
plt.show()
