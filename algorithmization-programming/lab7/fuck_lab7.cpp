// Searching point for trinagle with maximum inbound points
// by Mark Vodyanitskiy (@mvodya) 09.06.2019
#include <iostream>
#include <stack>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <vector>

struct point {
  int n;
  double x, y;
  int u, v;
  double angleU, angleV;
};
// Points of line
point pointA, pointB;
// Input stack
std::stack<point> points;
// Up & down set of points separated by line.
std::vector<point> pointsUP, pointsDOWN;
int n;

// Lenth from a to b
inline double vecAbs(point& a, point& b) {
  return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

// Vector matrix product
inline double vecX(point& a, point& b, point& c) {
  return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

// Vector scalar
inline double vecScalar(point& a, point& b, point& c) {
  return ((b.x - a.x) * (c.x - a.x)) + ((b.y - a.y) * (c.y - a.y));
}

// Angle between 3 points
inline double vecAngle(point& a, point& b, point& c) {
  return acos((vecScalar(a, b, c) / (vecAbs(a, b) * vecAbs(a, c)))) * 180.0f /
         M_PI;
}

// Counterclock-wise
inline bool cmpPointsU(point a, point b) { return a.angleU < b.angleU; }

// Clockwise
inline bool cmpPointsV(point a, point b) { return a.angleV < b.angleV; }

int main(int argc, char* argv[]) {
  // Open file
  if (argc == 1) {
    printf("You must specify path of the labirint file to be processed.\n");
    return 1;
  }
  FILE* file = fopen(argv[1], "r");
  if (!file) {
    printf("Error while opening file %s\n", argv[1]);
    return 1;
  }

  // Read file
  fscanf(file, "%d", &n);
  for (int i = 0; i < n; i++) {
    point t;
    t.n = i;
    fscanf(file, "%lf %lf", &t.x, &t.y);
    points.push(t);
  }
  fscanf(file, "%lf %lf", &pointA.x, &pointA.y);
  fscanf(file, "%lf %lf", &pointB.x, &pointB.y);

  // Prepare points
  while (!points.empty()) {
    point& top = points.top();
    top.angleU = vecAngle(pointB, pointA, top);
    top.angleV = vecAngle(pointA, pointB, top);
    if (vecX(pointA, pointB, top) > 0)
      pointsUP.push_back(top);
    else
      pointsDOWN.push_back(top);
    points.pop();
  }

  // UP
  std::sort(pointsUP.begin(), pointsUP.end(), cmpPointsU);
  int i = 0;
  for (auto& p : pointsUP) {
    p.u = i;
    i++;
  }

  // DOWN
  std::sort(pointsDOWN.begin(), pointsDOWN.end(), cmpPointsU);
  i = 0;
  for (auto& p : pointsDOWN) {
    p.u = i;
    i++;
  }

  // UP
  std::sort(pointsUP.begin(), pointsUP.end(), cmpPointsV);
  i = 0;
  for (auto& p : pointsUP) {
    p.v = i;
    i++;
  }

  // DOWN
  std::sort(pointsDOWN.begin(), pointsDOWN.end(), cmpPointsV);
  i = 0;
  for (auto& p : pointsDOWN) {
    p.v = i;
    i++;
  }

  // DEBUG OUT
  /*for (auto& p : pointsUP) {
    printf("%d) %lf %lf - (%lf / %lf) - [%d][%d/%d]\n", p.n, p.x, p.y, p.angleU,
           p.angleV, p.u + p.v, p.u, p.v);
  }
  printf("\n!!!\n");
  for (auto& p : pointsDOWN) {
    printf("%d) %lf %lf - (%lf / %lf) - [%d][%d/%d]\n", p.n, p.x, p.y, p.angleU,
           p.angleV, p.u + p.v, p.u, p.v);
  }*/

  // Answer
  std::vector<point> result;
  result.reserve(pointsUP.size() + pointsDOWN.size());
  result.insert(result.end(), pointsUP.begin(), pointsUP.end());
  result.insert(result.end(), pointsDOWN.begin(), pointsDOWN.end());
  // Searching max point
  int max = 0;
  point maxpoint;
  for (auto& p : result) {
    int t = p.u + p.v;
    if (t < max) continue;
    max = t;
    maxpoint = p;
  }
  printf("Point number: %d (%lf %lf)", maxpoint.n, maxpoint.x, maxpoint.y);
}