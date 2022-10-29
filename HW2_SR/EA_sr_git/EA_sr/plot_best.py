import matplotlib.pyplot as plt
import csv

data = open("data.txt")
p_x = []
p_y = []
for line in open("data.txt"): 
    line = data.readline()   
    # print(line)      
    line = line[:-1]     #去掉换行符
    x,y = line.split(", ")
    p_x = p_x+[float(x)]
    p_y = p_y+[float(y)]

    
data.close() #关闭文件  
# print(p_x,p_y)
# plt.plot(p_x,p_y)
# plt.axis([0,1,0,1])
# plt.show()
 


b_x = []
b_y = []

with open('lc_dot_1.csv', newline='') as csvfile:
    f_csv = csv.reader(csvfile)
    header = next(f_csv)
    for row in f_csv:
        
        b_x.append(float(row[0]))
        b_y.append(float(row[1]))

csvfile.close()

plt.plot(p_x,p_y,'o',color ='r' )
plt.plot(b_x,b_y, color = 'b')
plt.show()




