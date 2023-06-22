import math
import copy
with open("19-1.txt","r") as file:
    text = file.readlines()

#Textimport here
minutes = 24
# Type_Attribute = Value


class blueprint:
    def __init__ (self, o_o, c_o, ob_o, ob_c, g_o, g_ob):
        self.o_o = int(o_o)   # Ore Cost to build an Ore Robot
        self.c_o = int(c_o)        # Ore Cost to build an Clay Robot
        self.ob_o = int(ob_o)    # Ore Cost to build an Obisidan Robot
        self.ob_c = int(ob_c)        # Clay Cost to build an Ore Robot
        self.g_o = int(g_o)    # Ore Cost to build an Goede Robot
        self.g_ob = int(g_ob)        # Obsidian Cost to build an Goede Robot
    def all_paths(self, time):
        states = [[[[0,0,0,0],[1,0,0,0]]],]
        #states are filled with the following info: [time][[o,c,ob,g],[o_r,c_r,ob_r,g_r]]
        ob_unlocked = False
        g_unlocked = False
        for t in range(0,time):
            states.append([])
            
            for path in states[t]:
                if ob_unlocked and path[1][1] == 0 : # If a different path has obsidian while this one does not even have clay yet, it gets shut down
                    continue
                if g_unlocked and path[1][2] == 0 : # If a different path has geodes while this one does not even have Obsidian yet, it gets shut down
                    continue
                
                if path[0][0] >= self.g_o and path[0][2] >= self.g_ob: #New Geode Robot
                    newstate =[[
                    path[0][0]+path[1][0]-self.g_o, # o
                    path[0][1]+path[1][1], # c
                    path[0][2]+path[1][2]-self.g_ob, # ob
                    path[0][3]+path[1][3]  # g
                    ],[
                    path[1][0],path[1][1],path[1][2],path[1][3]+1
                    ]]
                    states[t+1].append(newstate)
                    g_unlocked = True
                    continue # If we can build one, this is clearly the best option to do this
                    
                if path[0][0] >= self.ob_o and path[0][1] >= self.ob_c and path[1][2] < self.g_ob : #New Obisidan Robot (Only build of we dont produce too much Obisidan already)
                    newstate =[[
                    path[0][0]+path[1][0]-self.ob_o, # o
                    path[0][1]+path[1][1]-self.ob_c, # c
                    path[0][2]+path[1][2], # ob
                    path[0][3]+path[1][3]  # g
                    ],[
                    path[1][0],path[1][1],path[1][2]+1,path[1][3]
                    ]]
                    #This removes duplicate states to cut down calculation time:
                    #Specifically it checks if a state is worse in all values
                    improvement = True
                    for state in states[t+1]:
                        if all(x <= y for x, y in zip(newstate[0], state[0])) and all(x <= y for x, y in zip(newstate[1], state[1])):improvement = False
                    ob_unlocked = True
                    if improvement : states[t+1].append(newstate)
                    #if path[1][2] == continue
                
                if path[0][0] >= self.c_o and path[1][1] < self.ob_c: #New Clay Robot (Only build of we dont produce too much Clay already)
                    newstate =[[
                    path[0][0]+path[1][0]-self.c_o, # o
                    path[0][1]+path[1][1], # c
                    path[0][2]+path[1][2], # ob
                    path[0][3]+path[1][3]  # g
                    ],[
                    path[1][0],path[1][1]+1,path[1][2],path[1][3]
                    ]]
                    #This removes duplicate states to cut down calculation time:

                    #No need to build an Additional Clayrobot If we have Twice or more of what is needed for an Obsidian Robot
                    if path[0][1] >= 2*self.ob_c: continue
                    #Specifically it checks if a state is worse in all values
                    improvement = True
                    for state in states[t+1]:
                        if all(x <= y for x, y in zip(newstate[0], state[0])) and all(x <= y for x, y in zip(newstate[1], state[1])):improvement = False
                    if improvement : states[t+1].append(newstate)
                    
                if path[0][0] >= self.o_o and path[1][0] < max(self.c_o,self.ob_o,self.g_o): #New Ore Robot (Only build of we dont produce too much Ore already)
                    newstate =[[
                    path[0][0]+path[1][0]-self.o_o, # o
                    path[0][1]+path[1][1], # c
                    path[0][2]+path[1][2], # ob
                    path[0][3]+path[1][3]  # g
                    ],[
                    path[1][0]+1,path[1][1],path[1][2],path[1][3]
                    ]]
                    #This removes duplicate states to cut down calculation time:
                    #Specifically it checks if a state is worse in all values
                    improvement = True
                    for state in states[t+1]:
                        if all(x <= y for x, y in zip(newstate[0], state[0])) and all(x <= y for x, y in zip(newstate[1], state[1])):improvement = False
                    if improvement : states[t+1].append(newstate)

                newstate = [[
                path[0][0]+path[1][0], # o
                path[0][1]+path[1][1], # c
                path[0][2]+path[1][2], # ob
                path[0][3]+path[1][3]  # g
                ],
                path[1]
                ]
                #This removes duplicate states to cut down calculation time:
                #Specifically it checks if a state is worse in all values
                improvement = True
                for state in states[t+1]:
                    if all(x <= y for x, y in zip(newstate[0], state[0])) and all(x <= y for x, y in zip(newstate[1], state[1])):improvement = False
                if improvement : states[t+1].append(newstate)
            print(len(states[-1]))
        ''' Debugging:    
        for minute,paths in enumerate(states):
            #print("Minute:"+str(minute))
            print(len(paths))
            for path in paths:
                #if path[0][3]!= 0 : print(path)
                pass
        '''
        best_geodes = max([path[0][3] for path in states[-1]])
        return best_geodes
            
blueprints = list(blueprint(*[line.split()[i] for i in [6,12,18,21,27,30]]) for line in text)
        
b = blueprint(4,2,3,14,2,7)
#b.max_geodes(11)
'''
b.all_paths(12)
'''

input()

max_geodes = list()
for nr,b in enumerate(blueprints):
    max_geodes.append(b.all_paths(32))
    print("Tested Blueprint Nr: " + str(nr+1))
    if nr == 2:break #Stop afert the Third

mult_g = 1
for i,geodes in enumerate(max_geodes):
    mult_g *= geodes
    #print(i*geodes)
    
print(mult_g)
# 95 Wrong
