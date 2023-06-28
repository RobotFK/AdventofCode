import itertools #Allows for quick moves manipulation
with open("22-1-1.txt","r") as file:
    text = file.readlines()

board = [line.strip("\n") for line in text[:-2]] #[y][x]
#The additiional padding means we never run out of bounds
board = [*[' ' * len(board[0])] + board+[' ' * len(board[0])]]#Top and bottom Padding
for i,row in enumerate(board): #L and R Padding
    board[i] = " " + row + " "

for i,row in enumerate(board): #L and R Padding
    if len(row) < max(len(row) for row in board):
        board[i] = row.ljust(max(len(row) for row in board)," ")

moves = text[-1].strip("\n")
moves = [i for i in text[-1].rsplit("R")]
r = ["R"] * (len(moves))
moves = list(itertools.chain.from_iterable(zip(moves,r)))
moves = moves[:-1] #Removes the overhanging R
moves = [seg.rsplit("L") for seg in moves]
movelist= list()
for i,seg in enumerate(moves):
    if len(seg) == 1:movelist.extend(seg)
    else:
        seg = list(itertools.chain.from_iterable(zip(seg,["L"] * len(seg))))
        seg = seg[:-1] #Removes the overhanging L
        movelist.extend(seg)

facing = 0
pos = type('pos', (), {'x': board[1].find("."), 'y': 1})()
for move in movelist:
    #print(str(move) + " F:" + str(facing))
    if move == "R":facing = (facing+1)%4
    elif move == "L":facing = (facing-1)%4
    else:
        for step in range(int(move)):
            #print("Step:"+str(step+1))
            if facing == 0: #Right
                ahead = board[pos.y][pos.x+1]
                #print("R Ahead: "+str(ahead))
                if ahead == '#':break
                if ahead == ' ':
                   if board[pos.x].find("#") < board[pos.x].find("."):break
                   else:pos.x = board[pos.y].find(".")
                if ahead == '.':pos.x += 1
            if facing == 1: #Down
                ahead = board[pos.y+1][pos.x]
                if ahead == '#':break
                if ahead == " ":
                    ahead = board[next(i for i,row in enumerate(board) if row[pos.x] != " ")][pos.x]
                    if ahead == '#':break
                    if ahead == '.':pos.y = next(i for i,row in enumerate(board) if row[pos.x] != " ")
                if ahead == '.':pos.y += 1 
            if facing == 2: #Left
                ahead = board[pos.y][pos.x-1]
                if ahead == '#':break
                if ahead == ' ':
                   if board[pos.x].rindex("#") > board[pos.x].rindex("."):break
                   else:pos.x = board[pos.y].rindex(".")
                if ahead == '.':pos.x -= 1
            if facing == 3: #Up
                ahead = board[pos.y-1][pos.x]
                if ahead == '#':break
                if ahead == ' ':
                    #input("Up warp")
                    collum = list(reversed([row[pos.x] for row in board])) #Make some calculation quicker by using this translation
                    lowest_y = next((i for i,v in enumerate(collum) if v != " "),-1) #Ugly, but does the job
                    ahead == board[lowest_y][pos.x]
                    if ahead == '#':break
                    if ahead == '.':pos.y = lowest_y
                if ahead == '.':pos.y -= 1 

print("Final at:")
print(str(pos.x) + " - " + str(pos.y))
print(facing)
print("Sum:"+ str( 1000*pos.y + 4*pos.x + facing))
