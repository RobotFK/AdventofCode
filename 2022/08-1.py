import copy
import math
textfile = open("8-1.txt","r")
text = textfile.read()
destructivetext = copy.deepcopy(text) #We will eat this copy as to not harm the original
readpos = 0
# In the Array each outer container is a Row so it will be [Row][Collum / Position]
Forest = [["0" for c in range(99)] for r in range(99)] # 9801 Tres
Forestvisible = [[int(0) for c in range(99)] for r in range(99)] # 1 is visible | 0 is Obscured

r = 0 # Row     ----
c = 0 # Column  ||||
for x in text:

    if (False == x.isdigit()): # End of line
        r=r+1
        c=0
    else:
        Forest [r][c] = x
        c = c+1

for x in range(9801): # 0 To 9800 (99^2)
    r = math.floor(x/99) 
    c = x-(99*math.floor(x/99))
    #print(str(r) + "-"+str(c))
    Obstruction = Forest[r][c]
    if(x <= 98):
        #print("Top Index:" + str(x))
        Forestvisible [r][c] = 1
        while(r < 98):
            r=r+1
            if(Forest[r][c] > Obstruction):
                Forestvisible [r][c] = 1
                #print("Saw " + str(Forest[r][c]) + " behind " +str(Obstruction))
                Obstruction = Forest[r][c]
            elif(Obstruction == 9):
                r=99
                
    elif((x+1)%99 == 1):
        #print("Left Index:" + str(x))
        Forestvisible [r][c] = 1
        while(c < 98):
            c=c+1
            if(Forest[r][c] > Obstruction):
                Forestvisible [r][c] = 1
                #print("Saw " + str(Forest[r][c]) + " behind " +str(Obstruction))
                Obstruction = Forest[r][c]
            elif(Obstruction == 9):
                c=99
                
    elif((x+1)%99 == 0):
        #print("Right Index:"+ str(x))
        Forestvisible [r][c] = 1
        while(c > 0):
            c=c-1
            
            if(Forest[r][c] > Obstruction):
                Forestvisible [r][c] = 1
                #print("Saw " + str(Forest[r][c]) + " behind " +str(Obstruction))
                Obstruction = Forest[r][c]
            elif(Obstruction == 9):
                c=0
                
    elif(x > 9701):
        #print("Bottom Index:" + str(x))
        Forestvisible [r][c] = 1
        while(r > 0):
            r=r-1
            
            if(Forest[r][c] > Obstruction):
                Forestvisible [r][c] = 1
                #print("Saw " + str(Forest[r][c]) + " behind " +str(Obstruction))
                Obstruction = Forest[r][c]
            elif(Obstruction == 9):
                r=0
    else:

        continue

visible = 0
invisible = 0
for x in Forestvisible:
    #print(x)
    for y in x:
        if(y == 1):
            visible=visible+1
        else:
            invisible=invisible+1
        
print("Visible "+str(visible))
#605 to low
print("Invisible "+str(invisible))
