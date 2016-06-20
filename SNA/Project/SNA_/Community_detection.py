#!/usr/bin/env python
__author__='__abhishek, Tapan, Pranav__'
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
from sklearn.cluster import KMeans
import os, sys
from numpy import linalg as LA
import sys
import scipy.sparse as sparse
import h5py

def forward_map(file_):
    """Since the graph nodes are strings(usernames) and functions such
       as LA.eig and KMean take integer/floats matrix, hence usernames
       mapped to (1,Number_of_users). numpy can't handle matrices of sizes
       greater than (25000x25000), hence to account for adjacency matrices
       of sizes larger than this, h5py library is used, which can handle
       matrices of arbitrary sizes.
    """
    mapping, mapped_number, edges ={}, 1 , []
    with open(file_) as f:
        for lines in f:
            users=lines.split()
            if users[0] not in mapping:
                mapping[users[0]]=mapped_number
                mapped_number+=1

            if users[1] not in mapping:
                mapping[users[1]]=mapped_number
                mapped_number+=1

            edges.append((mapping[users[0]],mapping[users[1]]))

    # inverting the mapping so as to map numbers -> users
    mapping={value: key for key,value in mapping.items()}

    # create adjacency matrix
    f= h5py.File("testFile.txt", "w")
    dset= f.create_dataset("default", (mapped_number, mapped_number), dtype='i')

    G=nx.Graph()
    G.add_edges_from(edges)
    for edge in edges:
        dset[edge[0], edge[1]]= 1
        dset[edge[1], edge[0]]= 1
    return f, dset, mapping, G,

def  top_k_eigen_vectors(A,k):
    """takes adjacency matrix as input and returns top-k
       eigen-vectors(only real parts of the vector entries)
    """
    eig_val, eig_vecotr = LA.eig(A)
    eig_val= [val.real for val in eig_val]

    sorted_eig_vals = [[eig_val[i], i] for i in range(len(eig_val))]
    sorted_eig_vals = sorted(sorted_eig_vals, key=lambda x : x[0], reverse=True)
    top_k_values = sorted_eig_vals[:k]
    top_k_vectors = [list(eig_vecotr[vals[1]]) for vals in top_k_values]
    top_k_vectors= [[val.real for val in rows] for rows in top_k_vectors]
    return top_k_vectors

def find_clusters(top_k, k):
    """takes top_k eigen-vectors and returns, k-clusters"""
    top_k = np.array(top_k)
    top_k = top_k.T
    k_means = KMeans(k)
    k_means.fit(top_k)
    Z = k_means.predict(top_k)
    return Z

def write_clusters(mapping, clusters, r_file_path):
    """Clusters are written to the files in result_files/ directory having the
       same name as the corresponding graph file in data_n/
    """
    clusters_map= {}
    with open(r_file_path, "w") as file_:
        for i in range(1, len(clusters)):
            if clusters[i] not in clusters_map:
                clusters_map[clusters[i]]=[mapping[i]]
            else:
                clusters_map[clusters[i]].append(mapping[i])

        for keys in clusters_map:
            file_.write("cluster-"+str(keys)+": ")
            for users in clusters_map[keys]:
                file_.write(users+",")
            file_.write("\n")

if __name__=='__main__':

    """Direcctory containing graphs-data is passed as command line argument
       , result_files/ directory is created, if doesn't exists and finally
       all the files are traversed and obtained clusters are written to the
       files in result_files/ directory
    """
    path=sys.argv[1]
    result_dir="result_files"

    if not os.path.exists(result_dir):
        os.system("mkdir "+result_dir)
    for root, dirs, files in os.walk(path):
        for file_ in files:
            file_path=os.path.join(root,file_)
            r_file_path= os.path.join(result_dir, file_)

            open(r_file_path, "a").close()
            f, dset, mapping, G = forward_map(file_path)
            if G.number_of_nodes()>0:
                conn_comps= len(list(nx.connected_component_subgraphs(G)))
                top_k= top_k_eigen_vectors(dset, conn_comps)
                clusters= find_clusters(top_k, conn_comps)
                write_clusters(mapping, clusters, r_file_path)
            f.close()

        for subdirs in dirs:
            sub_path=os.path.join(root, subdirs)
            for sub_files in sub_path:
                sub_file_path= os.path.join(sub_path, sub_files)