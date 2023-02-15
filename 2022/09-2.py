import copy
import math
textfile = open("9-1.txt","r")
text = textfile.readlines()
commands = list() 
tailpositions = list()
#We can fix this !

class command:#Commands are in [x][y] coordiante form saved

    def __init__(self,x,y,prev=None):
        self.x = x
        self.y = y
        self.prev = prev
    def __str__(self):
        return f"Move: ({self.x})x({self.y})"
class rope:
    def __init__(self,x,y):
        self.x = x
        self.y = y    
    def __str__(self):
        return f"({self.x})x({self.y})"

       
for x in text:
    if(x[0]== 'R'):
        commands.append(command(int(x.split(" ")[1]),0))
        #it seems that int() just eats the linebreaks, huh
    elif(x[0]== 'L'):
        commands.append(command(int(x.split(" ")[1])*-1,0 ))
    elif(x[0]== 'U'):
        commands.append(command(0,int(x.split(" ")[1])))
    elif(x[0]== 'D'):
        commands.append(command(0,int(x.split(" ")[1])*-1))
    else:
        print("Eror reading")

knots = list()
knots.append(rope(0,0)) # This is the Head
for x in range(9): #A linked list baby !
    knots.append(rope(0,0))
    knots[-1].prev = knots[-2]
tail = rope(0,0)

for command in commands:
    #print(command)
    #print("Executing " + str(abs(command.x) + abs(command.y))+" steps")
    for step in range(abs(command.x) + abs(command.y)):
        #Headmovement
        if(command.x != 0):# L/R
            #print("Moving x")
            knots[0].x = knots[0].x +1  if command.x > 0 else knots[0].x -1
        if(command.y != 0):# U/D
            #print("Moving y")
            knots[0].y = knots[0].y +1  if command.y > 0 else knots[0].y -1
        
        #Tailmovements
        pos = 0
        for tail in knots:
            if(pos == 0):
                pos = pos + 1
                continue
            #print("Checking Knot " + str(pos))
            
            if(abs(tail.x - tail.prev.x)>1 and tail.y != tail.prev.y or tail.x != tail.prev.x and abs(tail.y - tail.prev.y)>1):#Forcing Diagonal movement if possile   
                tail.x = tail.x +1  if tail.x < tail.prev.x else tail.x -1
                tail.y = tail.y +1  if tail.y < tail.prev.y else tail.y -1
                #print("Tail moving Diagonally")
            else:
                
                if(abs(tail.x - tail.prev.x)>1):#Moves if difference is larger than 1
                    tail.x = tail.x +1  if tail.x < tail.prev.x else tail.x -1
                    #print("Tail moving on x-axis")
                if(abs(tail.y - tail.prev.y)>1):#Moves if difference is larger than 1
                    tail.y = tail.y +1  if tail.y < tail.prev.y else tail.y -1
                    #print("Tail moving on y-axis")
            pos = pos + 1
        
        #print(knots[0],end="<-")
        #print(knots[-1])
        #Recordtail
        tailpos = str(knots[-1].x)+"x"+str(knots[-1].y)
        if not(tailpos in tailpositions):
            #print("Added "+str(tailpos))
            tailpositions.append(tailpos)
    #print("\n")

#2630
#for tailpos in tailpositions:
#    print(tailpos)
print(len(tailpositions))        
    
