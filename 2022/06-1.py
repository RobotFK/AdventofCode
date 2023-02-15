textfile = open("6-1.txt","r")
text = textfile.readline() #one letter less ;)
terminate =10
Pos = 1
marker = list()
#print(type(marker[0]))
for x in text:
    if(len(marker) <= 3):
        marker.append(set(x))
    if(len(marker) <= 3):
        Pos = Pos+1
        continue

    mark12 = marker[0].union(marker[1])
    mark34 = marker[2].union(marker[3])
    mark1234 = mark12.union(mark34)
    if(len(mark1234) == 4):
        print("Found!\nPosition is "+ str(Pos))
        print(mark1234)
        break
        #Sucsess!
    else:
        marker.pop(0)
        Pos = Pos+1
    
 

