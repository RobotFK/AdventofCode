textfile = open("13-1.txt","r")
text = textfile.readlines()

packets = list()

for x in text:
    if(x == "\n"): #Line is empty
        continue
    ls = list(eval(x))#Open security risk could be solved by ensuring that the string only contains numbers and brackets
    ls = eval(x)
    packets.append(ls)

print("Found "+str(int(len(packets)/2))+" Pairs")

def compare_lists (listl,listr) -> int:    
    #Compares if the lists are in Order
    #Sadly we need Trinary Results,as sublists can be either in/out of Order or be unsure
    #So for this we use the returns:
    # 0 = False (Packet has failed)
    # 1 = Unknown (Weak True) (Packet needs further checking)
    # 2 = True (Packet is in Order)

    #print("Comparing:")
    #print(listl)
    #print(listr)
    if len(listl) == 0 and len(listr) == 0:#Fixing Edgecases with this preamble
        #print("Both lists empty")
        return 1
    elif len(listl) == 0:
        #print("Left empty")
        return 2
    for i in range(len(listl)):
        '''
        print("\nI="+str(i))
        print(listl[i])
        try:
            print(listr[i])
        except:
            print("Error")
            print(listr)
        '''
        if(i > len(listr)-1): # Triggers when Right has run out
            #print("Return False,as listr onyl contains "+str(len(listr)))
            return 0
        if(isinstance(listl[i], int) and isinstance(listr[i], int)):# int x int
            #print("i i")
            if(listl[i] > listr[i]):
                #print("Failed")
                return 0
            elif(listl[i] < listr[i]):
                #print("Sucsess")
                return 2
        elif(isinstance(listl[i], list) and isinstance(listr[i], list)): # list x list
            #print("l l")
            sublistresult = compare_lists(listl[i],listr[i])
            if(sublistresult != 1):
                return sublistresult
        elif(isinstance(listl[i], list) and isinstance(listr[i], int)): # list x int
            #print("l i")
            sublistresult = compare_lists(listl[i],list([listr[i]]))
            if(sublistresult != 1):
                return sublistresult
        elif(isinstance(listl[i], int) and isinstance(listr[i], list)): # int x list
            #print("i l")
            sublistresult = compare_lists(list([listl[i]]),listr[i])
            if(sublistresult != 1):
                return sublistresult
        else:
            print("ERROR")
    if len(listl) < len(listr): #Having exhausted all of the left list, if there are more entries on the right we must be in order
        #print("Left ran out first")
        return 2
    return 1

S_packets = list()
S_packets.append([[2]])
S_packets.append([[6]])
for i in range(len(packets)):
    print("Comparing Next, Old index: ",end="")
    print(i)
    if (compare_lists(packets[i],S_packets[0])):#Check if Packet goes first
        S_packets.insert(0,packets[i])
        print("Inserted at pos 0")
        continue
    for j in range(len(S_packets)):
        #print("Comp i:"+str(i)+" to j:"+str(j))
        #print(compare_lists(S_packets[j],packets[i]))
        if (compare_lists(packets[i],S_packets[j])):#Packet goes after the evaluated one
            #If the already placed Package goes before the one in the Process
            S_packets.insert(j,packets[i])
            print("inserted at "+str(j))
            break

for i in range(len(S_packets)):
    if(S_packets[i] == [[2]]):
        print("Found 2:",end=" ")
        print(str(i+1))
    if(S_packets[i] == [[6]]):
        print("Found 6:",end=" ")
        print(str(i+1))

#1812 Too low
