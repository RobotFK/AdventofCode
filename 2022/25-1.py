with open("25-1.txt","r") as file:
    text = file.readlines()

#digits are 2, 1, 0, minus or double minus

conversion = {
    '2' : 2, 
    '1' : 1,
    '0' : 0,  
    '-' : -1,
    '=' : -2,  
    }
rconversion = {
    0  : '0',
    1  : '1',
    2  : '2',
    3 : '=', 
    4 : '-', 
    }
tanks = list()

def snafu_to_dec(snafu):
    value = 0
    for i,digit in enumerate(reversed(snafu)):
        #print(str(pow(5,i))+" * "+str(conversion[digit]))
        value += pow(5,i)*conversion[digit]
    return value

def dec_to_snafu(dec):
    value = str()
    remainder = int(dec)
    digits = 1
    carry = False
    while remainder:
        if carry:
            remainder += pow(5,digits-1)
            carry = False
        lsd = int((remainder%pow(5,digits))/pow(5,digits-1))#Least Significant Digit (in Base 5)
        if lsd > 2:#Handels 3,4
            carry = True
        value = str(rconversion[lsd])+ value
        remainder -= lsd*pow(5,digits-1)
        digits+=1
    if carry:value = '1'+ value #Handels the Most Significant Digit being Negative
    return value


for line in text:
    tanks.append(snafu_to_dec(line[:-1]))

Sumfuel = 0
for fuel in tanks:
    Sumfuel += fuel
print("Sum is in dec: "+str(Sumfuel))
print("Sum is in SNAFU: "+str(dec_to_snafu(Sumfuel)))

print("Done")


