# Convex Hull

Name: Garrett McDonnell
Algorithms Project 2

How to Run

1. Make sure both ConvexHall_GUI.java and my main.cpp files are in the same folder when you compile and run.
2. In ConvexHull_GUI.java, in main(), make sure boolean generateData is set to true to write test.txt.
3. Make sure for ConvexHull_GUI.java, in main(), testFileGen() has the desired shape and number of points set for writing to file.
4. To begin, cd into folder containing the files and compile the java file at linux command line using: 
		javac ConvexHull_GUI.java
   To run:
   		java ConvexHull_GUI
5. Now, compile main.cpp:
		g++ -o main main.cpp
   To run (using J,G, or Q depending on which algorithm to use on the data set):
   		./main J test.txt
6. Now, edit the java file to set generateData to false, find String hullFile in main() and change "hull.txt" to "hull_Q.txt", or whichever algorithm you chose
7. Recompile and rerun:
		javac ConvexHull_GUI.java
		java ConvexHull_GUI.java


Explanation

I use the Jarvis March, Graham Scan, and Quickhull Algorithm implementations from GeeksforGeeks.  I wrote my own Point class to use with these functions.  I also
reconfigured the implementations to accept STL vectors of Point objects, instead of arrays or sets.  I also altered the implementations so that only the extremes 
of each vertex of the hull are added to the hull.  So, for instance, a rectangular hull can only contain 4 points, any points that sit on the 4 lines are not added
to the hull.  After building the hull, I sort it starting with the bottommost and leftmost point using qsort(), so that all 3 algorithms have identical hulls in
the same order.  I wrote a function called file2vec that takes a filename by string and a vector.  It reads the file line by line, creates new Point objects from it, 
and adds them to the vector.  I also have a fileWrite function that takes a vector of Point objects and writes it to a text file.  For recording the running time 
of each algorithm, I start the timer at the start of each algorithm's function and end it where the GeeksforGeeks implementation ends.  This is when the algorithm 
itself is complete, before I sort the Points of the hull or write the hull to the file.  I have a global char* variable algorithmType, which takes the 2nd command 
line argument J, G, or Q.
