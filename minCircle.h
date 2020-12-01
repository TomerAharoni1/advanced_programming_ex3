// ID1	ID2

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <math.h>
#include <algorithm>

using namespace std;
//* Einat's comments are with this * so that you know to look for them.

// ------------ DO NOT CHANGE -----------
class Point{
public:
	float x,y;
	Point(float x,float y):x(x),y(y){}
};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

// you may add helper functions here
float distance(const Point& a, const Point& b) {
    return sqrt(pow(a.x - b.x, 2)
                + pow(a.y - b.y, 2));
}

bool isPointInsideCircle(const Circle& circle, const Point& point) {
    if(distance(circle.center, point) <= circle.radius) return true;
    else return false;
}

// The following two functions are used
// To find the equation of the circle when
// three points are given.

// Helper method to get a circle defined by 3 points
Point getCircleCenter(float bx, float by,
                        float cx, float cy) { // why like this?
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return { (cy * B - by * C) / (2 * D),
             (bx * C - cx * B) / (2 * D) };
}

// Function to return a unique circle that
// intersects three points
//* this is the old method which gives wrong answers
//* according to my testings
Circle circle_from3(const Point& a, const Point& b,
                   const Point& c) // complex, needs understanding and
                   // change
{
    float e = b.x * b.x + b.y * b.y;
    float f = c.x * c.x + c.y * c.y;
    float g = b.x * c.y - b.y * c.x;
    Point center = { (c.y * e - b.y * f) / (2 * g),(b.x * f - c.x * e) / (2 * g) };
    center.x = center.x + a.x;
    center.y = center.y + a.y;
    return {center, distance(center, a)};
}

//* the source of those functions (go to the end): https://he.wikipedia.org/wiki/%D7%9E%D7%A2%D7%92%D7%9C_%D7%97%D7%95%D7%A1%D7%9D#%D7%94%D7%A7%D7%95%D7%98%D7%A8_%D7%95%D7%94%D7%9E%D7%A8%D7%9B%D7%96_%D7%A9%D7%9C_%D7%94%D7%9E%D7%A2%D7%92%D7%9C_%D7%94%D7%97%D7%95%D7%A1%D7%9D
Circle circle_from(const Point& a, const Point& b, const Point& c) {
    //* a, b, c are the points on the edge of the circle.
    float A = (a.x * b.y) + (a.y * c.x) + (b.x * c.y) - (c.x * b.y) - (a.y * b.x) - (a.x * c.y);
    float Vsqr[] = {(a.x * a.x + a.y * a.y), (b.x * b.x + b.y * b.y), (c.x * c.x + c.y * c.y)};
    float B = Vsqr[0] * (b.x * c.y - c.x * b.y) + Vsqr[1] * (c.x * a.y - a.x * c.y) + Vsqr[2] * (a.x * b.y - a.y * b.x);
    float Sx = (Vsqr[0] * (b.y - c.y) + Vsqr[1] * (c.y - a.y) + Vsqr[2] * (a.y - b.y)) / 2;
    float Sy = (Vsqr[0] * (c.x - b.x) + Vsqr[1] * (a.x - c.x) + Vsqr[2] * (b.x - a.x)) / 2;
    Point center = Point(Sx / A, Sy / A);
    float radius = sqrt(A * B + Sx * Sx + Sy * Sy) / A;
    return {center, radius};
}

// Function to return the smallest circle
// that intersects 2 points
Circle circle_from(const Point& a, const Point& b) {
    // Set the center to be the midpoint of A and B
    Point point = { (a.x + b.x) / 2, (a.y + b.y) / 2 };
    float radius = distance(a, b) / 2;
    // Set the radius to be half the distance AB
    return { point, radius};
}

// Function to check whether a circle
// encloses the given points
bool is_valid_circle(const Circle& center,
                     const vector<Point>& points)
{

    // Iterating through all the points
    // to check  whether the points
    // lie inside the circle or not
    for (const Point& p : points)
        if (!isPointInsideCircle(center, p))
            return false;
    return true;
}

// Function to return the minimum enclosing
// circle for N <= 3
Circle min_circle_trivial(vector<Point>& pointsVector)
{
//    assert(pointsVector.size() <= 3);
    if (pointsVector.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (pointsVector.size() == 1) {
        return { pointsVector[0], 0 };
    }
    else if (pointsVector.size() == 2) {
        return circle_from(pointsVector[0], pointsVector[1]);
    }

    // To check if MEC can be determined
    // by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = circle_from(pointsVector[i], pointsVector[j]);
            if (is_valid_circle(c, pointsVector))
                return c;
        }
    }
    return circle_from(pointsVector[0], pointsVector[1], pointsVector[2]);
}

// Returns the MEC using Welzl's algorithm
// Takes a set of input points P and a set R
// points on the circle boundary.
// n represents the number of points in P
// that are not yet processed.
Circle findMinCircleAux(vector<Point>& pointsVector,
                        vector<Point> rVector, int size) {
    // Base case when all points processed or |R| = 3
    if (size == 0 || rVector.size() == 3) {
        return min_circle_trivial(rVector);
    }

    // Pick a random point randomly
    int index = rand() % size;
    Point point = pointsVector[index];

    // Put the picked point at the end of P
    // since it's more efficient than
    // deleting from the middle of the vector
    swap(pointsVector[index], pointsVector[size - 1]);

    // Get the MEC circle d from the
    // set of points P - {p}
    Circle circle2 = findMinCircleAux(pointsVector, rVector, size - 1);

    // If d contains p, return d
    if (isPointInsideCircle(circle2, point)) {
        return circle2;
    }

    // Otherwise, must be on the boundary of the MEC
    rVector.push_back(point);

    // Return the MEC for P - {p} and R U {p}
    return findMinCircleAux(pointsVector, rVector, size - 1);
}

vector<Point> createPointsVector(Point** points, size_t size) {
    vector<Point> vec;
    for (size_t i = 0; i < size; i++)
        vec.push_back(*points[i]);
    return vec;
}

// Main function:
Circle findMinCircle(Point** points, size_t size){
    //* for some reason the commented line didn't work, so I wrote an helpout
    //* method which does the same job
    // vector<Point> pointsVector(points, points + size); // is it correct?
    vector<Point> pointsVector = createPointsVector(points, size);
    random_shuffle(pointsVector.begin(), pointsVector.end());
    return findMinCircleAux(pointsVector, {}, pointsVector.size());
}

#endif /* MINCIRCLE_H_ */
