import cv2;
import numpy as np;
import matplotlib.pyplot as plt

def filter_color(image_name = 'DelFly_tulip.jpg', y_low = 50, y_high = 200, \
                 u_low = 120, u_high = 130, v_low = 120, v_high = 130, resize_factor=1):
    im = cv2.imread(image_name);
    im = cv2.resize(im, (int(im.shape[1]/resize_factor), int(im.shape[0]/resize_factor)));
    YUV = cv2.cvtColor(im, cv2.COLOR_BGR2YUV);
    Filtered = np.zeros([YUV.shape[0], YUV.shape[1]]);
    for y in range(YUV.shape[0]):
        for x in range(YUV.shape[1]):
            if(YUV[y,x,0] >= y_low and YUV[y,x,0] <= y_high and \
               YUV[y,x,1] >= u_low and YUV[y,x,1] <= u_high and \
               YUV[y,x,2] >= v_low and YUV[y,x,2] <= v_high):
                Filtered[y,x] = 1;
    
    plt.figure();
    RGB = cv2.cvtColor(im, cv2.COLOR_BGR2RGB);
    plt.imshow(RGB);
    plt.title('Original image');
    
    plt.figure()
    plt.imshow(Filtered);
    plt.title('Filtered image');
    plt.show();