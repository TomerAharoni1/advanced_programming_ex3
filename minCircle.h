// 31490586 31326321

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstddef>

using namespace std;

// ------------ DO NOT CHANGE -----------
class Point {
public:
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
};

class Circle {
public:
    Point center;
    float radius;
    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

float distance(const Point &a, const Point &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

bool isPointInsideCircle(const Circle &circle, const Point &point) {
    float d = distance(circle.center, point);
    if (d <= circle.radius+1) return true;
    return false;
}

Circle circleFrom(const Point &a, const Point &b, const Point &c) {
    //* a, b, c are the points on the edge of the circle.
    float A = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    float Vsqr[] = {(a.x * a.x + a.y * a.y), (b.x * b.x + b.y * b.y),
                    (c.x * c.x + c.y * c.y)};
    float B = Vsqr[0] * (b.x * c.y - c.x * b.y) +
              Vsqr[1] * (c.x * a.y - a.x * c.y) +
              Vsqr[2] * (a.x * b.y - a.y * b.x);
    float Sx =
            (Vsqr[0] * (b.y - c.y) + Vsqr[1] * (c.y - a.y) + Vsqr[2] * (a.y - b.y)) /
            2;
    float Sy =
            (Vsqr[0] * (c.x - b.x) + Vsqr[1] * (a.x - c.x) + Vsqr[2] * (b.x - a.x)) /
            2;
    Point center = Point(Sx / A, Sy / A);
    float radius = sqrt(A * B + Sx * Sx + Sy * Sy) / fabs(A);
return {center, radius};
}

Circle circleFrom(const Point &a, const Point &b) {
    // printf("2 points circle.\n");
    // Set the center to be the midpoint of A and B
    Point point = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    float radius = distance(a, b) / 2;
    // Set the radius to be half of the distance of AB.
    return {point, radius};
}

// The function checks if a circle contains the given points:
bool isCircleValid(const Circle &center, const vector<Point> &points) {

    for (const Point &p : points)
        if (isPointInsideCircle(center, p) == false)
            return false;
    return true;
}

Circle minCircleTrivial(vector<Point> &pointsVector) {
    if (pointsVector.empty()) {
        return {{0, 0}, 0};
    } else if (pointsVector.size() == 1) {
        return {pointsVector[0], 0};
    } else if (pointsVector.size() == 2) {
        return circleFrom(pointsVector[0], pointsVector[1]);
    }

    Circle min_circle = circleFrom(pointsVector[0], pointsVector[1],
                                    pointsVector[2]);

    Circle c = circleFrom(pointsVector[0], pointsVector[1]);
    if (isCircleValid(c, pointsVector) && c.radius < min_circle.radius)
        min_circle = c;

    c = circleFrom(pointsVector[0], pointsVector[2]);
    if (isCircleValid(c, pointsVector) && c.radius < min_circle.radius)
        min_circle = c;

    c = circleFrom(pointsVector[1], pointsVector[2]);
    if (isCircleValid(c, pointsVector) && c.radius < min_circle.radius)
        min_circle = c;

    return min_circle;
}

Circle findMinCircleAux(vector<Point> &pointsVector, vector<Point> rVector,
                        int size) {
    if (size == 0 || rVector.size() == 3) return minCircleTrivial(rVector);
    // Pick random point:
    int index = rand() % size;
    Point point = pointsVector[index];


    swap(pointsVector[index], pointsVector[size - 1]);
    Circle circle2 = findMinCircleAux(pointsVector, rVector, size - 1);
    if (isPointInsideCircle(circle2, point)) return circle2;
    rVector.push_back(point);
    return findMinCircleAux(pointsVector, rVector, size - 1);
}

vector<Point> createPointsVector(Point **points, size_t size) {
    vector<Point> vec;
    for (size_t i = 0; i < size; i++)
        vec.push_back(*points[i]);
    return vec;
}

// Main function:
Circle findMinCircle(Point **points, size_t size) {
    vector<Point> pointsVector = createPointsVector(points, size);
    random_shuffle(pointsVector.begin(), pointsVector.end());
    return findMinCircleAux(pointsVector, {}, pointsVector.size());
}

#endif /* MINCIRCLE_H_ */