from cmath import sin
import matplotlib.pyplot as plt
import numpy as np

# xl = set(); 
yl = set(); 
yl.clear()
x = np.arange(0, 20, 0.1)
for i in range(len(x)):
    y = x[i] *(-sin(1.5*x[i]))/(sin(-1*x[i])+(-2))
    yl.add(y)
    # xl.add(x[i])

yl = list(yl) 
plt.title("(-sin(1.5*x[i]))/(sin(-1*x[i])+(-2))")
plt.plot(x, yl)
 
plt.show()
 