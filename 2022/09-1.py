import copy
import math
textfile = open("9-1.txt","r")
text = textfile.readlines()
commands = list() 
tailpositions = list()
#Uh, my mind just decided on an implementation that would also allow more complex movement I guess

class command:#Commands are in [x][y] coordiante form saved

    def __init__(self,x,y):
        self.x = x
        self.y = y
    def __str__(self):
        return f"(Move: ({self.x})x({self.y})"
class rope:
    def __init__(self,x,y):
        self.x = x
        self.y = y    
    def __str__(self):
        return f"({self.x})x({self.y})"
        
for x in text:
    if(x[0]== 'R'):
        commands.append(command(int(x.split(" ")[1]),0))
    elif(x[0]== 'L'):
        commands.append(command(int(x.split(" ")[1])*-1,0 ))
    elif(x[0]== 'U'):
        commands.append(command(0,int(x.split(" ")[1])))
    elif(x[0]== 'D'):
        commands.append(command(0,int(x.split(" ")[1])*-1))
    else:
        print("Eror reading")

head = rope(0,0)
tail = rope(0,0)

for command in commands:
    #print(command)
    #print("Executing " + str(abs(command.x) + abs(command.y))+" steps")
    for step in range(abs(command.x) + abs(command.y)):
        #Headmovement
        if(command.x != 0):# L/R
            #print("Moving x")
            head.x = head.x +1  if command.x > 0 else head.x -1
        if(command.y != 0):# U/D
            #print("Moving y")
            head.y = head.y +1  if command.y > 0 else head.y -1
        
        #Tailmovement
        if(abs(tail.x - head.x)>1 and t
           ail.y != head.y or tail.x != head.x and abs(tail.y - head.y)>1):#Forcing Diagonal movement if possile   
            tail.x = tail.x +1  if tail.x < head.x else tail.x -1
            tail.y = tail.y +1  if tail.y < head.y else tail.y -1
            #print("Tail moving Diagonally")
        else:
            if(abs(tail.x - head.x)>1):#Moves if difference is larger than 1
                tail.x = tail.x +1  if tail.x < head.x else tail.x -1
                #print("Tail moving on x-axis")
            if(abs(tail.y - head.y)>1):#Moves if difference is larger than 1
                tail.y = tail.y +1  if tail.y < head.y else tail.y -1
                #print("Tail moving on y-axis")
        
        #print(head,end="<-")
        #print(tail)
        #Recordtail
        tailpos = str(tail.x)+"x"+str(tail.y)
        if not(tailpos in tailpositions):
            #print("Added "+str(tailpos))
            tailpositions.append(tailpos)
    #print("\n")

#3034 too low
#for tailpos in tailpositions:
#    print(tailpos)
print(len(tailpositions))        
    
