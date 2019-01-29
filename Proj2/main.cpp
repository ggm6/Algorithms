/*
 
Name: Garrett McDonnell

 NOTE: all convex hull functions, orientation, nextToTop, swap, distSq, findSide, lineDist, and compare functions taken from "https://www.geeksforgeeks.org/passing-vector-function-cpp/", but I use my own Point class and edit algorithm to use a vector instead of an array
 	   time duration calculations taken from "https://www.geeksforgeeks.org/measure-execution-time-function-cpp/"

COMPILE: g++ -o main main.cpp

RUN: ./main J/G/Q test.txt
 
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <stack>
#include <set>
#include <stdlib.h>
#include <cmath>

using namespace std;
using namespace std::chrono;


template <class t>  // Templatization unexploited in this program (program uses int for all cases), but allows for reusability of function
class Point
{
public:

	t x, y;
	
	Point() : x((int) 0), y((int) 0) {};
	Point(t a, t b) : x(a), y(b) {};

};


void jarvisHull(vector<Point<int>> points);
void grahamHull(vector<Point<int>> points);
void quickHull(vector<Point<int>> a);
void recursiveHull(vector<Point<int>> a, vector<Point<int>>& hull, Point<int> p1, Point<int> p2, int side);  // For quickHall
bool isUnique(vector<Point<int>> v, Point<int> a);  // mimics a property of set::insert (used in geeksforgeeks's implementation) by only inserting unique points in the hull
int findSide(Point<int> p1, Point<int> p2, Point<int> p);
int lineDist(Point<int> p1, Point<int> p2, Point<int> p);
int orientation(Point<int> p, Point<int> q, Point<int> r);
Point<int> nextToTop(stack<Point<int>> &S);
int swap(Point<int> &p1, Point<int> &p2);
int distSq(Point<int> p1, Point<int> p2);
int compare(const void *vp1, const void *vp2);
void file2vec(vector<Point<int>>& v, string fileName);
void fileWrite(vector<Point<int>> v);
char* algorithmType;  // made global for use in multiple functions

// A globle point needed for  sorting points with reference 
// to  the first point Used in compare function of qsort() 
Point<int> p0;


int main(int args, char* argv[])
{
	cout << endl;
	algorithmType=argv[1];
	char* fileName=argv[2];
	vector<Point<int>> pts;
	file2vec(pts, fileName);
	if (*algorithmType=='J' || *algorithmType=='j')
		jarvisHull(pts);
	else if (*algorithmType=='G' || *algorithmType=='g')
		grahamHull(pts);
	else if (*algorithmType=='Q' || *algorithmType=='q')
		quickHull(pts);
	return 0;
}


// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point<int> p, Point<int> q, Point<int> r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
    return (val > 0)? 1: 2; // clock or counterclock wise 
}

// A utility function to find next to top in a stack 
Point<int> nextToTop(stack<Point<int>> &S) 
{ 
    Point<int> p = S.top(); 
    S.pop(); 
    Point<int> res = S.top(); 
    S.push(p); 
    return res; 
} 

// A utility function to swap two points 
int swap(Point<int> &p1, Point<int> &p2) 
{ 
    Point<int> temp = p1; 
    p1 = p2; 
    p2 = temp; 
}

// A utility function to return square of distance 
// between p1 and p2 
int distSq(Point<int> p1, Point<int> p2) 
{ 
    return (p1.x - p2.x)*(p1.x - p2.x) + 
          (p1.y - p2.y)*(p1.y - p2.y); 
} 

// A function used by library function qsort() to sort an array of 
// points with respect to the first point 
int compare(const void *vp1, const void *vp2) 
{ 
   Point<int> *p1 = (Point<int> *)vp1; 
   Point<int> *p2 = (Point<int> *)vp2; 
  
   // Find orientation 
   int o = orientation(p0, *p1, *p2); 
   if (o == 0) 
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1; 
  
   return (o == 2)? -1: 1; 
}

// Returns the side of point p with respect to line 
// joining points p1 and p2. 
int findSide(Point<int> p1, Point<int> p2, Point<int> p) 
{ 
    int val = (p.y - p1.y) * (p2.x - p1.x) - 
              (p2.y - p1.y) * (p.x - p1.x); 
  
    if (val > 0) 
        return 1; 
    if (val < 0) 
        return -1; 
    return 0; 
} 
  
// returns a value proportional to the distance 
// between the point p and the line joining the 
// points p1 and p2 
int lineDist(Point<int> p1, Point<int> p2, Point<int> p) 
{ 
    return abs ((p.y - p1.y) * (p2.x - p1.x) - 
               (p2.y - p1.y) * (p.x - p1.x)); 
} 

// Prints convex hull of a set of n points. 
void jarvisHull(vector<Point<int>> points) 
{ 

	auto start = high_resolution_clock::now();  // Start of clock cycle to measure algorithm run time

    // There must be at least 3 points 
    if (points.size() < 3) return; 
  
    // Initialize Result 
    vector<Point<int>> hull; 
  
    // Find the leftmost point and lowest point
    int l=0;
    for (int i = 0; i<points.size(); ++i) 
    {
        if (points[i].x < points[l].x) 
            l = i; 
        else if (points[i].x == points[l].x)
        {
        	if (points[i].y<points[l].y)
        		l=i;
        }
    }
    // Start from leftmost point, keep moving counterclockwise 
    // until reach the start point again.  This loop runs O(h) 
    // times where h is number of points in result or output. 
    int p=l,q;
    do
    { 
        // Add current point to result;
        hull.push_back(points[p]); 
  
        // Search for a point 'q' such that orientation(p, x, 
        // q) is counterclockwise for all points 'x'. The idea 
        // is to keep track of last visited most counterclock- 
        // wise point in q. If any point 'i' is more counterclock- 
        // wise than q, then update q. 
        q = (p+1)%points.size(); 
        for (int i = 0; i < points.size(); i++) 
        { 
           // If i is more counterclockwise than current q, then 
           // update q 
           if (orientation(points[p], points[i], points[q]) == 2) 
               q = i;
           else if (orientation(points[p], points[i], points[q]) == 0)
           {
           	   double oldMag=sqrt(pow(points[p].x-points[q].x,2)+pow(points[p].y-points[q].y,2));
	           double newMag=sqrt(pow(points[p].x-points[i].x,2)+pow(points[p].y-points[i].y,2));
	           if (newMag>oldMag)
	              q=i;
           }
        } 

  
        // Now q is the most counterclockwise with respect to p 
        // Set p as q for next iteration, so that q is added to 
        // result 'hull' 
        p = q; 
  
    } while (p != l);  // While we don't come to first point 

    auto stop=high_resolution_clock::now();
	auto duration=duration_cast<microseconds> (stop-start);  // end clock cycle and output running time in microseconds
	cout << "n = " << points.size() << "\n\n\tExecution time: " << (double) duration.count()/1000 << " milliseconds" << endl << endl;

	Point<int> leftOfHall=hull[0];
	for (auto it=hull.begin(); it!=hull.end(); ++it)
	{
		if (it->x < leftOfHall.x)
			leftOfHall=*it;
		else if (it->x == leftOfHall.x)
		{
			if (it->y < leftOfHall.y)
				leftOfHall=*it;
		}
	}
  	p0=leftOfHall;
	qsort(&hull[0], hull.size(), sizeof(Point<int>), compare);

    fileWrite(hull);


}

void grahamHull(vector<Point<int>> points)
{

	auto start = high_resolution_clock::now();  // Start of clock cycle to measure algorithm run time

   // Find the bottommost point 
   int ymin = points[0].y, min = 0; 
   for (int i = 1; i < points.size(); i++) 
   { 
     int y = points[i].y; 
  
     // Pick the bottom-most or chose the left 
     // most point in case of tie 
     if ((y < ymin) || (ymin == y && 
         points[i].x < points[min].x)) 
        ymin = points[i].y, min = i; 
   } 
  
   // Place the bottom-most point at first position 
   swap(points[0], points[min]); 
  
   // Sort n-1 points with respect to the first point. 
   // A point p1 comes before p2 in sorted ouput if p2 
   // has larger polar angle (in counterclockwise 
   // direction) than p1 
   p0 = points[0]; 
   qsort(&points[1], points.size()-1, sizeof(Point<int>), compare); 
  
   // If two or more points make same angle with p0, 
   // Remove all but the one that is farthest from p0 
   // Remember that, in above sorting, our criteria was 
   // to keep the farthest point at the end when more than 
   // one points have same angle. 
   int m = 1; // Initialize size of modified array 
   for (int i=1; i<points.size(); i++) 
   { 
       // Keep removing i while angle of i and i+1 is same 
       // with respect to p0 
       while (i < points.size()-1 && orientation(p0, points[i], 
                                    points[i+1]) == 0) 
          i++; 
  
  
       points[m] = points[i]; 
       m++;  // Update size of modified array 
   } 
  
   // If modified array of points has less than 3 points, 
   // convex hull is not possible 
   if (m < 3) return; 
  
   // Create an empty stack and push first three points 
   // to it. 
   stack<Point<int>> S; 
   S.push(points[0]); 
   S.push(points[1]); 
   S.push(points[2]); 
  
   // Process remaining n-3 points 
   for (int i = 3; i < m; i++) 
   { 
      // Keep removing top while the angle formed by 
      // points next-to-top, top, and points[i] makes 
      // a non-left turn 
      while (orientation(nextToTop(S), S.top(), points[i]) != 2) 
         S.pop(); 
      S.push(points[i]); 
   } 

   auto stop=high_resolution_clock::now();
   auto duration=duration_cast<microseconds> (stop-start);  // end clock cycle and output running time in microseconds
   cout << "n = " << points.size() << "\n\n\tExecution time: " << (double) duration.count()/1000 << " milliseconds" << endl << endl;
  
   std::vector<Point<int>> hull;
   while (!S.empty()) 
   { 
       hull.push_back(S.top()); 
       S.pop(); 
   }
   
   Point<int> leftOfHall=hull[0];
	for (auto it=hull.begin(); it!=hull.end(); ++it)
	{
		if (it->x < leftOfHall.x)
			leftOfHall=*it;
		else if (it->x == leftOfHall.x)
		{
			if (it->y < leftOfHall.y)
				leftOfHall=*it;
		}
	}
	p0=leftOfHall;
    qsort(&hull[0], hull.size(), sizeof(Point<int>), compare);

   fileWrite(hull);

}

void quickHull(std::vector<Point<int>> a)
{

	auto start = high_resolution_clock::now();  // Start of clock cycle to measure algorithm run time

	// a[i].y -> y-coordinate of the ith point 
    if (a.size() < 3) 
    { 
        cout << "Convex hull not possible\n"; 
        return; 
    } 
  
    // Finding the point with minimum and 
    // maximum x-coordinate 
    int min_x = 0, max_x = 0; 
    for (int i=1; i<a.size(); i++) 
    { 
        if (a[i].x < a[min_x].x) 
            min_x = i;
        else if (a[i].x == a[min_x].x) 
        {
        	if (a[i].y < a[min_x].y)
        		min_x=i;
        }
        if (a[i].x > a[max_x].x) 
            max_x = i;
        else if (a[i].x == a[max_x].x) 
        {
        	if (a[i].y > a[max_x].y)
        		max_x=i;
        }
    }


    vector<Point<int>> hull;

    // Recursively find convex hull points on 
    // one side of line joining a[min_x] and 
    // a[max_x] 
    recursiveHull(a, hull, a[min_x], a[max_x], 1); 
  
    // Recursively find convex hull points on 
    // other side of line joining a[min_x] and 
    // a[max_x] 
    recursiveHull(a, hull, a[min_x], a[max_x], -1);

    auto stop=high_resolution_clock::now();
    auto duration=duration_cast<microseconds> (stop-start);  // end clock cycle and output running time in microseconds
    cout << "n = " << a.size() << "\n\n\tExecution time: " << (double) duration.count()/1000 << " milliseconds" << endl << endl;

    Point<int> leftOfHall=hull[0];
	for (auto it=hull.begin(); it!=hull.end(); ++it)
	{
		if (it->x < leftOfHall.x)
			leftOfHall=*it;
		else if (it->x == leftOfHall.x)
		{
			if (it->y < leftOfHall.y)
				leftOfHall=*it;
		}
	}
	p0=leftOfHall;
    qsort(&hull[0], hull.size(), sizeof(Point<int>), compare);

    fileWrite(hull);
}

// End points of line L are p1 and p2.  side can have value 
// 1 or -1 specifying each of the parts made by the line L 
void recursiveHull(vector<Point<int>> a, vector<Point<int>>& hull, Point<int> p1, Point<int> p2, int side) 
{ 
    int ind = -1; 
    int max_dist = 0;
  
    // finding the point with maximum distance 
    // from L and also on the specified side of L. 
    for (int i=0; i<a.size(); i++) 
    { 
        int temp = lineDist(p1, p2, a[i]); 
        if (findSide(p1, p2, a[i]) == side && temp > max_dist) 
        { 
            ind = i; 
            max_dist = temp; 
        } 
    } 
  
    // If no point is found, add the end points 
    // of L to the convex hull. 
    if (ind == -1) 
    {
    	if (isUnique(hull,p1)) 
        	hull.push_back(p1); 
        if (isUnique(hull,p2))
        	hull.push_back(p2); 
        return; 
    } 
  
    // Recur for the two parts divided by a[ind] 
    recursiveHull(a, hull, a[ind], p1, -findSide(a[ind], p1, p2)); 
    recursiveHull(a, hull, a[ind], p2, -findSide(a[ind], p2, p1)); 
}

bool isUnique(vector<Point<int>> v, Point<int> a)
{
	for (auto it=v.begin(); it!=v.end(); ++it)
	{
		if (it->x==a.x && it->y==a.y)
			return false;
	}

	return true;
}

void file2vec(vector<Point<int>>& v, string fileName)
{
	ifstream file;
	file.open(fileName);
	string line;
	while (getline(file,line))
	{
		size_t pos=line.find('\t');
		string xStr=line.substr(0,pos);
		string yStr=line.substr(pos+1,line.length());
		int xInt=stoi(xStr);
		int yInt=stoi(yStr);
		Point<int> pt{xInt,yInt};
		v.push_back(pt);
	}
	file.close();
}

void fileWrite(vector<Point<int>> v)
{
	ofstream file;
	if (*algorithmType=='J' || *algorithmType=='j')
		file.open("hull_J.txt", std::ofstream::out | std::ofstream::trunc);
	else if (*algorithmType=='G' || *algorithmType=='g')
		file.open("hull_G.txt", std::ofstream::out | std::ofstream::trunc);
	else if (*algorithmType=='Q' || *algorithmType=='q')
		file.open("hull_Q.txt", std::ofstream::out | std::ofstream::trunc);
	for (auto it=v.begin(); it!=v.end(); ++it)
	{
		if (it!=v.end()-1)
			file << it->x << '\t' << it->y << endl;
		else
			file << it->x << '\t' << it->y;
	}
	file.close();

}