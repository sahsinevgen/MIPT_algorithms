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

    bool operator == (const Point other) const {
        return x == other.x && y == other.y;
    }

    bool operator != (const Point other) const {
        return !(*this == other);
    }

    double len() {
        return x * x + y * y;
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
T dot(const Point<T> &a, const Point<T> &b) {
    return a.x * b.x + a.y * b.y;
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
        //cout << "\n" << m << "\n" << ab << "\n" << ac << "\n" << cross(ac, ab) << endl;
        if (cross(ac, ab) < 0) {
            r = m;
        } else {
            l = m;
        }
    }
    //cout << l << endl;
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

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<Point<int64_t> > polygon(n);
    for (int i = 0; i < n; i++) {
        cin >> polygon[i];
    }
    int cnt = 0;
    for (int i = 0; i < m; i++) {
        Point<int64_t> a;
        cin >> a;
        cnt += Is_in_polygon(polygon, a);
        //cout << (Is_in_polygon(polygon, a) ? "YES" : "NO") << endl;
    } 
    cout << (cnt >= k ? "YES\n" : "NO\n");
}
