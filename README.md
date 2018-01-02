# image-filter
A program that applies filters to bmp files

![University of Toronto](https://upload.wikimedia.org/wikipedia/en/thumb/9/9a/UofT_Logo.svg/1280px-UofT_Logo.svg.png)


#### **Table of Contents**
1. Description
2. Setup
3. Usage


### **Description**
This is an assignment from the University of Toronto's Computer Science program. It is the third assignment from the course
CSC209. This assignment transforms bmp images by applying filters on image. This assignment is comprised of two parts:
1. Five image filters (copy, greyscale, gaussian blur, edge detection and scale). These filters reads a bmp file from standard in and prints out the image with the filter applied to standard out. 
2. A program that call processes to execute each specified filter given as an argument. The image name is passed in as the first parameter and the output file is passed in as the second parameter. 

For the first part, Since images are read in from standard in and the new image is printed using standard out, we use redirection when calling the program. For example:

$./gaussian_blue < dog.bmp > dog_copy.bmp

Using unix commands, we are able to apply a series of filters to one image using pipes. For example, the following applies
the filter edge_detection and scale with a factor of 2 to the image dog.bmp and saves the new image as dog_new.bmp:

$./edge_detection < dog.bmp | ./scale 2 > dog_new.bmp

The second part of the assignment removes the need for redirection and piping. Instead, the program implements reading from the file and using multiple processes and pipes to apply the specified filters. For example, the following produces the same image as the above example:

$./image_filter dog.bmp dog_new.bmp ./edge_detection "./scale 2"

The following is the structure for image_filter

$./iamge_filter <image name> <new file name> [filter]...
  
Note: If no filter is specified, the copy filter is applied.

There is a lot of detail required in this assignment. See http://www.teach.cs.toronto.edu/~csc209h/fall/assignments/a3.html for additional detail.

Final Mark: 96.43%. <br />
Note: Some code was starter code written by the staff of the University of Toronto.

### **Setup**
1. Clone this repo
2. $make


### **Usage**

