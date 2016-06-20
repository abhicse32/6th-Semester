#!/usr/bin/env python
__author__='Vinay Babu'
import numpy as np
import cv2,sys
from skimage.filters import gabor_kernel
from skimage.util import img_as_float
from skimage import io
from skimage import data, color, exposure
from skimage.filters import gabor_filter
from matplotlib import pyplot as plt
from scipy import ndimage as nd
import matplotlib.pyplot as plt

complex_array=[]

def showfilter(gk,st):
    if(st == 'real'):
        plt.figure()
        io.imshow(gk.real)
        io.show()
    elif(st=='imag'):
        plt.figure()
        io.imshow(gk.imag)
        io.show()
    else:
        plt.figure()
        io.imshow(gk)
        io.show()

def get_gabor(freq,ori):
    gk = gabor_kernel(frequency= (np.pi)/(2*(np.sqrt(2))**freq), theta=(ori*np.pi)/8,
                       bandwidth=1, sigma_x=2*np.pi, sigma_y=2*np.pi, n_stds=3, offset=0)
    gk /= (1.5*gk.sum())
    return gk

def get_gaborfilters(scale,orientation):
    gabor_filters= []
    for i in range(scale):
        gabr_ori = []
        for j in range(orientation):
            gk2 = get_gabor(i,j)
            gabr_ori.append(gk2)
        gabor_filters.append(gabr_ori)

    return gabor_filters

def val_cal(lst_,val):
    result=0
    for i in range(len(lst_)):
        if lst_[i]>=val:
            result+=(1<<i)
    return result

def calc_lbp(complex_array):
    #lbp calculation for magnitude values\
    len_i=len(complex_array)
    len_j=len(complex_array[0])
    lst_=[0 for val in range(8)]
    shape_= np.array(complex_array).shape
    modified_arr=[[0 for val in range(shape_[1])] for val_ in range(shape_[0])]
    for i in range(len_i):
        complex_array_i=complex_array[i]
        for j in range(len_j):
            complex_array_i_j=complex_array_i[j]
            shape= np.array(complex_array_i_j).shape
            arr_= [[0 for val in range(shape[1])] for val_ in range(shape[0])]
            len_l= len(complex_array_i_j[0])
            len_k= len(complex_array_i_j)
            for k in range(len_k):
                for l in range(len_l):
                    lst_[0] = complex_array_i[j-1][k][l]
                    lst_[1] = complex_array_i_j[k-1][l]
                    lst_[2] = complex_array[i-1][j][k][l]
                    lst_[3] = complex_array_i_j[k][(l+1)%len_l]
                    lst_[4] = complex_array_i[(j+1)%len_j][k][l]
                    lst_[5] = complex_array_i_j[(k+1)%len_k][l]
                    lst_[6] = complex_array[(i+1)%len_i][j][k][l]
                    lst_[7] = complex_array_i_j[k-1][l]

                    arr_[k][l]=val_cal(lst_,complex_array_i_j[k][l])

            modified_arr[i][j]=arr_
    return modified_arr

def histogram(gabor_filters):
    hist_lists=[]
    for rows in gabor_filters:
        for filters in rows:
            hist_=[0 for val in range(256)]
            len_= len(filters[0])
            for i in range(len(filters)):
                filters_i=filters[i]
                for j in range(len_):
                    hist_[filters_i[j]]+=1
            hist_lists.append(hist_)
    return hist_lists


def normalize(hist):
    sum_ = float(sum(hist))
    for i in range(len(hist)):
        hist[i]/=sum_
    return hist

def plot_histogram(mat1, label_):
    plt.figure()
    plt.plot([x for x in range(256)],mat1,label=label_)
    plt.xlabel('E-GV-LBP-code')
    plt.ylabel('percentage')
    plt.show()

if __name__=='__main__':
    img_path= sys.argv[1]
    v,u=5,8
    gb_comp = get_gaborfilters(v,u)
    img=cv2.imread(img_path)
    img = color.rgb2gray(img)
    for i in range(5):
        temp=[]
        for j in range(0,8):
            real=nd.convolve(img, np.real(gb_comp[i][j]),mode='constant')
            imag=nd.convolve(img, np.imag(gb_comp[i][j]),mode='constant')
            temp.append(real+1j*(imag))
        complex_array.append(temp)

    magnitude= np.absolute(complex_array)
    phase= np.angle(complex_array)
    magnitude = calc_lbp(magnitude)
    phase = calc_lbp(phase)

    magnitude_hist= histogram(magnitude)
    phase_hist= histogram(phase)

    normalized_mag_hist= normalize(magnitude_hist[5])
    normalized_pahse_hist= normalize(phase_hist[5])

    plot_histogram(normalized_mag_hist,'magnitude-histogram')
    plot_histogram(normalized_pahse_hist,'phase-histogram')
    print(magnitude_hist[0])
    print(phase_hist[0])







