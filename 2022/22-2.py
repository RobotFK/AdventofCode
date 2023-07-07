import itertools #Allows for quick moves manipulation
import math #Nice Flooring
with open("22-1.txt","r") as file:
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
sidelen = int((len(board)-2)/4)
def wrap ():#Returns Flase if it would wrap into a wall
    #Due to the different warping of The Faces this algorithem will only work for the Real data, not the test
    #Our Cube with Faces like a dice :
    #
    #    14
    #    2
    #   36
    #   5
    global facing
    global pos
    global board
    global sidelen
    '''
    Wrap status:
    1->3 (Inverted) Works
    1->5 Works
    4->5 Works
    4->2 Works
    4->6 (Inverted) Works
    2->4 Works
    2->3 Works
    3->2 Works
    3->1 (Inverted) Works
    6->4 (Inverted) Works
    6->5 Works
    5->1 (Inverted)
    5->1 Works
    5->4 Works
    5->6 Works
    '''
    match math.floor((pos.y-1)/sidelen):#-1 Due to the Empty line at the Top
        case 0:
            match math.floor((pos.x-1)/sidelen):
                case 1: # Face 1
                    #print("Face1")
                    if facing == 2:# Wrap left to 3 enterting facing right
                        if board[((sidelen+1)-pos.y) +(sidelen*2)][1] == '#':return False
                        facing = 0
                        pos.x = 1
                        pos.y = ((sidelen+1)-pos.y) +(sidelen*2) # 1-50 -(x)-> 150-101
                        return True
                    if facing == 3:# Wrap up to 5 enterting facing right
                        #breakpoint()
                        if board[pos.x +(sidelen*2)][1] == '#':return False
                        facing = 0
                        pos.y = pos.x +(sidelen*2) #51-100 -> 151-200
                        pos.x = 1
                        return True
                case 2: # Face 4
                    #print("Face4")
                    if facing == 0:# Wrap Right to 6 enterting facing left
                        if board[(sidelen*3 +1) -pos.y][sidelen*2] == '#':return False
                        facing = 2
                        pos.x = sidelen*2
                        pos.y = (sidelen*3 +1) -pos.y #1-50 -(x)-> 150-101
                        return True
                    if facing == 1:# Wrap down to 2 enterting facing left
                        if board[pos.x -sidelen][sidelen*2] == '#':return False
                        facing = 2
                        pos.y = pos.x -sidelen   # 101- 150 -> 51-100
                        pos.x = sidelen*2
                        return True
                    if facing == 3:# Wrap up to 5 enterting facing up
                        if board[sidelen*4][pos.x -sidelen*2] == '#':return False
                        facing = 3
                        pos.x = pos.x -sidelen*2 # 101- 150 -> 1-50
                        pos.y = sidelen*4
                        return True
                case _:
                    #breakpoint()
                    print("C0 X wrap Error")# Should never occur
        case 1:
            match math.floor((pos.x-1)/sidelen):
                case 1: # Face 2
                    #print("Face2")
                    if facing == 0:# Wrap Right to 4 enterting facing up
                        if board[sidelen][pos.y +sidelen] == '#':return False
                        facing = 3
                        pos.x = pos.y +sidelen
                        pos.y = sidelen
                        return True
                    if facing == 2:# Wrap left to 3 enterting facing down
                        if board[(sidelen*2) +1][pos.y -sidelen] == '#':return False
                        facing = 1
                        pos.x = pos.y -sidelen
                        pos.y = sidelen*2 +1
                        return True
                case _:
                    print("C1 X wrap Error")# Should never occur
        case 2:
            match math.floor((pos.x-1)/sidelen):#math.floor((pos.x-1)/sidelen):
                case 0: # Face 3
                    #print("Face3")
                    if facing == 2:# Wrap left to 1 enterting facing right
                        if board[((sidelen*3) +1)- pos.y][sidelen+1] == '#':return False
                        facing = 0
                        pos.y = ((sidelen*3) +1)- pos.y # 101-150 -(x)-> 51-1
                        pos.x = sidelen+1
                        return True
                    if facing == 3:# Wrap up to 2 enterting facing right
                        if board[pos.x +sidelen][sidelen+1] == '#':return False
                        facing = 0
                        pos.y = pos.x +sidelen
                        pos.x = sidelen+1
                        return True
                case 1: # Face 6
                    #print("Face6")
                    if facing == 0:# Wrap right to 4 enterting facing left
                        if board[((sidelen*3)+1)-pos.y][sidelen*3] == '#':return False
                        facing = 2
                        pos.x = sidelen*3
                        pos.y = ((sidelen*3)+1)-pos.y # 101-150 -(x)-> 50-1
                        return True
                    if facing == 1:# Wrap down to 5 enterting facing left
                        if board[pos.x +(sidelen*2)][sidelen] == '#':return False
                        facing = 2
                        pos.y = pos.x +(sidelen*2)
                        pos.x = sidelen
                        return True
                case _:
                    print("C2 X wrap Error")# Should never occur 
        case 3:
            match math.floor((pos.x-1)/sidelen):
                case 0: # Face 5
                    #print("Face5")
                    if facing == 0:# Wrap right to 6 enterting facing up
                        if board[sidelen*3][pos.y -(sidelen*2)] == '#':return False
                        facing = 3
                        pos.x = pos.y -(sidelen*2)
                        pos.y = sidelen*3
                        return True
                    if facing == 1:# Wrap down to 4 enterting facing down
                        if board[1][pos.x+(sidelen*2)] == '#':return False
                        facing = 1
                        pos.x = pos.x +(sidelen*2)
                        pos.y = 1
                        return True
                    if facing == 2:# Wrap left to 1 enterting facing down
                        if board[1][pos.y -(sidelen*2)] == '#':return False
                        facing = 1
                        pos.x = pos.y -(sidelen*2) # 151-200 -> 51-100
                        pos.y = 1
                        return True
                case _:
                    print("X wrap Error")# Should never occur
        case _:
            breakpoint()
            print("Y wrap Error")# Should never occur

def print_board(y,x):
    global board
    global facing
    for r_i,row in enumerate(board):
        if r_i != y:
            print(row)
            continue
        row = "".join([pos if c_i != x  else str(facing) for c_i,pos in enumerate(row)])
        print(row)
    print("\n")

input("Main moves")
for move in movelist:
    #print(str(move) + " F:" + str(facing))
    if move == "R":facing = (facing+1)%4
    elif move == "L":facing = (facing-1)%4
    else:
        for step in range(int(move)):
            #print("Step:"+str(step+1))
            #print_board(pos.y,pos.x)
            match facing:
             case 0: #Right
                ahead = board[pos.y][pos.x+1]
                #print("R Ahead: "+str(ahead))
                if ahead == '#':break
                if ahead == ' ':
                    if not wrap ():break
                if ahead == '.':pos.x += 1
             case 1: #Down
                ahead = board[pos.y+1][pos.x]
                if ahead == '#':break
                if ahead == ' ':
                    if not wrap ():break
                if ahead == '.':pos.y += 1 
             case 2: #Left
                ahead = board[pos.y][pos.x-1]
                if ahead == '#':break
                if ahead == ' ':
                    if not wrap ():break
                if ahead == '.':pos.x -= 1 
             case 3: #Up
                ahead = board[pos.y-1][pos.x]
                if ahead == '#':break
                if ahead == ' ':
                   if not wrap ():break
                if ahead == '.':pos.y -= 1 

print("Final at:")
print(str(pos.x) + " - " + str(pos.y))
print("Facing"+str(facing))
print("Sum: "+ str( 1000*pos.y + 4*pos.x + facing))
#138401 too low

