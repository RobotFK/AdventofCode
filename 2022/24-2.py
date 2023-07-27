import pickle #This allows us to export the created Boards and stops the need to recompute them everytime (and get banned from Company Networks)
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
unique_boards = 12 #Uodate this by hand
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
Inital_board = list()
for line in text:
    if line[1] == '#' or line[-3] == '#':continue
    Inital_board.append([values[symbol] for symbol in line[1:-2]])

#Remove again to use arraywrapping for wrapping
boards = [Inital_board]
m_r = len(boards[0]) #maximum rows, to solve overflow
m_c = len(boards[0][0]) #maximum Collums, to solve overflow
def calc_boards(v = 0):
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
            
    if input("Export ?(y/n)") == 'y':
        with open('24-1.pickle', 'wb') as f:
            pickle.dump(boards, f)


def import_boards(file):
    global boards
    with open(file, 'rb') as f:
        boards = pickle.load(f)
        
    print(str(file)+ " Importet")
    
def boards_to_bin():
    global boards
    for i,board in enumerate(boards):
        boards[i] = [[0 if not(bool(v)) else None for v in row] for row in board]
        boards[i].insert(0,*[[0]+[None]*(len(boards[i][1])-1)])
        boards[i].append(*[[None]*(len(boards[i][1])-1)+[0]])
    print("boards are now in binary")
    
def calc_path():
    #Fck this, for 24-2 we will just build a function that does Excatly and only what we need
    global boards
    state = 1 # Tells us on wich part of the Journey we are
    directions= [[ 0,-1],[ 0, 1],[-1, 0],[ 1, 0],[0,0]]# (x,y) offsets
    boards[0][0][0]= 1
    for b,board in enumerate(boards):
        if state == 1 and board[-1][-1] >= 1:
            print(str((board[-1][-1])-1)+" Minutes Needed for Part "+str(state))
            boards[b] = [[0 if v != None and r!=(len(board)-1) else v for v in row] for r,row in enumerate(board)] #Resets all (Allowed) Values exept for the one in the last row
            board = boards[b]
            state += 1
        elif state == 2 and board[0][0] >= 1:
            print(str((board[-1][-1])-1)+" Minutes Needed for Part "+str(state-1))#I dont know where this off by one originates,but i dont care anymore
            boards[b] = [[0 if v != None and r!=0 else v for v in row] for r,row in enumerate(board)] #Resets all (Allowed) Values exept for the one in the last row
            board = boards[b]
            state += 1
        elif state == 3 and board[-1][-1] >= 1:
            print(str((board[-1][-1])-1)+" Minutes Needed for Part "+str(state-1))#I dont know where this off by one originates,but i dont care anymore
            boards[b] = [[0 if v != None and r!=(len(board)-1) else v for v in row] for r,row in enumerate(board)] #Resets all (Allowed) Values exept for the one in the last row
            board = boards[b]
            state += 1
            return
        next_board = (b+1)%len(boards)
        for r,row in enumerate(board):#Row is the Y axis
            for c,value in enumerate(row):#Collum/Value is the X axis
                if value is None:continue
                #Reaching here means that we are pointing to a valid spot
                if value <= 0:continue
                 #Reaching here means that we are pointing to a spot that we can reach
                for d in directions:
                    if r + d[1] < 0 or r + d[1] >= (len(board)):continue #Y axis oob
                    if c + d[0] < 0 or c + d[0] >= len(board[0]):continue #X axis oob
                    #if value == 1:input("At Board["+str(b)+"] "+str(r)+"-"+str(c)+" D:"+str(d))
                    if boards[next_board][r + d[1]][c + d[0]] == 0:boards[next_board][r + d[1]][c + d[0]] = value+1
        if b %100 == 0 and b != 0:print(str(b)+" Minutes passed") #Helps keep QA testers sane

def print_boards(index = -1):
    global boards
    if index == -1:
        for board in boards:
            for r in board:
                for v in r:
                    print('#' if v is None else '.' if v == 0 else str(v % 10), end='')
                print('\n',end='')
    else:
        for r in boards[index]:
            for v in r:
                print('#' if v is None else '.' if v == 0 else str(v % 10), end='')
            print('\n',end='')
    print("\n")

import_boards("24-1.pickle")
repeat = 10 #Change by hand
boards = boards + boards[:-1] * repeat
boards_to_bin()
#input()
calc_path()
print("Done")
'''
print_boards(33)
import_boards("24-1-1.pickle")
first_back = calc_path(1,first+2)
print("First_back="+str(first_back))
'''
#Internal notes:
#Board index(b) is the Minute
#The value is the Minute+1
