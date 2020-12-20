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
vector<Point<T> > Jarvis(vector<Point<T> > points) {
    int n = points.size();
    vector<int> ans(1);
    ans[0] = 0;
    for (int i = 0; i < n; i++) {
        if (points[ans[0]].y > points[i].y || 
            points[ans[0]].y == points[i].y && points[ans[0]].x > points[i].x) {
            ans[0] = i;
        }
    }
    while (true) {
        int next_id = (ans.back() + 1) % n;
        for (int i = 0; i < n; i++) {
            if (cross(points[i] - points[ans.back()], 
                      points[next_id] - points[ans.back()]) > 0) {
                next_id = i;
            } else if (cross(points[i] - points[ans.back()],
                             points[next_id] - points[ans.back()]) == 0 &&
                             Is_on_segment(points[ans.back()], 
                                           points[i], 
                                           points[next_id]) &&
                             points[i] != points[next_id]) {
                //next_id = i;
            }
        }
        if (next_id == ans[0]) {
            break;
        }
        ans.push_back(next_id);
    }
    vector<Point<T> > real_ans(ans.size());
    for (int i = 0; i < ans.size(); i++) {
        real_ans[i] = points[ans[i]];
    }
    return real_ans;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n, r;
    cin >> n >> r;
    vector<Point<int> > points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i];
    }
    points = Jarvis(points);
    double ans = 2 * M_PI * r;
    for (int i = 0; i < points.size(); i++) {
        ans += (points[i] - points[(i + 1) % points.size()]).len();
    }
    cout << fixed << setprecision(8) << ans << endl;
}
