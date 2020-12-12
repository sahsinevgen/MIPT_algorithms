#include<iostream>
#include <iomanip>
#include<cmath>

using namespace std;

class Point{
public:
    double x;
    double y;

    Point(double x_ = 0, double y_ = 0): x(x_), y(y_) {

    }

    Point(const Point &other): x(other.x), y(other.y) {

    }

    Point operator = (const Point &other) {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }

    Point operator += (const Point &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator + (const Point &other) {
        Point temp = *this;
        temp += other;
        return temp;
    }

    Point operator -= (const Point &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point operator - (const Point &other) {
        Point temp = *this;
        temp -= other;
        return temp;
    }

    Point operator /= (double x_) {
        x /= x_;
        y /= x_;
        return *this;
    }

    Point operator / (double x_) {
        Point temp = *this;
        temp /= x_;
        return temp;
    }

    double len() {
        return x * x + y * y;
    }

    Point norm() {
        return (*this) / (this->len());
    }
};

double dot(Point &a, Point &b) {
    return a.x * b.x + a.y * b.y;
}

double cross(Point &a, Point &b) {
    return a.x * b.y - a.y * b.x;
}

double angle(Point &a, Point &b) {

    return atan2(cross(a, b), dot(a, b)) ;
}

int main() {
    Point a, b;
    cin >> a.x >> a.y >> b.x >> b.y;
    cout << fixed << setprecision(6) << abs(angle(a, b)) << endl;

}