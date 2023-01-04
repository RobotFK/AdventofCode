textfile = open("4-1.txt","r")
text = textfile.readlines()
Sum = 0
for x in text:
    x= ''.join(x.splitlines()) # Removes the linebreak
    Group = x.split(",")
    for Elf in range(2):
     Group[Elf] = Group[Elf].split("-")
     Group[Elf][0] =int(Group[Elf][0])
     Group[Elf][1] =int(Group[Elf][1])
    #print(Group)
    if (Group[0][0] >= Group[1][0] and Group[0][1]<= Group[1][1]): #Elf1 has less
        Sum = Sum+1
        #print(x)
        #print("1 Less")
    elif(Group[0][0] <= Group[1][0] and Group[0][1]>= Group[1][1]): #Elf2 has less
        Sum = Sum+1
        #print(x)
        #print("2 Less")
print(Sum)
