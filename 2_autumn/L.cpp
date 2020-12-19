#include<iostream>
#include<iomanip>
#include<cmath>
#include<vector>

using namespace std;

template<typename T>
class Point{
public:
    T x;
    T y;

    Point(T x_ = 0, T y_ = 0): x(x_), y(y_) {

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

    Point operator *= (double x_) {
        x *= x_;
        y *= x_;
        return *this;
    }

    Point operator * (double x_) const {
        Point temp = *this;
        temp *= x_;
        return temp;
    }

    bool operator == (const Point other) const {
        return x == other.x && y == other.y;
    }

    bool operator != (const Point other) const {
        return !(*this == other);
    }

    double len() {
        return sqrt(x * x + y * y);
    }

    Point norm() {
        return (*this) / (this->len());
    }
};

template<typename T>
istream& operator >> (istream &in, Point<T> &a) {
    in >> a.x >> a.y;
    return in;
}

template<typename T>
ostream& operator << (ostream &out, Point<T> a) {
    out << a.x << " " << a.y;
    return out;
}

template<typename T>
class Segment {
public:
    Point<T> first;
    Point<T> second;
    
    Segment() {};
    Segment(T a, T b, T c, T d): first(a, b), second(c, d) {};
    Segment(Point<T> a, Point<T> b): first(a), second(b) {};

    bool is_point() const {
        return first == second;
    }
};

template<typename T>
istream& operator >> (istream &in, Segment<T> &a) {
    in >> a.first >> a.second;
    return in;
}

template<typename T>
ostream& operator << (ostream &out, Segment<T> a) {
    out << a.first << " " << a.second;
    return out;
}

template<typename T>
T dot(const Point<T> &a, const Point<T> &b) {
    return a.x * b.x + a.y * b.y;
}

template<typename T>
int sign(T a) {
    if (a == 0) {
        return 0;
    }
    return (a > 0 ? 1 : -1);
}

template<typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return a.x * b.y - a.y * b.x;
}

template<typename T>
double angle(const Point<T> &a, const Point<T> &b) {

    return atan2(cross(a, b), dot(a, b)) ;
}

template<typename T>
bool Is_on_segment(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    Point<T> ab = a - b;
    Point<T> ac = a - c;
    Point<T> bc = b - c;
    return cross(ab, ac) == 0 && dot(ab, ac) >= 0 && dot(-ab, bc) >= 0;
}

bool Is_on_segment(const Point<double> &a, const Point<double> &b, 
                   const Point<double> &c) {
    Point<double> ab = a - b;
    Point<double> ac = a - c;
    Point<double> bc = b - c;
    double eps = 1e-9;
    return abs(cross(ab, ac)) < eps && dot(ab, ac) >= -eps && dot(-ab, bc) >= -eps;
}

template<typename T>
bool Is_on_segment(const Segment<T> &a, const Point<T> &c) {
    return Is_on_segment(a.first, a.second, c);
}

template<typename T>
bool Is_in_polygon(const vector<Point<T> > &polygon, const Point<T> &a) {
    if (a == polygon[0]) {
        return true;
    }
    int sz = polygon.size();
    int l = 0;
    int r = sz;
    while (r - l > 1) {
        int m = (l + r) / 2;
        Point<T> ab = a - polygon[0];
        Point<T> ac = polygon[m] - polygon[0];
        if (cross(ac, ab) < 0) {
            r = m;
        } else {
            l = m;
        }
    }
    if (l == 0 || l == sz - 1 && !Is_on_segment(polygon[0], polygon[sz - 1], a)) {
        return false;
    }
    Point<T> ab = a - polygon[0];
    Point<T> ac = polygon[(l + 1) % sz] - polygon[l];
    ab = a - polygon[l];
    ac = polygon[(l + 1) % sz] - polygon[l];
    if (cross(ac, ab) < 0) {
        return false;
    }
    return true;
}

template<typename T>
bool are_segments_cross(Segment<T> a, Segment<T> b) {
    class help {
    public:    
        bool static intersect(T a, T b, T c, T d) {
            if (a > b) swap(a, b);
            if (c > d) swap(c, d);
            return max(a, c) <= min(b, d);
        }
    };
    return help::intersect(a.first.x, a.second.x, b.first.x, b.second.x) &&
           help::intersect(a.first.y, a.second.y, b.first.y, b.second.y) &&
           sign(cross(a.first  - b.first, b.second - b.first)) * 
           sign(cross(a.second - b.first, b.second - b.first)) <= 0 && 
           sign(cross(b.first  - a.first, a.second - a.first)) *
           sign(cross(b.second - a.first, a.second - a.first)) <= 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    Point<double> c, a, b;
    double radius;
    cout << fixed << setprecision(9);
    while (cin >> c >> radius >> a >> b) {

        Point<int64_t> c_, a_, b_;
        int64_t radius_ = round(radius * 100);
        c_.x = round(c.x * 100);
        c_.y = round(c.y * 100);
        a_.x = round(a.x * 100);
        a_.y = round(a.y * 100);
        b_.x = round(b.x * 100);
        b_.y = round(b.y * 100);
        int64_t res = cross(b_ - a_, c_ - a_);
        int64_t res2 = dot(b_ - a_, b_ - a_);
        if (res4 >= 0 && res3 >= 0 && res * res - res2 * radius_ * radius_ == 0);
        int64_t res3 = cross(a_ - b_, c_ - b_);
        int64_t res4 = dot(b_ - a_, c_ - a_);

        if (radius == 0) {
            if (Is_on_segment(a, b, c)) {
                cout << 1 << "\n" << c << "\n";
            } else {
                cout << 0 << "\n";
            }
            continue;
        }
        double eps = 1e-7;
        if ((a - b).len() < eps) {
            if (abs((a - c).len() - radius) < eps) {
                cout << 1 << "\n" << a << "\n";
            } else {
                cout << 0 << "\n";
            }
            continue;
        }
        Point<double> l = a;
        Point<double> r = b;
        double eps2 = 1e-12;
        while ((r - l).len() > eps2) {
            Point<double> m1 = l + (r-l) * 1 / 3;
            Point<double> m2 = l + (r-l) * 2 / 3;
            if ((m1 - c).len() < (m2 - c).len()) {
                r = m2;
            } else {
                l = m1;
            }
        }
        //cout << l << endl << r << endl;
        Point<double> d = l;
        if (abs((d - c).len() - radius) < eps) {
            cout << 1 << "\n" << l << "\n";
        } else if ((d - c).len() - radius > eps) {
            cout << 0 << "\n";
        } else {
            l = d;
            r = b;
            while ((r - l).len() > eps2) {
                Point<double> m = (l + r) / 2;
                //cout << l << " " << r << " " << m << " " << (m - c).len() << endl;
                if ((m - c).len() > radius) {
                    r = m;
                } else {
                    l = m;
                }
            }
            Point<double> first = l;

            l = d;
            r = a;
            while ((r - l).len() > eps2) {
                Point<double> m = (l + r) / 2;
                if ((m - c).len() > radius) {
                    r = m;
                } else {
                    l = m;
                }
            }
            Point<double> second = l;
            if (first.x - second.x > eps) {
                swap(first, second);
            } else if (abs(first.x - second.x) < eps && 
                       first.y - second.y > eps) {
                swap(first, second);
            }
            if ((first - c).len() - radius < -eps) {
                cout << 1 << "\n" << second << "\n";
            } else if ((second - c).len() - radius < -eps) {
                cout << 1 << "\n" << first << "\n";
            } else if ((a - b).len() < eps) {
                cout << 1 << "\n" << first << "\n";
            } else {
                cout << 2 << "\n" << first << "\n" << second << "\n";
            }
        }
    }
}
