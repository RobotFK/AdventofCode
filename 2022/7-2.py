import copy
textfile = open("7-1.txt","r")
text = textfile.readlines()
currentpath = []
folders = list()
Disksize = 70000000 #70Million

class Folder:
    def __init__(self,path):
        self.path = copy.deepcopy(path)
        self.basesize = 0
        self.subsize = 0
        self.sumsize = 0
        self.subfolders = []

    def __str__(self):
        return f"{self.path}\nSubfolders:{self.subfolders}\nBasesize:{self.basesize}\nSubsize:{self.subsize}\nSumsize:{self.sumsize}"

    def sumsizecalc(self):
        self.sumsize = self.basesize + self.subsize    
for x in text:
    #print(x, end =" ")
    if(x[0] == "$"):
        #print("syscomand")
        if(x[2] == "l"): # $ ls
        #This means we just entered a Folder for the First time
            folders.append(Folder(currentpath))
        elif(x[2] == "c"): # $ cd
            if(x[5] != "."): # $ cd into something
                currentpath.append(x.split()[2])
                #print(currentpath)
            else: # cd .. -> go up one Folder
                #print("leaving")
                #As a result we add the Size of our Childfolder to the Parrent
                #This is done by Calculating the Total size of the Child and adding that to the Parent (and finding both by Search for a folder with matching Path)
                size = 0
                for x in folders:
                    if(currentpath == x.path):
                        x.sumsizecalc()
                        #print("Sum Calculated")
                        size = x.sumsize
                currentpath.pop(-1)
                for x in folders:
                    if(currentpath == x.path):
                        x.subsize += size
                        #print("Sum Subsize added")
    
    elif (x[0] == "d"): #dir
        folders[-1].subfolders.append(x.split()[1])
    else: # File
        folders[-1].basesize += int(x.split()[0])

while(currentpath != []):
        print("Leaving : ", end="")
        print(currentpath)
        for x in folders:
            if(currentpath == x.path):
                x.sumsizecalc()
                #print("Sum Calculated")
                size = x.sumsize
        currentpath.pop(-1)
        for x in folders:
            if(currentpath == x.path):
                x.subsize += size
                #print("Sum Subsize added"

print("Folders created:", end=" ")
print(len(folders))
#for x in folders:
#    print(x,end = "\n\n")
#folders[0].sumsizecalc()#As / has its summ never calculated we do this manually here once
#folders[-1].sumsizecalc()#As the last folder has its summ never calculated we do tis herer aswell
#print(folders[0])
unusedspace = 70000000 - folders[0].sumsize
neededspace = 30000000 - unusedspace 
contender = folders[0]
#print("Currently "+ str(folders[0].sumsize)+" of 70M are taken up")
#print("Only " + str(unusedspace)+" are free")
print("Looking for min: " + str(neededspace))
for x in folders:
    if(x.sumsize >= neededspace and x.sumsize < contender.sumsize):
        contender = x
print("\nBest match:")
print(contender)
# Too high: 38529505
# Too low:  3253197

