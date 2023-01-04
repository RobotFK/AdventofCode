textfile = open("1-1.txt","r")
text = textfile.readlines()
MaxElf = 0
Elf = 0
for x in text:
  if x != "\n":
   #print("Cal is " + str(x))
   Elf += int(x)
  else:
    #print("Elf at " + str(Elf))
    if Elf >= MaxElf:
        MaxElf = Elf
    Elf = 0

print(str(MaxElf))
