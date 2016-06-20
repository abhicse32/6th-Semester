import re
import networkx as nx
from operator import itemgetter
import matplotlib.pyplot as plt
import powerlaw
prof_lst=["John Augustine","Shankar Balachandran","Sutanu Chakraborti",
     "C. Chandra Sekhar","Sukhendu Das","T. A. Gonsalves","D. Janakiram",
     "V. Kamakoti","Deepak Khemani","P. Sreenivasa Kumar",
     "Rajsekar Manokaran","Chester Rebeiro","Anurag Mittal",
     "Hema A. Murthy","C. Siva Ram Murthy","Madhu Mutyam",
     "V. Krishna Nandivada","Meghana Nasre","Rupesh Nasre",
     "N. S. Narayanaswamy","S. V. Raghavan","C. Pandu Rangan",
     "Sayan Ranu","B. V. Raghavendra Rao","B. Ravindran",
     "Jayalal Sarma","Krishna M. Sivalingam"]
def create_edges():
	read_from=open("data.txt","r")
	lst,curr_prof=[],""
	for lines in read_from:
		if lines[0]!='@':
			match=re.search(r"[a-zA-Z][a-zA-Z. ]*[a-zA-Z.]",lines)
			lst.append((curr_prof,match.group()))
		else:
			curr_prof=lines[1:].rstrip('\n\t ');
	return lst

prof_dict={}
with open("prof.txt","r") as file_:
    for lines in file_:
        str_= lines.split('"')
        prof_dict[str_[1]]=str_[3]

lst=create_edges()
G=nx.Graph()
G.add_edges_from(lst)

print(G.number_of_edges())
# degree list
degree_list= nx.degree(G).values()
degree_set=set(degree_list)
nodes_= G.number_of_nodes()
min_ = min(degree_set)
max_ = max(degree_set)
data_ = []
for i in range(len(degree_set)):
    k=degree_set.pop()
    fraction = float(degree_list.count(k))/nodes_
    data_.append(fraction)

results = powerlaw.Fit(data_)
print(results.power_law.alpha)

#nx.draw(G)
#plt.show()
#number of connected components of the graph
print "1. number of connected components: %d"%nx.number_connected_components(G)

#diameter of each component
sub_graph_list=nx.connected_component_subgraphs(G)
#for subs in sub_graph_list:
#	print list(set(subs.nodes()).intersection(set(prof_lst)))

print "2. diameters of the connected components are:",
for graph in sub_graph_list:
	print nx.diameter(graph,e=None),
print 

#closeness centrality
num=G.number_of_nodes()/10
degree_list=nx.degree(G)
degree_list=sorted(degree_list.items(), key=itemgetter(1))
degree_list.reverse()
top_ten_percent=degree_list[:num]
print "Closeness centrality of top 10 percent nodes:"
closeness_centrality_={}
for nodes in top_ten_percent:
	value=nx.closeness_centrality(G,nodes[0],None,normalized=True)
	closeness_centrality_[nodes[0]]=value

#for nodes in sorted(closeness_centrality_,key=closeness_centrality_.get,reverse=True):
#	print "(%s, %f),"%(nodes, closeness_centrality_[nodes])

# betweeenness centrality
print "\nbetweenness centrality of the top 10% nodes:"
value=nx.betweenness_centrality(G,k=None,normalized=True,weight=None,endpoints=False,seed=None)
#for nodes in top_ten_percent:
#	print "(%s: %f), "%(nodes[0],value[nodes[0]]),


#clustering coefficient 
clustering_coeffs_={}
print "clustering coefficient of the nodes:"
for nodes in top_ten_percent:
	value=nx.clustering(G,nodes[0],weight=None)
	clustering_coeffs_[nodes[0]]=value
#for nodes in sorted(clustering_coeffs_, key=clustering_coeffs_.get,reverse=True):
#	print "(%s: %f), "%(nodes, clustering_coeffs_[nodes]),
kamakoti_numbers={}
for profs in prof_lst:
	if profs!='V. Kamakoti':
		if nx.has_path(G,'V. Kamakoti',profs)==True:
			kamakoti_numbers[profs]=len(nx.shortest_path(G,profs,"V. Kamakoti",weight=None))-1
		else:
			kamakoti_numbers[profs]=-1
print kamakoti_numbers
