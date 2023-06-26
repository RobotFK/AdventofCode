with open("21-1.txt","r") as file:
    text = file.readlines()

monkeys = [line.split() for line in text]

for i,monkey in enumerate(monkeys):
    monkeys[i][0] = monkey[0].strip(":")
    if len(monkey) == 2: monkeys[i] = [monkey[0],int(monkey[1])]
    if monkey[0] == 'humn':
        monkeys[i] = [monkey[0],None]

numbersfound = 1#First loop info is now off by one
while not numbersfound == 0:
    numbersfound = 0
    for i,monkey in enumerate(monkeys):
        if len(monkey) == 2: continue
        if monkey[0] == 'root' or monkey[0] == 'humn':
            continue 
        sign = monkey[2]
        input1 = next((imonkey[1] for imonkey in monkeys if len(imonkey)==2 and imonkey[0]== monkey[1]), None)
        input2 = next((imonkey[1] for imonkey in monkeys if len(imonkey)==2 and imonkey[0]== monkey[3]), None)
        if input1 == None or input2 == None:continue
        if sign == '+':monkeys[i]= [monkey[0],(input1+input2)]
        elif sign == '-':monkeys[i]= [monkey[0],(input1-input2)]
        elif sign == '*':monkeys[i]= [monkey[0],(input1*input2)]
        elif sign == '/':monkeys[i]= [monkey[0],(int(input1/input2))]
        numbersfound += 1
    print(str(numbersfound)+ " new found")

print("Starting Reverse solve:")    
#By now there is a only a single (or a number) of direct connections left between root and humn
    
#New list of Relevant monkeys
eq_monkeys = list() #[index][id,input1,sign,input2,result]
for monkey in monkeys:
    if monkey[0] == 'humn':eq_monkeys.append([monkey[0],0,0,0]) #Proxy value to reach len 4 to catch it later
    if len(monkey) == 2 : continue
    eq_monkeys.append(monkey)
    #print(eq_monkeys[-1])

#Set Root as Seed (only works if root's sign is '+')
for i,monkey in enumerate(eq_monkeys):
    if monkey[0] != 'root':continue
    eq_monkeys[i] = monkey + [0]
    

numbers_solved = 1#First loop info is now off by one
while not numbers_solved == 0:
    numbers_solved  = 0
    for monkey in eq_monkeys:
        if len(monkey) != 5 : continue #Only derive Requirements where solutions have been found
        #print("Found len 5")
        #print(monkey)
        if monkey[0] == 'humn':
            print(monkey)
            break
        sign = monkey[2]
        input1 = next((imonkey[1] for imonkey in monkeys if len(imonkey)==2 and imonkey[0]== monkey[1]), None)
        input2 = next((imonkey[1] for imonkey in monkeys if len(imonkey)==2 and imonkey[0]== monkey[3]), None)
        
        #This segment stops calculations for Monkeys where their inputs have already been given a solution
        if input1 == None:
            input1_m = next((imonkey for imonkey in eq_monkeys if imonkey[0]== monkey[1]))
            if len(input1_m) == 5: continue
        if input2 == None:
            input2_m = next((imonkey for imonkey in eq_monkeys if imonkey[0]== monkey[3]))
            if len(input2_m) == 5: continue
        #print("Passed redundancy check")
        output = monkey[4]
        if monkey[0] == 'root':
            if input1 == None:
                solution = input2
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[1] else imonkey for imonkey in eq_monkeys]
            elif input2 == None :
                solution = input1
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[3] else imonkey for imonkey in eq_monkeys]
        elif sign == '+':
            if input1 == None:#The First Input now needs to be found and filled in
                solution = output - input2
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[1] else imonkey for imonkey in eq_monkeys]
            elif input2 == None :
                solution = output - input1
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[3] else imonkey for imonkey in eq_monkeys]
        elif sign == '-':
            if input1 == None:
                solution = output + input2
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[1] else imonkey for imonkey in eq_monkeys]
            elif input2 == None :
                solution = input1 - output
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[3] else imonkey for imonkey in eq_monkeys]
        elif sign == '*':
            if input1 == None:
                solution = output / input2
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[1] else imonkey for imonkey in eq_monkeys]
            elif input2 == None :
                solution = output / input1
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[3] else imonkey for imonkey in eq_monkeys]
        elif sign == '/':
            if input1 == None:
                solution = input2 * output
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[1] else imonkey for imonkey in eq_monkeys]
            elif input2 == None :
                solution = input1 / output
                eq_monkeys = [imonkey + [solution] if imonkey[0] == monkey[3] else imonkey for imonkey in eq_monkeys]
        numbers_solved+= 1
    print(str(numbers_solved)+ " new Solved") 

print("Done")
        
