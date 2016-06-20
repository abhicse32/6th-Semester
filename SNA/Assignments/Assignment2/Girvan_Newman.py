__author__ = 'abhishek'

import networkx as nx
import sys

#reading the data from the file
def read_datset():
    filename=sys.argv[1]
    file_=open(filename,"r")
    edges,nodes=[],[]
    for lines in file_:
        temp=[int(val)-1 for val in lines.split()[:2]]
        edges.append(tuple(temp))
        if temp[0] not in nodes:
            nodes.append(temp[0])
        if temp[1] not in nodes:
            nodes.append(temp[1])
    file_.close()
    nodes.sort()
    return nodes[len(nodes)-1]+1,edges

def ground_truth():
    filename=sys.argv[2]
    file_=open(filename,"r")
    count,labels=0,{}
    for lines in file_:
        labels[count]=int(lines)-1
        count+=1
    file_.close()
    return labels

def create_graph(n,edges):
    G=nx.Graph()
    G.add_nodes_from([val for val in range(n)])
    G.add_edges_from(edges)
    components=nx.connected_component_subgraphs(G)
    for comps in components:
        if comps.number_of_nodes() >400:
            return comps

    return G

def best_betweenness(G_):
    dict_betweenness=nx.edge_betweenness_centrality(G_)
    items_list=dict_betweenness.items()
    items_list.sort(key=lambda x:x[1],reverse=True)
    return  items_list[0][0]

def Girwan_Newman(G):
    if G.number_of_nodes()==1:
        return G.nodes()
    lst=list(nx.connected_component_subgraphs(G))
    while len(lst) <5:
        X=best_betweenness(G)
        G.remove_edge(X[0],X[1])
        lst=list(nx.connected_component_subgraphs(G))
    return G
n,edges=read_datset()
G=create_graph(n,edges)
del edges
labels=ground_truth()
G=Girwan_Newman(G)
components=nx.connected_component_subgraphs(G)
numerator=0
for comps in components:
    max0,max1,max2,max3,max4=0,0,0,0,0
    cluster_nodes=comps.nodes()
    for nodes in cluster_nodes:
        if nodes in labels:
            x=labels[nodes]
            if x==0:
                max0+=1
            elif x==1:
                max1+=1
            elif x==2:
                max2+=1
            elif x==3:
                max3+=1
            else:
                max4+=1
    numerator+=max(max0,max1,max2,max3,max4)
print float(numerator)/412.0

# 0.458737864078
#Spectral custering : 0.491646778043
# Modularity Maximization: 0.484486873508
# Block Model Approaximation : 0.479713603819

