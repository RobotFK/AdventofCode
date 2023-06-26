with open("21-1.txt","r") as file:
    text = file.readlines()

monkeys = [line.split() for line in text]

for i,monkey in enumerate(monkeys):
    monkeys[i][0] = monkey[0].strip(":")
    if len(monkey) == 2: monkeys[i] = [monkey[0],int(monkey[1])]

numbersfound = 0
root_know = False
while not root_know:
    for i,monkey in enumerate(monkeys):
        if len(monkey) == 2: continue
        sign = monkey[2]
        input1 = next((imonkey[1] for imonkey in monkeys if len(imonkey)==2 and imonkey[0]== monkey[1]), None)
        input2 = next((imonkey[1] for imonkey in monkeys if len(imonkey)==2 and imonkey[0]== monkey[3]), None)
        if input1 == None or input2 == None:continue
        if sign == '+':monkeys[i]= [monkey[0],(input1+input2)]
        if sign == '-':monkeys[i]= [monkey[0],(input1-input2)]
        if sign == '*':monkeys[i]= [monkey[0],(input1*input2)]
        if sign == '/':monkeys[i]= [monkey[0],(input1/input2)]
        numbersfound += 1
    print(str(numbersfound)+ " new found")
    numbersfound = 0
    if next((len(imonkey) for imonkey in monkeys if imonkey[0]== 'root'), -1) == 2:
        root_know = True

root = next((imonkey for imonkey in monkeys if imonkey[0]== 'root'), -1)
print(int(root[1]))
