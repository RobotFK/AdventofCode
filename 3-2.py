textfile = open("3-1.txt","r")
text = textfile.readlines()
Sum = 0
for x in text:
  print(str((len(x)-1)/2))
  Compartment1 = ""
  Compartment2 = ""
  Match = "a"
  Matchfound = False
  for Char in range(int((len(x)-1)/2)):
    Compartment1 += x[Char]
    
  for Char in range(int((len(x)-1)/2),int((len(x)-1))):
    Compartment2 += x[Char]
  #print(Compartment1) 
  for Char1 in Compartment1:
    for Char2 in Compartment2:
      if Char1 == Char2 and Match != Char1:  
      #Next line Cuts down the if Statement a bit 
        if (ord(Char1) >= ord(Match) and ord(Match) >= 97)\
        or (ord(Char1) <= 90 and (ord(Match) >= 97 or  ord(Char1) > ord(Match))):
          #print(Char1+" Replaces "+ Match)
          Match = Char1
          Matchfound = True
          if Match.islower():
            Value = (ord(Match)-96)
          else:
            Value = (ord(Match)-38)
  
  if Matchfound:
    print("Final Match:"+ Match + " " + str(Value))
    Sum += Value
  else:
    print("No match:"+ Compartment1 + " - " + Compartment2)
  #break
print(Sum)
