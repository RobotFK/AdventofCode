textfile = open("10-1.txt","r")
text = textfile.readlines()
# This has a couple usefull aproaches, but i decide on:
# Simulating a machine with running a loop that executes the next input or waits or Outputs
# This should reduce the techdebt :)

#But to save some overhead I will destroy var text in the process

cycle = 0 #Current Cylce
reg_x = 1 #Registry X value
state = str() # The state the machine is currently in
sum_signal = 0
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

    if ((cycle+20)%40 == 0):
        strength = cycle * reg_x
        print("Sending:")
        print("Cycle \t" + str(cycle))
        print("reg-x \t" + str(reg_x))
        print("Adding \t" + str(strength),end="\n\n")
        sum_signal = sum_signal + strength
        if (cycle == 220):
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
    
print(sum_signal)
