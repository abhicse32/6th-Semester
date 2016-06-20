#!/usr/bin/env python
__author__ = 'abhishek'

import matplotlib.pyplot as plt
import numpy as np
import os
import sys
import re
import networkx as nx

def plot(freq_list, months):
    """freq_list: 3-D list of the form:
    [[[community_size, frequency],.....,][[community_size,frequency],.......,],.......]
    months: lists of months
    """
    colormap = plt.cm.gist_ncar
    list_len= len(freq_list)
    labels=[]
    for i in range(list_len):
        plt.plot(freq_list[i])
        labels.append(months[i])

    plt.xlabel('Community-size')
    plt.ylabel('number of communities')
    plt.legend(labels, ncol=7, loc='upper center', bbox_to_anchor=[.5, 1.1],
               columnspacing= 1.0, labelspacing= 0.0,
               handletextpad= 0.0, handlelength= 1.5,
               fancybox= True, shadow= True)
    plt.show()
def get_the_frequency(path):
    """path: path to the files containing communities. and these are stored as:
       label1: node_n11, node_n12,.......,
       label2: node_n21, node_n22........
       .
       .
       .

       returns the dictionary of community sizes and their frequencies
    """
    frequency_dict= {}
    with open(path, "r") as file_:
        for lines in file_:
            lst = filter(None, re.split(r"[, ]+", lines))
            cluster_len = len(lst)-1
            if cluster_len> 5:
                if cluster_len not in frequency_dict:
                    frequency_dict[cluster_len]=0
                frequency_dict[cluster_len]+=1
    return frequency_dict

def get_freq_from_graph(path):
    """Same as above except in this case, instead
       file containing community grapsh, the original graph
       itself is passed, and returns community sizes and their
       frequencies
    """
    freq_dict= {}
    G= nx.Graph()
    edges= []
    with open(path, "r") as file_:
        for lines in file_:
            str_lst= lines.split()
            edges.append(tuple(str_lst))
    G.add_edges_from(edges)
    del edges
    conn_comps= nx.connected_component_subgraphs(G)

    for subgraphs in conn_comps:
        nodes= subgraphs.number_of_nodes()
        if nodes >5:
            if nodes not in freq_dict:
                freq_dict[nodes]=0
            freq_dict[nodes]+=1

    return freq_dict

# functiont to iterate through the files in a directory
def iterate(path, flag, freq_lst, months_list):

    """Iterates through the files/subdirectories of the given path"""
    for root, dirs , files in os.walk(path):
        for file_ in files:
            months_list.append(file_)
            file_path= os.path.join(root,file_)
            dict_freq= None
            if flag==1:
                dict_freq= get_the_frequency(file_path)
            else:
                dict_freq= get_freq_from_graph(file_path)

            temp= [[key, dict_freq[key]] for key in dict_freq]
            freq_lst.append(temp)

        for subdirs in dirs:
            sub_path= os.path.join(root, subdirs)

if __name__=='__main__':
    """path_data_n: path to path_n/ directory containing the edges of the graph
       path_result: directory contains files, containing the clusters obtained in the graphs
       path_large_data: contains graphs of sizes |V| ~ 10^5 and |E|~ |10^7|
    """
    path_data_n= sys.argv[1]
    path_result= sys.argv[2]
    path_large_data= sys.argv[3]

    list_data_n= os.listdir(path_data_n)
    list_result= os.listdir(path_result)

    for files in list_data_n:
        if files not in list_result:
            file_path= os.path.join(path_data_n, files)
            os.system("mv "+file_path+" "+path_large_data)

    freq_lst, months_list= [], []
    iterate(path_result, 1, freq_lst, months_list)
    iterate(path_large_data, 2, freq_lst, months_list)
    plot(freq_lst, months_list)