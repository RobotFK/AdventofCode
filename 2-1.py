textfile = open("2-1.txt","r")
text = textfile.readlines()
Score = prescore = 0
# X  = Loss +0
# Y  = Draw +3
# Z  = Win  +6
# Stone(==A) +1 
# Paper(==B) +2
# Scissor(==C) +3
for x in text:
  prescore = Score
  if x[2] == "X":
       Score += 0
  elif x[2] == "Y":
       Score += 3
  elif x[2] == "Z":
       Score += 6

  if x == "A Y\n" or x == "B X\n" or x == "C Z\n" :
      Score += 1
      #print("Rock")

  elif x == "A Z\n" or x == "B Y\n" or x == "C X\n" :
      Score += 2
      #print("Papper")

  elif x == "A X\n" or x == "B Z\n" or x == "C Y\n" :
      Score += 3
      #print("Scissor")

  #print( str(x) + " gives " + str(Score-prescore)) 
print(str(Score))
