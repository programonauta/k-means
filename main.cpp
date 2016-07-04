/*

   Implementation of k-means clustering algorithm

   Forked from  David Robison repository at 
              https://github.com/drobison/k-means


   Ricardo Brandao 

   Logbook

   jul/01    repo forked
             <math.h> included due sqrt function

*/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <math.h>

using namespace std;

ifstream infile;
ofstream outfile;

// Class to represent points 

class Point {

  public:
    double x, y;
    int centroidAssigned; // centroid assigned to point
    
    // Constructor with default values 0.0, 0.0
    Point(double x=0.0, double y=0.0):x(x),y(y) {};

    // Method to calculate the distance to another point

    double dist(Point other) {
      double xd = x - other.x;
      double yd = y - other.y;
      return sqrt(xd*xd + yd*yd);
    };
};

// Operator << 
ostream& operator<< (ostream&out, const Point& p) {
  out << "( " << p.x << ", " << p.y << " )";
  return out;
}

Point items[1000];
Point centroid[10];
Point oldCentroid[10];

int k = 4;   // k = number of centroids
int centroidCount[10];  // number of items by centroid
int dataCount = 0;  // dataCount = number of lines input.txt

void assignCentroid(Point* item, int point);
void calculateNewCentroid();

int main()
{
  string input = "input.txt";
  string output = "output.txt";

  double minX = 9.99e+20, minY = 9.99e+20;
  double maxX = 0, maxY = 0;

  dataCount = 0;

  outfile.open(output.c_str());

  // Read in input.
  infile.open(input.c_str());
  if (!infile)
  {
    cout << "Unable to open input."<< endl;
  }
  while(!infile.eof())		
  {
    infile >> items[dataCount].x >> items[dataCount].y;
    if (items[dataCount].x > maxX) maxX = items[dataCount].x;
    if (items[dataCount].x < minX) minX = items[dataCount].x;
    if (items[dataCount].y > maxY) maxY = items[dataCount].y;
    if (items[dataCount].y < minY) minY = items[dataCount].y;
    dataCount++;
  }

  // Close input file
  infile.close();

  srand (time(NULL));

  // Chose initial centroids.  Hard coding for testing but can/should be random per requirements.
  for( int i=0; i<k; i++)
  {
    centroid[i].x = (double)(rand() % (((int)(maxX * 1000) - (int)(minX * 1000)) + (int)(minX * 1000))) / 1000;
    centroid[i].y = (double)(rand() % (((int)(maxY * 1000) - (int)(minY * 1000)) + (int)(minY * 1000))) / 1000;

    // For debugging
    cout << "Centroid " << i << ", "<< centroid[i] << endl;
    outfile << "Centroid " << i << ", " << centroid[i] << endl;
  }

  // Read all points and calculate the closest centroid
  // Assign points to centroids based on closest mean
  for( int i=0; i<dataCount; i++)
  {
    assignCentroid( &items[i], i );
  }

  // Show items by centroid
  for( int i=0; i<k; i++)
  {
    cout << "Number of items centroid " << i << ": " << centroidCount[i] << endl;
  }
  // Open output

  for( int i=0; i<4; i++)
  {
    calculateNewCentroid();
    for( int i=0; i<dataCount; i++)
    {
      assignCentroid( &items[i], i );
    }
  }

  outfile.close();

}

void assignCentroid(Point* item, int point)
{
  double smallest = 999;
  int chosenCentroid = k+1;

  // iterate all centroids (k)
  for( int i=0; i<k; i++)
  {
    // calculate distance from point item to centroid[i]
    double distanceToCentroid = item->dist(centroid[i]);

    if( distanceToCentroid < smallest )
    {
      smallest = distanceToCentroid;
      chosenCentroid = i;
    }
  }

  item->centroidAssigned = chosenCentroid;
  centroidCount[chosenCentroid]++;

}

void calculateNewCentroid()
{

  // iterate centroids (k = number of centroids)
  for( int i=0; i<k; i++)
  {
    cout << endl;
    outfile << endl;

    // save old k th centroid
    oldCentroid[i] = centroid[i];

    double xsum = 0;
    double ysum = 0;
    double count = 0;

    // Calculate center of mass of centroid k, calculating average of x and y
    // Iterate items
    for( int j=0; j < dataCount; j++)
    {
      // verifiy if item[j] was assigned to centroid k
      if(items[j].centroidAssigned == i)
      {
        xsum += items[j].x;
        ysum += items[j].y;
        count++;
      }
    }

    // Calculate new coordinate of centroid i
    centroid[i].x = xsum / count;
    centroid[i].y = ysum / count;

    // Calculate distance btw new centroid to old centroid
    double movement = centroid[i].dist(oldCentroid[i]);

    cout << "New Centroid " << i << ":" << centroid[i] << endl;
    outfile << "New Centroid " << i << ":" << centroid[i] << endl;
    cout << "Centroid moved " << movement << endl;
  }
}
