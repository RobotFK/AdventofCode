textfile = open("6-1.txt","r")
text = textfile.readline() #one letter less ;)
terminate =10
Pos = 1
marker = list()
#print(type(marker[0]))
for x in text:
    if(len(marker) <= 13):
        marker.append(set(x))
    if(len(marker) != 14):
        Pos = Pos+1
        continue

    Union = set()
    for y in range(len(marker)):
        Union = Union.union(marker[y])
    if(len(Union) == 14):
        print("Found!\nPosition is "+ str(Pos))
        print(Union)
        break
        #Sucsess!
    else:
        marker.pop(0)
        Pos = Pos+1
    
 

