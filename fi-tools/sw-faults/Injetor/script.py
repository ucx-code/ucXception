from subprocess import call
from os import listdir
import sys
sys.stdout = open('stats.txt', 'w')


lista = listdir()
for i in range (len(lista)-1,0,-1):
    if (lista[i].endswith(".c") or lista[i].endswith(".cpp")):
        #call(["prompt", "$d", "$t", "$_$P$G"])
        call(["java", "-jar", "FaultInjector.jar" ,lista[i]])
        
    



files = []
operadores = ["Nº Lin","MIFS","MLAC","MFC","MIA","MLOC","MLPA","MVAE","MVAV","MIEB","MVIV","WVAV","WAEP","WPFV", "TOTAL", "RACIO"]
total = [0]*len(operadores)
patchs = []
stats = []
numficheiros = 0


total_index = len(operadores) - 2
racio_index = len(operadores) - 1

numlinhas_index = 0

lista = listdir()
for i in range (len(lista)-1,0,-1):
    if (lista[i].endswith(".c") or lista[i].endswith(".cpp")):
        if (lista[i].find("FORMATTED") == -1):
            numficheiros = numficheiros+1
            files.append(lista[i])
            stats.append([0]*len(operadores))
            
           
    if (lista[i].endswith(".patch")):
        patchs.append(lista[i])
            
for i in range (0,len(files)):
    
    with open(files[i]) as myfile:
        count = sum(1 for line in myfile)         
    stats[i][numlinhas_index] = count
    total[numlinhas_index] = total[numlinhas_index]  + count
    
    for j in range (0,len(patchs)):
        for k in range (0,len(operadores)):
            if (patchs[j].find(files[i]+"._"+operadores[k]) != -1):
                stats[i][k] = stats[i][k] + 1
                stats[i][total_index] = stats[i][total_index] + 1
                total[k] = total[k] + 1
                total[total_index] = total[total_index] + 1
                
                

print("Numero de ficheiros: \t" + str(numficheiros))   
print("Numero de patchs:    \t" + str(total[total_index]))   
print()

first = 4
others = 12
last = 5
    
print("".ljust(first)+"".ljust(others)+"\t",end="")
for i in range(0,len(operadores)):
    print(str(operadores[i]).ljust(last)+"\t",end="")
print()
for i in range(0,len(files)):    
    print(str(i).ljust(first)+str(files[i]).ljust(others)+"\t",end="")
    for j in range(0,len(stats[i])):
        if (j==racio_index):
            print(str(round(stats[i][j-1]/stats[i][numlinhas_index],2)).rjust(last)+"\t",end="")
        else:
            print(str(stats[i][j]).rjust(last)+"\t",end="")
    print()
    
    
print("".ljust(first)+"".ljust(others)+"\t",end="")
for j in range(0,len(total)):
    print(str(total[j]).rjust(last)+"\t",end="")    
    
    