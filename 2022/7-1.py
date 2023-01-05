import copy
textfile = open("7-1.txt","r")
text = textfile.readlines()

currentpath = []
folders = list()

class Folder:
    def __init__(self,path):
        self.path = copy.deepcopy(path)
        self.basesize = 0
        self.subsize = 0
        self.sumsize = 0
        self.subfolders = []

    def __str__(self):
        return f"({self.path}\nSubfolders:{self.subfolders}\nBasesize:{self.basesize}\nSubsize:{self.subsize})"

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
    elif (x[0] == "d"):
        folders[-1].subfolders.append(x.split()[1])
    else:
        folders[-1].basesize += int(x.split()[0])

print("Folders created:", end=" ")
print(len(folders))
#for x in folders:
#    print(x,end = "\n\n")

Sum = 0
for x in folders:
    if(x.sumsize <= 100000):
        Sum += x.sumsize
print(Sum)
