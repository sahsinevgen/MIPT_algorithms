#include<iostream>
#include<iomanip>
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

    Point operator + (const Point &other) const {
        Point temp = *this;
        temp += other;
        return temp;
    }

    Point operator -= (const Point &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point operator - (const Point &other) const {
        Point temp = *this;
        temp -= other;
        return temp;
    }

    Point operator - () const {
        Point temp = *this;
        temp.x *= -1;
        temp.y *= -1;
        return temp;
    }

    Point operator /= (double x_) {
        x /= x_;
        y /= x_;
        return *this;
    }

    Point operator / (double x_) const {
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

istream& operator >> (istream &in, Point &a) {
    in >> a.x >> a.y;
    return in;
}

ostream& operator << (ostream &out, Point a) {
    out << a.x << " " << a.y;
    return out;
}

double dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

double angle(const Point &a, const Point &b) {

    return atan2(cross(a, b), dot(a, b)) ;
}

bool Is_on_segment(const Point &a, const Point &b, const Point &c) {
    Point ab = a - b;
    Point ac = a - c;
    Point bc = b - c;
    return cross(ab, ac) == 0 && dot(ab, ac) >= 0 && dot(-ab, bc) >= 0;
}

int main() {
    Point a, b, c;
    cin >> c >> a >> b;
    cout << (Is_on_segment(a, b, c) ? "YES"
                                    : "NO") << endl;

}