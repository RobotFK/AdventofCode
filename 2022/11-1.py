#No need to read the input this time, I can punch in 7 Monkeys by hand

import math
class monkey:#Yeah baby, more OOP in Python (It feels like a small crime)
    def __init__(self,
                 identification,
                 mult,  #Default is 1
                 add,   #Default is 0
                 inventory,
                 divisor,
                 true,
                 false,
                 exponent=1):
        self.id = identification
        self.mult = mult
        self.add = add
        self.inv = inventory
        self.div = divisor
        self.true = true #Yeah, prone to errors by being dependent on the Preexisting lists index, I know
        self.false = false
        self.powr = exponent
        self.activity = 0
    def __str__(self):
        return f"Monkey {self.id}: {self.inv}"
    def inspect (self):
        self.activity = self.activity + len(self.inv)      
    def operation (self):
        self.inv = [((x+self.add)*self.mult)**self.powr for x in self.inv]
        #Looks strange but all execpt one factor are always set to something that doesnt Change x
        #Also List comprehension feels fucking great
    def bored (self):
        self.inv = [math.floor(x/3) for x in self.inv]            
    def throw (self):
        #print("Monkey" + str(self.id))
        for x in self.inv:
            #print("x is: "+ str(x))
            if(not(x%self.div)):
                #Throw to true monkey
                #print("\n"+str(self.id) + "- "+str(x)+" ->" +str(self.true) )

                monkeys[self.true].inv.append(x)
            else:
                #Throw to False monkey
                #print("\n"+str(self.id) + "- "+str(x)+" ->" +str(self.false) )
                monkeys[self.false].inv.append(x)
        self.inv = list()#Restoration of items into discrete states ;)

monkeys = [[] for i in range(8)]
monkeys[0]=(monkey(0,
    3,0,[54,53],2,2,6))
monkeys[1]=(monkey(1,
    11,0,[95,88,75,81,91,67,65,84],7,3,4))

monkeys[2]=(monkey(2,
    1,6,[76,81,50,93,96,81,83],3,5,1))

monkeys[3]=(monkey(3,
    1,4,[83,85,85,63],11,7,4))

monkeys[4]=(monkey(4,
    1,8,[85,52,64],17,0,7))

monkeys[5]=(monkey(5,
    1,2,[57],5,1,3))

monkeys[6]=(monkey(6,
    1,0,[60,95,76,66,91],13,2,5,2))

monkeys[7]=(monkey(7,
    1,5,[65,84,76,72,79,65],19,6,0))

for m in monkeys:
        print(m)
print("")
  
for x in range(20):
    for m in monkeys:
        #print(m)
        m.inspect()
        m.operation()
        m.bored()
        m.throw()
    #for m in monkeys:
    #    print(m)    
    #print("")
monkeybuisness = list()
for m in monkeys:
   monkeybuisness.append(m.activity)
monkeybuisness.sort(reverse=True)
print(monkeybuisness)
print(monkeybuisness[0]*monkeybuisness[1])

        
