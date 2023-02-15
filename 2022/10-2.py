import math
textfile = open("10-1.txt","r")
text = textfile.readlines()
# This has a couple usefull aproaches, but i decide on:
# Simulating a machine with running a loop that executes the next input or waits or Outputs
# This should reduce the techdebt :)

#But to save some overhead I will destroy var text in the process

cycle = 0 #Current Cylce
reg_x = 1 #Registry X value
state = str() # The state the machine is currently in
led_on = "█" #Makes Configuration easier
led_off = " "
crt = [[led_off for c in range(40)] for r in range(6)]

'''
for x in crt:
    for y in x:
        print(y,end="")
    print("\n")
'''
print("\n\n")

while(text):
    #Pre Cycle
    cycle = cycle +1 #After this Command Cycel is accurate
    if not state: #Import next Command
        state = text[0]
        text.pop(0)
        if (state == "noop\n"): # Clear the state (and idle) if Noop is the active command
            state = None
        working = True # As all command have to wait once; we do that here
        
    #During Cycle
    #print(cycle)
    line = math.ceil(cycle/40) -1
    #print(str(line) + " - " + str(cycle))
    #print(abs(reg_x - cycle%40))
    if (abs(reg_x - (cycle%40-1)) <=1): # So many of by one corrections -_-
        crt[line][cycle%40-1] = led_on
        #crt[line][cycle%40-1] = line
    if (cycle == 241):
            break
        
    #End of Cycle
    if not working: # We enter here if we spent 2 Full cycels doing what we love (Calculating)
        if (state.split(" ")[0] == "addx"): # Technically redundant because out instructions are so limited but allow for a clear flow
            #print("Cycle \t" + str(cycle))
            #print("Adding \t"+ state.split(" ")[1])
            reg_x = reg_x + int(state.split(" ")[1])
            state = None
    else:
        #print("Cycle \t" + str(cycle) + " spent working")
        working = False

print("Output:")   
for x in crt:
    for y in x:
        print(y,end="")
    print("\n",end="")
