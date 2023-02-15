textfile = open("5-1.txt","r")
text = textfile.readlines()

'''
[Q] [J]                         [H]
[G] [S] [Q]     [Z]             [P]
[P] [F] [M]     [F]     [F]     [S]
[R] [R] [P] [F] [V]     [D]     [L]
[L] [W] [W] [D] [W] [S] [V]     [G]
[C] [H] [H] [T] [D] [L] [M] [B] [B]
[T] [Q] [B] [S] [L] [C] [B] [J] [N]
[F] [N] [F] [V] [Q] [Z] [Z] [T] [Q]
 1   2   3   4   5   6   7   8   9
 '''

Cargo = [    ['F','T','C','L','R','P','G','Q'],
             ['N','Q','H','W','R','F','S','J'],
             ['F','B','H','W','P','M','Q'],
             ['V','S','T','D','F'],
             ['Q','L','D','W','V','F','Z'],
             ['Z','C','L','S'],
             ['Z','B','M','V','D','F'],
             ['T','J','B'],
             ['Q','N','B','G','L','S','P','H']
             ] #Faster do to this by Hand
for x in text:
    if x[0] != 'm':
     continue
    raw = x.split()
    Amount = int(raw[1])
    Take = int(raw[3]) -1 # Converted to Index
    Give = int(raw[5]) -1 # Converted to Index
    #print("\n")
    #print(raw)

    #print(str(Take+1), end =" ")
    #print(Cargo[Take])
    #print(str(Give+1), end =" ")
    #print(Cargo[Give])
    
    for i in range(Amount):
        Crane = Cargo[Take][-1]
        Cargo[Take].pop(-1) #Needs to always take the last one

        #print("Crane:"+ Crane)

        Cargo[Give].append(Crane)
    #print(str(Take+1), end =" ")
    #print(Cargo[Take])
    #print(str(Give+1), end =" ")
    #print(Cargo[Give])
    
for x in range(9):
    print(Cargo[x][-1], end ="")

