#include "minCircle.h"
#include <chrono>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>


using namespace std;
using namespace std::chrono;

float get_random_num() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100;
}

void check_circumscribed() {
    vector<Point> points;
    points.push_back({get_random_num(), get_random_num()});
    points.push_back({get_random_num(), get_random_num()});
    points.push_back({get_random_num(), get_random_num()});
    printf("(%f, %f), (%f,%f), (%f, %f)\n", points[0].x, points[0].y, points[1].x,
                 points[1].y, points[2].x, points[2].y);
    Circle c1 = circle_from3(points[0], points[1], points[2]);
    Circle c2 = circle_from(points[0], points[1], points[2]);
    if (fabs(c1.radius - c2.radius) > 1e-5)
        printf("wrong radius: %f instead of %f. ", c2.radius, c1.radius);
    if (distance(c1.center, c2.center) > 1e-5)
        printf("wrong center with distance of %f. center: (%f, %f) instead of: "
                     "(%f, %f) ",
                     distance(c1.center, c2.center), c2.center.x, c2.center.y,
                     c1.center.x, c1.center.y);
}

Point **generate(Point center, int R, size_t size) {
    Point **p = new Point *[size];
    for (size_t i = 0; i < size; i++) {
        float r = 1 + rand() % R;
        float a = 3.14159 * (rand() % 360) / 180;
        float x = center.x + r * cos(a);
        float y = center.y + r * sin(a);
        p[i] = new Point(x, y);
    }
    return p;
}

int main() {
    srand(time(NULL));
    const size_t N = 250;
    float R = 10 + rand() % 1000;
    float cx = -500 + rand() % 1001;
    float cy = -500 + rand() % 1001;
    Point **ps = generate(Point(cx, cy), R, N);
	bool printall = false;

    // for (int i = 0; i < 10; i++)
    // {
    // 	check_circumscribed();
    // 	printf("%i\n", i);
    // }

    // your working copy
    Point **ps_copy = new Point *[N];
    for (size_t i = 0; i < N; i++)
        ps_copy[i] = new Point(ps[i]->x, ps[i]->y);

    auto start = high_resolution_clock::now();
    Circle c = findMinCircle(ps_copy, N);
    auto stop = high_resolution_clock::now();

    if ((int)c.radius > (int)R) {
        cout << "you need to find a minimal radius (-40)" << endl;
        printf("%f instead of %f\n", c.radius, R);
    }

    bool covered = true;
	if (printall) {
		printf("----------\n");
		printf("right: %f, (%f, %f)\n", R, cx, cy);
		printf("wrong: %f, (%f, %f)\n", c.radius, c.center.x, c.center.y);
	}
    for (size_t i = 0; i < N && covered; i++) {
        float x2 = (c.center.x - ps[i]->x) * (c.center.x - ps[i]->x);
        float y2 = (c.center.y - ps[i]->y) * (c.center.y - ps[i]->y);
        float d = sqrt(x2 + y2);
        if (d > c.radius + 1)
            covered = false;
		if (printall)
        	printf("%f, %f\n", ps[i]->x, ps[i]->y);
    }
    if (!covered)
        cout << "all points should be covered (-45)" << endl;

    auto duration = duration_cast<microseconds>(stop - start);
    int stime = duration.count();
    cout << "your time: " << stime << " microseconds" << endl;
    if (stime > 3000) {
        cout << "over time limit ";
        if (stime <= 3500)
            cout << "(-5)" << endl;
        else if (stime <= 4000)
            cout << "(-8)" << endl;
        else if (stime <= 6000)
            cout << "(-10)" << endl;
        else
            cout << "(-15)" << endl;
    }

    for (size_t i = 0; i < N; i++) {
        delete ps[i];
        delete ps_copy[i];
    }
    delete[] ps;
    delete[] ps_copy;

    cout << "done" << endl;
    return 0;
}
