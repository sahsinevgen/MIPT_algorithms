#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>

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

    T sqr_of_len() const {
        return x * x + y * y;
    }

    double len() const {
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

template<typename T>
bool cmp_x(Point<T> a, Point<T> b) {
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

template<typename T>
bool cmp_y(Point<T> a, Point<T> b) {
    return a.y < b.y || a.y == b.y && a.x < b.x;
}

Point<int64_t> q[200000];
Point<int64_t> temp[200000];
Point<int64_t> w[200000];

int64_t get_ans(int l, int r) {
    int64_t ans;
    if (r - l < 5) {
        sort(q + l, q + r, cmp_y<int64_t>);
        ans = (q[l] - q[l + 1]).sqr_of_len();
        for (int i = l; i < r; i++) {
            for (int j = l; j < i; j++) {
                ans = min(ans, (q[i] - q[j]).sqr_of_len());
            }
        }
        return ans;
    }

    int m = (l + r) / 2;
    int mx = q[m].x;
    ans = min(get_ans(l, m), get_ans(m, r));
    merge(q + l, q + m, 
          q + m, q + r, 
          temp, cmp_y<int64_t>);
    int w_sz = 0;
    for (int i = l; i < r; i++) {
        q[i] = temp[i - l];
        if (ans > (q[i].x - mx) * (q[i].x - mx)) {
            w[w_sz++] = q[i];
        }
    }
    for (int i = 0; i < w_sz; i++) {
        for (int j = max(i - 8, 0); j < i; j++) {
            ans = min(ans, (w[i] - w[j]).sqr_of_len());
        }
    }
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> q[i];
    }
    sort(q, q + n, cmp_x<int64_t>);
    cout << fixed << setprecision(8) << sqrt(get_ans(0, n)) << endl;
}
