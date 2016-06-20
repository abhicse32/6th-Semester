#!/usr/bin/env python
import sys, math, Image
import os

def Distance(p1,p2):
    dx = p2[0] - p1[0]
    dy = p2[1] - p1[1]
    return math.sqrt(dx*dx+dy*dy)


def ScaleRotateTranslate(image, angle, center = None, new_center = None, scale = None, resample=Image.BICUBIC):
    if (scale is None) and (center is None):
        return image.rotate(angle=angle, resample=resample)
    nx,ny = x,y = center
    sx=sy=1.0
    if new_center:
        (nx,ny) = new_center
    if scale:
        (sx,sy) = (scale, scale)
    cosine = math.cos(angle)
    sine = math.sin(angle)
    a = cosine/sx
    b = sine/sx
    c = x-nx*a-ny*b
    d = -sine/sy
    e = cosine/sy
    f = y-nx*d-ny*e
    return image.transform(image.size, Image.AFFINE, (a,b,c,d,e,f), resample=resample)


def CropFace(image, eye_left=(0,0), eye_right=(0,0), offset_pct=(0.2,0.2), dest_sz = (70,70)):
    offset_h = math.floor(float(offset_pct[0])*dest_sz[0])
    offset_v = math.floor(float(offset_pct[1])*dest_sz[1])
    # get the direction
    eye_direction = (eye_right[0] - eye_left[0], eye_right[1] - eye_left[1])
    # calc rotation angle in radians
    rotation = -math.atan2(float(eye_direction[1]),float(eye_direction[0]))
    # distance between them
    dist = Distance(eye_left, eye_right)
    # calculate the reference eye-width
    reference = dest_sz[0] - 2.0*offset_h
    # scale factor
    scale = float(dist)/float(reference)
    # rotate original around the left eye
    image = ScaleRotateTranslate(image, center=eye_left, angle=rotation)
    # crop the rotated image
    crop_xy = (eye_left[0] - scale*offset_h, eye_left[1] - scale*offset_v)
    crop_size = (dest_sz[0]*scale, dest_sz[1]*scale)
    image = image.crop((int(crop_xy[0]), int(crop_xy[1]), int(crop_xy[0]+crop_size[0]), int(crop_xy[1]+crop_size[1])))
    # resize it
    image = image.resize(dest_sz, Image.ANTIALIAS)
    return image

def read_coords(coords_file):
    eye_locations={}
    with open(coords_file,"r") as file_:
        for lines in file_:
            list_=lines.split()
            eye_locations[list_[0]]=[float(val) for val in list_[1:5]]
    return eye_locations

if __name__ == "__main__":
    path=sys.argv[1]
    coords_path=sys.argv[2]
    eye_coords_dict= read_coords(coords_path)
    #print(len(eye_coords_dict))
    cropped_path= "./cropped_images"
    if not os.path.exists(cropped_path):
        os.system("mkdir "+cropped_path)

    lst= os.listdir(path)
    #print(len(lst))
    for root, dirs, files in os.walk(path):
        count=0
        for file_ in files:
            file_path=os.path.join(root, file_)
            image =  Image.open(file_path)
            dict_file=file_[:-4]
            print(dict_file)
            if dict_file in eye_coords_dict:
                coords= eye_coords_dict[dict_file]
                cropped_file_path=os.path.join(cropped_path,file_)
                #CropFace(image, eye_left=(coords[0:2]), eye_right=(coords[2:4]), offset_pct=(0.1,0.1), dest_sz=(200,200)).save("arnie_10_10_200_200.jpg")
                CropFace(image, eye_left=(coords[:2]), eye_right=(coords[2:4]), offset_pct=(0.2,0.2),
                         dest_sz=(200,200)).save(cropped_file_path)
                #CropFace(image, eye_left=(252,364), eye_right=(420,366), offset_pct=(0.3,0.3), dest_sz=(200,200)).save("arnie_30_30_200_200.jpg")
                #CropFace(image, eye_left=(252,364), eye_right=(420,366), offset_pct=(0.2,0.2)).save("arnie_20_20_70_70.jpg")
    for subdirs in dirs:
        sub_path= os.path.join(root,subdirs)