import pickle #This allows us to export the created Boards and stops the need to recompute them everytime
import copy
with open("24-1.txt","r") as file:
    text = file.readlines()
'''
Groundwork:
Input is 27*122
Giving us 25*120 = 3k positions
There are 3k-306 = 2694 Storms
Since lcm(25,120)= 600 there are only 600 unique boards

If we precalc them all we could just treat this like an 3D Pathfinding problem
We could even optimise by just thinking about the clear paths, as the Storms are
unique and remove all dead ends to save computation :)

The board gets convertet to numbers, and overlaps get handeld as an Addition
'''
unique_boards = 600 #Uodate this by hand
#
# 12 lcm(4,6)
# 600 lcm(25,120)
values = {
    '#' : -1, 
    '.' : 0,
    '^' : 1,  
    '>' : 2,
    'v' : 4,  
    '<' : 8
    }
Inital_board = list()#Inital board
for line in text:
    if line[1] == '#' or line[-3] == '#':continue
    Inital_board.append([values[symbol] for symbol in line[1:-2]])

#Remove again to use arraywrapping for wrapping
boards = [Inital_board]
m_r = len(boards[0]) #maximum rows, to solve overflow
m_c = len(boards[0][0]) #maximum Collums, to solve overflow
def calc_boards(file,v = 0):
    global boards
    global m_r
    global m_c
    boards = [Inital_board]
    for i in range(unique_boards):
        new_board = [[0 for c in enumerate(boards[-1][0])] for r in enumerate(boards[-1])]
        for r,row in enumerate(boards[-1]):
            for c,value in enumerate(row):
                #Same order as the Dict, lower,left,higher,right
                #print(str(r)+"-"+str(c))
                new_board[r][c] = 0
                if boards[-1][(r+1)%m_r][c]%2:new_board[r][c] += 1
                if (boards[-1][r][c-1] >>1)%2:new_board[r][c]+=2
                if (boards[-1][r-1][c] >>2)%2:new_board[r][c]+=4
                if boards[-1][r][(c+1)%m_c] >= 8 :new_board[r][c]+=8
        if v == 1: #v for visuals
            for r in new_board:print(r)
            print("\n")
        boards.append(new_board)
            
    if input("Export ?") == 'y':
        with open('24-1.pickle', 'wb') as f:
            pickle.dump(boards, f)


def import_boards(file):
    global boards
    with open(file, 'rb') as f:
        boards = pickle.load(f)
        
    print(str(file)+ " Importet")

def calc_path():
    global boards
    #This first Changes all spaces we are allowed to move on to the value of 1 and all others to the value of 0
    for i,board in enumerate(boards): 
        boards[i] = [[int(not bool(v)) for v in row] for row in board]
    print("boards now in binary")
    #Then we just let exploratory pathfinding run through it
    boards[1][0][0] = 2
    taken_time = 0
    directions= [[ 0,-1],[ 0, 1],[-1, 0],[ 1, 0],[0,0]]# (x,y) offsets

    while not taken_time:
        for b,board in enumerate(boards):
            for r,row in enumerate(board):#Row is the Y axis
                for c,value in enumerate(row):#Collum/Value is the X axis
                    if value <= 1:continue
                    #Reaching here means that we are pointing to a valid spot
                    for d in directions:
                        if r + d[1] < 0 or r + d[1] >= len(board):continue #Y axis oob
                        if c + d[0] < 0 or c + d[0] >= len(board[0]):continue #X axis oob
                        #if value == 999:print("At Board["+str(b)+"] "+str(r)+"-"+str(c)+" D:"+str(d))
                        if boards[(b+1)%len(boards)][r + d[1]][c + d[0]] == 1:boards[(b+1)%len(boards)][r + d[1]][c + d[0]] = value+1
            if board[-1][-1] > 1:
                taken_time = (board[-1][-1])
                break
            if b %100 == 0 and b != 0:print("100 Cycels iterated")

    print("First result is "+str(taken_time))

def print_boards(index = -1):
    global boards
    if index == -1:
        for board in boards:
            for r in board:
                print(r)
            print("\n")
    else:
        for r in boards[index]:
            print(r)
        
#calc_boards("24-1.pickle")
import_boards("24-1.pickle")
calc_path()
