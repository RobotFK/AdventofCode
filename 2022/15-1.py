with open("15-1.txt","r") as file:
    text = file.readlines()

#Converts our input into pure data with brutal efficiency 
sensors = [[int(sensor.split()[word][2:].strip(",:")) for word in (2,3,-2,-1)]for sensor in text]
#sensors now contains each sensor as an entry
#with the 4 values being Scanner X, Scanner Y, Beacon X and Beacon Y

#A lot of work can be done here, but part one requires just a tiny fracture of the possible knowledge 

Y2M = 0 #Records the amount of Scanned and empty Fields in Y = 2 000 000

covered_ranges = set() #As a Tool of agravation we compile an Array of noncolliding ranges the sensors cover

def append_covered_ranges(lower, upper):
    if not covered_ranges: #Fires if empty
        covered_ranges.add((lower,upper))
        return
    else:
        for range_old in covered_ranges:
            if  range_old[1] >= upper and range_old[0] <= lower: #New range is fully covered by the old range
                print("Contained")
                return
            elif range_old[1] >= lower and range_old[0] <= lower: #New Lower bounds is within this the old range
                covered_ranges.remove((range_old[0],range_old[1]))
                print("reinserting")
                append_covered_ranges(range_old[0], upper) #Next we reinsert the stiched together range(using the same function)
                return
            elif range_old[1] >= upper and range_old[0] <= upper: #New Upper bounds is within this the old range
                covered_ranges.remove((range_old[0],range_old[1]))
                print("reinserting")
                append_covered_ranges(lower, range_old[1]) #Next we reinsert the stiched together range(using the same function)
                return
            elif range_old[1] <= upper and range_old[0] >= lower: #New range fullly covers the old range
                covered_ranges.remove((range_old[0],range_old[1]))
                print("Replacing")
                append_covered_ranges(lower, upper) #Retry insertion without the obsolete range
                return
        print("Adding as new entry")
        covered_ranges.add((lower,upper)) #Range does not match any existing records, adding it as it's own entry
    return

for sensor in sensors:
    dist_to_2m = abs(sensor[1]-2000000)
    #print("Y offset " + str(dist_to_2m))
    sensor_range = abs(sensor[0]-sensor[2])+abs(sensor[1]-sensor[3]) #This is how many units (manhattan dist) are to move from scanner to beacon
    #print("Sensorrange " + str(sensor_range))
    Y2M_range = sensor_range - dist_to_2m if sensor_range - dist_to_2m > 0 else 0
    if Y2M_range:
        print("Centerd at X=" + str(sensor[0]))
        print("Covering " + str(Y2M_range))
        append_covered_ranges(sensor[0]-Y2M_range, sensor[0]+Y2M_range)
        print(covered_ranges)
        print("\n")
        
for ranges in covered_ranges:
    Y2M += ranges[1]-ranges[0]
print(Y2M)
#2020830 too low
#3224696 too low
#4920846 too high
