with open("16-1.txt","r") as file:
    text = file.readlines()

#Converts our input into pure data with brutal efficiency
#Carefull, the strip only works because there are no caves named RA AT or TE ;)
#Tuning thanks to chatgpt of course

#Datastructures:
#caves  ((Name,Flow),(Conn,Conn2,...)
#valves [Name,Flow,{(Conn,Dist),(Conn2,Dist2),...}]

caves = tuple(
    (
        tuple((cave.split()[1].strip("rate=;"),int(cave.split()[4].strip("rate=;")))), 
        tuple(c.strip(",") for c in cave.split()[9:]) 
    ) for cave in text)

#I only include valves here that have a non-zero positive integer as onyl these are relevant right now
valves = [list(cave[0][:2]) for cave in caves if int(cave[0][1]) != 0]
valves.append(['AA',0])#Due to an oversight the Pathing is not modular and as such we just force AA into it as a fake entry
for valve in valves:
    print(valve)
    
#Using a Decision tree to record the traveltime to any other valve
for valve in valves:
    valve.append(set()) #Add a Subset that will contain the Name and Traveltime of all other Valves
    #A* algoirthem time again:
    visited_caves = {valve[0],}
    edge_caves = next(set(cave[1]) for cave in caves if cave[0][0] == valve[0]) #Caves to map in the current A* cycle
    f_edge_caves = set () #Caves to map in the next A* cycle (Needed to iterate/update without manipulating the iterated lsit)
    dist = 1
    while len(valve[2]) < (len(valves)-1) and dist < len(caves): #Safeguard against being stuck in a loop

        for edge_cave in edge_caves:
            #Test if Edge cave is not yet visited
            if edge_cave not in visited_caves and edge_cave in {item[0] for item in valves}:
                valve[2].add(tuple((edge_cave, dist)))
            connectedcave = next(cave[1] for cave in caves if cave[0][0] == edge_cave)
            f_edge_caves.update(item for item in connectedcave if item not in visited_caves)
            visited_caves.add(edge_cave)
        edge_caves = f_edge_caves
        f_edge_caves = set()
        dist += 1
    #print(valve)
    #break

# Having done all that optimising all remaining tasks can just be forced by iterationg all options (safe upper bound of O(n) is >=24! but estimations place it much lower)
print("Starting Path Calc")


ttl = 11 #Maximum TimeToLive acting as a safeguard against being stuck in a loop
time = 26 # Amount we have
paths = set() #Lists all paths
ppaths = set() #Lists all possible paths
ppaths.add((time,'AA'))
growth = True
growthamount = 0
while(growth):
    growth = False #Records if any new Paths were found
    f_paths = set()#List of all possible paths after updating
    for path in ppaths:
        p_growth = False #Records if this Path found a new addition
        vertex = next(valve for valve in valves if valve[0] == path[-1]) # Vertex contains our current position
        for f_vertex in vertex[2]: 
            remaining_time = path[0] - (1+f_vertex[1]) # We subtract the time it takes to move to the valve + The minute to open
            if f_vertex[0] not in path and remaining_time >= 0:
                growth = p_growth = True
                growthamount += 1
                #We have found a new vertex to move to
                f_paths.add(tuple((remaining_time, *path[1:] ,f_vertex[0]))) #Deduction is left up to the reader, I can only say that ChatGPT offerd the star as an unpacking operator
        if not p_growth:
            paths.add(path)
    #After having gone through every path    
    if (len(f_paths) != 0):
        ppaths = f_paths
    print("Open|Closed Paths: " + str(len(ppaths))+" | "+ str(len(paths)))
    ttl -= 1
    if ttl == 0:
        break
    
#paths.update(ppaths)

print("Starting Pressure Calc")
pressure = list()
for path in paths : # Adding the Pressurervalue at index 0, moving all others 1 futher
    pressure_release = 0
    remaining_time = 26
    prev_valve = str() #Simply a Memory of the prev vertex
    for vertex in path:
        #print(vertex)
        if type(vertex) == int or vertex == 'AA': # Skip trivial ones
            prev_valve = vertex 
            continue
        valve = next(valve for valve in valves if valve[0] == vertex) #Get all data from the current valve
        traveltime = next(tunnel[1] for tunnel in valve[2] if tunnel[0] == prev_valve) #Match the time it took to go from the current valve to the previous (traveltime SHOULD be bidirektional)
        remaining_time -= (traveltime+1) #Subtract the time it took to reach this vertex and to turn on the valve
        flow = next(valve[1] for valve in valves if valve[0] == vertex) # How much flows through the current vertex(current valve)
        pressure_release += int(flow)*remaining_time #Add the total flow this valve will release to the sum pressure_release
        prev_valve = vertex
    pressure.append((pressure_release ,path[2:]))
    
print("Pressures recorded:")
print(len(pressure))

'''
input()
print("Starting ElepthantPath Calc")
ttl = 11 #Maximum TimeToLive acting as a safeguard against being stuck in a loop
time = 26 # Amount we have
paths = set() #Lists all paths
ppaths = set() #Lists all possible paths
ppaths.add((time,'AA'))
growth = True
growthamount = 0
while(growth):
    growth = False #Records if any new Paths were found
    f_paths = set()#List of all possible paths after updating
    for path in ppaths:
        p_growth = False #Records if this Path found a new addition
        vertex = next(valve for valve in valves if valve[0] == path[-1]) # Vertex contains our current position
        for f_vertex in vertex[2]: 
            remaining_time = path[0] - (1+f_vertex[1]) # We subtract the time it takes to move to the valve + The minute to open
            if f_vertex[0] not in path and remaining_time >= 0:
                growth = p_growth = True
                growthamount += 1
                #We have found a new vertex to move to
                f_paths.add(tuple((remaining_time, *path[1:] ,f_vertex[0]))) #Deduction is left up to the reader, I can only say that ChatGPT offerd the star as an unpacking operator
        if not p_growth:
            paths.add(path)
    #After having gone through every path    
    if (len(f_paths) != 0):
        ppaths = f_paths
    print("Open|Closed Paths: " + str(len(ppaths))+" | "+ str(len(paths)))
    ttl -= 1
    if ttl == 0:
        break
print("Starting ElepthantPressure Calc")
'''

print("Top single:")
pressure.sort(key=lambda item: item[0])
print(pressure[-1])

minpartner = 0
p = pressure[-1]
for j in pressure:
    if(p == j):
        continue
    pset = set(p[1])
    jset = set(j[1])
    if len(pset.intersection(jset)) == 0:
        #print("Pre intersect")
        if j[0] > minpartner:
            minpartner = j[0]

print("Minimum Partnervalue:")
print(minpartner)
minpair = minpartner + pressure[-1][0]
#Brute force search to ensure finding the best one
pressurepair = list()
for p in pressure:
    if p[0] < minpartner:
        continue
    for j in pressure[pressure.index(p)+1:]:
        if j[0] < minpartner or (p == j):
            continue
        pset = set(p[1])
        jset = set(j[1])
        if len(pset.intersection(jset)) == 0:
            pair = (p[0]+j[0],p,j)
            pressurepair.append(pair)

print("Calculations done, Displaying:")
      
pressurepair.sort(key=lambda item: item[0])
print(pressurepair[-1][1:])
print(pressurepair[-1][0])

