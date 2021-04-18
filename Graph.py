#!/usr/bin/env python
# coding: utf-8

# In[2]:


import matplotlib.pyplot as plt


# In[12]:


# x axis values
x1 = [512,1024,2048,3072,4096]
# corresponding y axis values
y1 = [4114,27136,256276,796611,1987031]
plt.plot(x1, y1, label = "Key Generation")

x2 = [512,1024,2048,3072,4096]
y2=[62,95,182,270,375]
plt.plot(x2, y2, label = "Encryption")

x3 = [512,1024,2048,3072,4096]
y3=[706,1986,7923,18893,40909]
plt.plot(x3, y3, label = "Decryption")

plt.yscale('log')
# naming the x axis
plt.xlabel('Key Size')
# naming the y axis
plt.ylabel('Time (microseconds)')
plt.xlim([500,4500])
# giving a title to my graph
#plt.title('Time taken for RSA Key Generation')
plt.legend()
# function to show the plot
plt.show()
plt.savefig('graph.jpg')


# In[ ]:




