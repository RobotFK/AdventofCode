import math
with open("20-1.txt","r") as file:
    text = file.readlines()
    
decryption_key = 811589153
mix_amount = 10
file = [(i,int(line.strip("\n"))*decryption_key) for i,line in enumerate(text)]
print(file)
#This aporach is O(n)= n
for mix in range(mix_amount):
    for iindex in range(len(file)):
        #print("Move: "+str(iindex+1))
        current_index = next(i for i,f in enumerate(file) if f[0] == iindex)
        #print("Located initally at "+str(current_index))
        value = file[current_index][1]
        item = file[current_index]
        #if value == 4:input()
        if value ==  0:continue
        del file[current_index]
        new_index = (current_index+value)% len(file)     
        file.insert(new_index,item)
        #print(*(map(lambda v: v[1], file)))
    print("Mix "+str(mix+1)+" Done")

print(file)

Zeroindex = next(i for i,f in enumerate(file) if f[1] == 0)
grove_coordinates = file[(Zeroindex+1000)%len(file)][1]+file[(Zeroindex+2000)%len(file)][1]+file[(Zeroindex+3000)%len(file)][1]
print("grove_coordinates: "+str(grove_coordinates))
