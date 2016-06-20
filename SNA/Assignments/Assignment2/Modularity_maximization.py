__author__ = 'abhishek'

import networkx as nx
import numpy as np
from numpy import linalg as LA
import sys
from sklearn.cluster import KMeans

#reading the data from the file
def read_datset():
    filename=sys.argv[1]
    file_=open(filename,"r")
    edges,nodes=[],[]
    for lines in file_:
        temp=[int(val)-1 for val in lines.split()[:2]]
        edges.append(temp)
        if temp[0] not in nodes:
            nodes.append(temp[0])
        if temp[1] not in nodes:
            nodes.append(temp[1])
    file_.close()
    nodes.sort()
    return nodes[len(nodes)-1]+1,edges

def create_adjacency(n,edges):
    A=[[0 for val in range(n)] for vals in range(n)]
    degree_count={}
    for edge in edges:
        A[edge[0]][edge[1]]=1
        A[edge[1]][edge[0]]=1
        if edge[0] in degree_count:
            degree_count[edge[0]]+=1
        else:
            degree_count[edge[0]]=1
        if edge[1] in degree_count:
            degree_count[edge[1]]+=1
        else:
            degree_count[edge[1]]=1

    return A,degree_count

def degree_vector(n,m,degree_count,A):
    d=[[0] for val in range(n)]
    for nodes in degree_count:
        d[nodes][0]=degree_count[nodes]
    d=np.array(d)
    dT=d.T
    d_dot_dT=np.dot(d,dT)
    d_dot_dT=d_dot_dT/(2.0*m)
    B=np.subtract(A,d_dot_dT)
    return B

def ground_truth(filename):
    file_=open(filename,"r")
    count,labels=0,{}
    for lines in file_:
        labels[count]=int(lines)-1
        count+=1
    file_.close()
    return labels

def top_k_eigen_vectors(A):
    eig_val,eig_vecotr=LA.eig(np.array(A))
    sorted_eig_vals=[[eig_val[i],i] for i in range(len(eig_val))]
    sorted_eig_vals=sorted(sorted_eig_vals,key=lambda x:x[0],reverse=True)
    top_k_values=sorted_eig_vals[:5]
    top_k_vectors=[list(eig_vecotr[vals[1]]) for vals in top_k_values]
    return top_k_vectors

def find_clusters(top_k):
    top_k=np.array(top_k)
    top_k=top_k.T
    k_means=KMeans(5)
    k_means.fit(top_k)
    Z=k_means.predict(top_k)
    return Z

def purity_(Z,labels):
    clusters = [0,1,2,3,4]
    numerator = 0
    for i in clusters:
        max_1 = 0
        max_2 = 0
        max_3 = 0
        max_4 = 0
        max_5 = 0
        for j in range(len(Z)):
            if Z[j]==i:
                if labels[j]==0:
                    max_1 = max_1+1
                elif labels[j]==1:
                    max_2 = max_2+1
                elif labels[j]==2:
                    max_3 = max_3+1
                elif labels[j]==3:
                    max_4 = max_4+1
                elif labels[j]==4:
                    max_5 = max_5+1
        numerator = numerator + max(max_1,max_2,max_3,max_4,max_5)
    purity = float(numerator)/419
    return purity

n,edges=read_datset()
A,degree_count=create_adjacency(n,edges)
filename2=sys.argv[2]
labels=ground_truth(filename2)
m=len(edges)
B=degree_vector(n,m,degree_count,A)
top_k=top_k_eigen_vectors(B)
Z=find_clusters(top_k)
print labels
print purity_(Z,labels)
