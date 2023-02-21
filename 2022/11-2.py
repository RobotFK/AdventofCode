#No need to read the input this time, I can punch in 7 Monkeys by hand
import math

class number:
    def __init__(self,base):
        # Any Natural Number can be modeld as = k*df+rem
        self.rem = base #Remainder of number %df
        self.df = 9699690 # DivisonFactor as in a product of all divisors from all Monkeys
        self.k = 0
    def __str__(self):
        return f"Number is {self.k} * {self.df} + {self.rem}"
    def update(self):
        if(self.rem >= self.df):
            self.k = self.k + math.floor(self.rem / self.df)
            self.rem = self.rem % self.df 
    def add(self,add):
         self.rem = self.rem + add
         self.update()
    def mult(self,mult):
         self.rem = self.rem * mult
         self.update()
    def square(self):
         self.rem = self.rem * self.rem
         self.k = 0 # Unproven, but needed to make calculations even possible
         self.update()

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
        return f"Monkey {self.id}:" + str(len(self.inv)) + " items"
    def inspect (self):
        self.activity = self.activity + len(self.inv)      
    def operation (self):
        #self.inv = [((x+self.add)*self.mult)**self.powr for x in self.inv]
        #This is what is performance heavy, so we will need to apply some optimisaiton
        for x in self.inv:
            if (self.mult != 1):
                x.mult(self.mult)
            elif(self.add != 0):
                x.add(self.add)
            elif(self.powr != 1):
                x.square()   
    def bored (self):
        #print(self.inv)
        #print((9699690*self.div))
        #self.inv = [(x/9699690) for x in self.inv if(x >= (9699690*self.div))] #Dunno why this breaks, but lets do it slow then:
        #print(self.inv)
        pass
    def throw (self):
        #print("Monkey" + str(self.id))
        for x in self.inv:
            #print("x is: "+ str(x))
            if(not(x.rem%self.div)):
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
    3,0,[number(54),number(53)],2,2,6))
monkeys[1]=(monkey(1,
    11,0,[number(95),number(88),number(75),number(81),number(91),number(67),number(65),number(84)],7,3,4))

monkeys[2]=(monkey(2,
    1,6,[number(76),number(81),number(50),number(93),number(96),number(81),number(83)],3,5,1))

monkeys[3]=(monkey(3,
    1,4,[number(83),number(85),number(85),number(63)],11,7,4))

monkeys[4]=(monkey(4,
    1,8,[number(85),number(52),number(64)],17,0,7))

monkeys[5]=(monkey(5,
    1,2,[number(57)],5,1,3))

monkeys[6]=(monkey(6,
    1,0,[number(60),number(95),number(76),number(66),number(91)],13,2,5,2))

monkeys[7]=(monkey(7,
    1,5,[number(65),number(84),number(76),number(72),number(79),number(65)],19,6,0))

for m in monkeys:
        print(m)
print("")
  
for x in range(10000):
    for m in monkeys:
        #print(m)
        m.inspect()
        m.operation()
        m.bored()
        m.throw()
        
monkeybuisness = list()
for m in monkeys:
   monkeybuisness.append(m.activity)
monkeybuisness.sort(reverse=True)
print(monkeybuisness)
print(monkeybuisness[0]*monkeybuisness[1])

        
