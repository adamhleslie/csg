README.md
This file explains how to use and run the program. To read our report see the file called Report.pdf


To Run:
Change your dicrectory to be the csg directory.

Make sure you have a file with the proper language details below to define shapes. It needs to be called "file.csg" and be in the csg folder.
Feel free to use some of the inputs we have already defined in our report.

Run the command "make debug"
After that is complete, run the command "make"


Language details:
The language utilizes Polish notation

Here is the description of how to write a primitive:

Cylinder:
[C, xposition, yposition, zposition, xrotation degrees, yrotation degrees, zrotation degrees, number of points for circle, height, radius, color]

Cone:
[K, xposition, yposition, zposition, xrotation degrees, yrotation degrees, zrotation degrees, number of points for circle, height, radius, color]

Sphere: (number of extensions should be 3 for fast rendering)
[S, xposition, yposition, zposition, xrotation degrees, yrotation degrees, zrotation degrees, number of extensions, radius, color]

Diamond:
[D, xposition, yposition, zposition, xrotation degrees, yrotation degrees, zrotation degrees, length width, height, color]

Rectangular Prism
[R, xposition, yposition, zposition, xrotation degrees, yrotation degrees, zrotation degrees, length width, height, color]

Trianglular Prism:
[T, xposition, yposition, zposition, xrotation degrees, yrotation degrees, zrotation degrees, length width, height, color]


Color values:
0 = Red
1 = Green
2 = Blue
3 = Grey
4 = White

Binary operations:
"+" = Union 
"^" = Intersection
"-" = Difference

Look to the file.csg in the in the repository to see an example of how these are used.
Look in the images directory for more examples of runnable inputs.


