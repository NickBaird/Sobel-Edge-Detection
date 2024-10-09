# Sobel-Edge-Detection
A program that uses sobel filters to determine the edges of the image. Sobel filters are two filters, typically of size 3x3 or 5x5, that highlight the 'differences' in pixel values in the X and Y direction of the image. Combining the X and Y 'differences' we are able to approximate the gradient of the image. The gradient highlights the edges of the image, making it possible to take apply a some threshold value and creating a bitmap of the edges within the image.

You can view the results of this program [https://github.com/NickBaird/Sobel-Edge-Detection/tree/main/results](here), which shows images for the 3x3 and 5x5 sobel filters in both the X and Y direction, the gradients, and the edges for both low and high thresholds.
