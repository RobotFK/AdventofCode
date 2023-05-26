with open("15-1.txt","r") as file:
    text = file.readlines()

#Converts our input into pure data with brutal efficiency 
sensors = [[int(sensor.split()[word][2:].strip(",:")) for word in (2,3,-2,-1)]for sensor in text]
#sensors now contains each sensor as an entry
#with the 4 values being Scanner X, Scanner Y, Beacon X and Beacon Y

#A lot of work can be done here, and for the second part we just bruteforce it

covered_ranges = set() #As a Tool of agravation we compile an Array of noncolliding ranges the sensors cover

def append_covered_ranges(lower, upper):
    if not covered_ranges: #Fires if empty
        covered_ranges.add((lower,upper))
        return
    else:
        for range_old in covered_ranges:
            if  range_old[1] >= upper and range_old[0] <= lower: #New range is fully covered by the old range
                #print("Contained")
                return
            elif range_old[1] >= lower and range_old[0] <= lower: #New Lower bounds is within this the old range
                covered_ranges.remove((range_old[0],range_old[1]))
                #print("reinserting")
                append_covered_ranges(range_old[0], upper) #Next we reinsert the stiched together range(using the same function)
                return
            elif range_old[1] >= upper and range_old[0] <= upper: #New Upper bounds is within this the old range
                covered_ranges.remove((range_old[0],range_old[1]))
                #print("reinserting")
                append_covered_ranges(lower, range_old[1]) #Next we reinsert the stiched together range(using the same function)
                return
            elif range_old[1] <= upper and range_old[0] >= lower: #New range fullly covers the old range
                covered_ranges.remove((range_old[0],range_old[1]))
                #print("Replacing")
                append_covered_ranges(lower, upper) #Retry insertion without the obsolete range
                return
        #print("Adding as new entry")
        covered_ranges.add((lower,upper)) #Range does not match any existing records, adding it as it's own entry
    return

scanline = 0#Line we are currently testing(Y) going up to max 4m
lastping = 1#
while scanline != -1:
    covered_ranges.clear()
    for sensor in sensors:
        dist_to_scanline = abs(sensor[1]-scanline)
        #print("Y offset " + str(dist_to_scanline))
        sensor_range = abs(sensor[0]-sensor[2])+ abs(sensor[1]-sensor[3]) #This is how many units (manhattan dist) are to move from scanner to beacon
        #print("Sensorrange " + str(sensor_range))
        scanline_range = sensor_range - dist_to_scanline if sensor_range - dist_to_scanline > 0 else 0
        if scanline_range:
            #print("Centerd at X=" + str(sensor[0]))
            #print("Covering " + str(scanline_range))
            append_covered_ranges(sensor[0]-scanline_range, sensor[0]+scanline_range)
            #print(covered_ranges)
            #print("\n")
    
    if scanline % lastping == 0:#To stop the Person watching the Output from loosing it
        print(scanline)
        print(covered_ranges)
        lastping *= 2
    if len(covered_ranges) != 1:#If there are multiple ranges there has to be a Hole in between them
        print(covered_ranges)
        print(scanline)
        keystroke = input("Entry found") #To stop the process
    else:
        scanline +=1

