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

T_Packets = set()
F_Packets = set()
Pairs = int(len(packets)/2)
for i in range(Pairs):
    #print(i)
    if(compare_lists(packets[i*2],packets[i*2+1])):
        #print("True \t" +str(i+1))
        T_Packets.add(i+1)
    else:
        #print("False\t" +str(i+1))
        F_Packets.add(i+1)

Sum = int()
for x in T_Packets: 
    Sum += x

print(Sum)
#923 Too low
#5340 Too low
#6204 Wrong
#6288 Wrong

