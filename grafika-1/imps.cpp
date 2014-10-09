#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const int MAX_CONTROLL_POINTS = 10;
const int MAX_SUBDIVISION_ITERATIONS = 5;

//2^(k-1) * (2n-1) - (k-1) : n = MAX_CONTROLL_POINRS, k = iterations
const int MAX_SUBDIVISION_POINTS = 300;

//--------------------------------------------------------
// 2D Vektor
//--------------------------------------------------------
struct Vector2D {
    float x, y;
    
    Vector2D() {
        x = y = 0;
    }
    
    Vector2D(float x, float y): x(x), y(y) {}
    
    Vector2D operator+(const Vector2D& v) {
        return Vector2D(x + v.x, y + v.y);
    }
    Vector2D operator-(const Vector2D& v) {
        return Vector2D(x - v.x, y - v.y);
    }
    Vector2D operator+=(const Vector2D& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2D operator-=(const Vector2D& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    
    //Skalar szorzat
    Vector2D operator*(const float a) {
        return Vector2D(x*a,y*a);
    }
    
    float abs() { return sqrtf(x * x + y * y); }
};

//--------------------------------------------------------
// 2D Pont
//--------------------------------------------------------
struct Point2D {
    float x,y;
    
    Point2D() {
        x = y = 0;
    }
    
    Point2D(float x, float y) : x(x), y(y) {}
    
    Point2D(Vector2D v) : x(v.x), y(v.y) {}
    
    float dist(Point2D p) {
        return sqrtf( (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y));
    }
    
    //Translation
    Point2D operator+(const Vector2D& v) {
        return Point2D(x*v.x, y*v.y);
    }
    
    //Vector
    Vector2D operator-(const Point2D& p) {
        return Vector2D(x-p.x, y-p.y);
    }
    
    //Which has lower coordinates
    bool operator <(const Point2D& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    
    bool operator ==(const Point2D& p) const {
        return x == p.x && y == p.y;
    }
    
    Vector2D vectorFromOrigo(){
        return Vector2D(x,y);
    }
    
    Point2D toGlCoordinates(float screenWidthf, float screenHeightf) {
        return Point2D((x - screenWidthf/2.0f) / (screenWidthf/2.0f), (screenHeightf/2.0f - y) / (screenHeightf/2.0f));
    }
    
    Point2D toGlCoordinates(float screenWidthf, float screenHeightf, Vector2D tranVect) {
        return (*this + tranVect).toGlCoordinates(screenWidthf, screenHeightf);
    }
    
    Point2D move(const Vector2D& v) {
        return Point2D(x+v.x, y+v.y);
    }
    
    float crossProduct(const Point2D &p1, const Point2D &p2)
    {
        return (p1.x - x) * (p2.y - y) - (p1.y - y) * (p2.x - x);
    }
    
    Point2D average(const Point2D& p){
        return Point2D((x+p.x)/2,(y+p.y)/2);
    }
};

//--------------------------------------------------------
// ControllPoints
//--------------------------------------------------------
struct ControllPoints {
    Point2D points[MAX_CONTROLL_POINTS];
    int size = 0;
    
    bool add(Point2D p){
        //Max 10
        if(size>=MAX_CONTROLL_POINTS)
            return false;
        points[size++] = p;
        return true;
    }
};

//--------------------------------------------------------
// Curve
//--------------------------------------------------------
class Curve {
protected:
    ControllPoints* cp;
    
public:
    virtual Vector2D r(float t) = 0;
    void setControllPoints(ControllPoints *cp) {
        this->cp = cp;
    }
};

//--------------------------------------------------------
// Brezier curve
//--------------------------------------------------------
class BrezierCurve : public Curve {
    int nCk (int n, int k) {
        if (k > n) return 0;
        if (k == 0) return 1;
        
        int temp = n;
        for(int i = 2; i <= k; i++)  {
            temp *= (n - i+1);
            temp /= i;
        }
        return temp;
    }
    
    float B(int i, float t){
        int n = cp->size - 1;
        return nCk(n,i)*powf(t, i)* powf((1-t),n-i);
    }
    
public:
    Vector2D r(float t) {
        Vector2D r;
        for(int i = 0; i < cp->size; i++)
            r += cp->points[i].vectorFromOrigo() * B(i,t);
        return r;
    }
};

//--------------------------------------------------------
// Convex Hull (Monotone chain algorithm - Wikipedia)
//--------------------------------------------------------
class ConvexHull {
    ControllPoints* cp;
    Point2D points[MAX_CONTROLL_POINTS];
    
    static int compare (const void * a, const void * b) {
        return (*(Point2D*)a < *(Point2D*)b) ? 1 : -1;
    }
public:
    Point2D hull[2*MAX_CONTROLL_POINTS];
    int size = 0;
    
    ConvexHull(ControllPoints *cp): cp(cp) {}
    
    void calcHull(){
        int n = cp->size, k = 0;
        
        // Copy points
        for (int i = 0; i < n; i++) {
            points[i] = cp->points[i];
        }
        
        // Sort
        qsort(points, n, sizeof(Point2D), compare);
        
        // Build lower hull
        for (int i=0; i < n; ++i) {
            while (k >= 2 && hull[k-2].crossProduct(hull[k-1], points[i]) <= 0) k--;
            hull[k++] = points[i];
        }
        
        // Build upper hull
        for (int i=n-2, l=k+1; i >= 0; i--) {
            while (k >= l && hull[k-2].crossProduct(hull[k-1], points[i]) <= 0) k--;
            hull[k++] = points[i];
        }
        size = k;
    }
};

//--------------------------------------------------------
// Catmull-Rom spline
//--------------------------------------------------------
class CatmullRomSpline {
    ControllPoints *cp;
    
    float B(int i,float t) {
        switch (i) {
            case 0:
                return powf(1-t,3) / 6;
            case 1:
                return (1 + 3*(1-t) + 3*t*powf(1-t,2)) / 6;
            case 2:
                return (1 + 3*t + 3*(1-t)*powf(t,2)) / 6;
            default:
                return powf(t,3) / 6;
        }
    }
public:
    CatmullRomSpline(ControllPoints &cp): cp(&cp) {};
    
    Vector2D r(float t, int sg) {
        Vector2D r;
        for(int i = sg-2; i <= sg+1; i++)
            r += cp->points[i].vectorFromOrigo() * B(i,t);
        return r;
    }
};

//--------------------------------------------------------
// Catmull-Clark Subdivision curve
//--------------------------------------------------------
class CatmullClarkCurve {
    ControllPoints* cp;
    
    void subdivision() {
        Point2D middlePoints[MAX_SUBDIVISION_POINTS];
        Point2D newPoints[MAX_SUBDIVISION_POINTS];
        
        //Calculate middle points
        for (int i = 0; i < size-1; i++) {
            middlePoints[i] = points[i].average(points[i+1]);
        }
        
        //Move original points
        for (int i = 1; i < size-1; i++) {
            //avg of 2 middle points
            auto avg = middlePoints[i-1].average(middlePoints[i]);
            
            points[i] = Point2D((2*avg.x + points[i].x)/3, (2*avg.y + points[i].y)/3);
        }
        
        //Add middle points between original points
        for (int i = 0; i < size; i++) {
            //Add moved original point
            newPoints[i*2] = points[i];
            
            //Add middle point
            newPoints[i*2+1] = middlePoints[i];
        }
        
        size += size - 1;
        
        // Copy new points
        for (int i = 0; i < size; i++) {
            points[i] = newPoints[i];
        }
    }
public:
    Point2D points[MAX_SUBDIVISION_POINTS];
    int size = 0;
    
    CatmullClarkCurve(ControllPoints *cp): cp(cp) {}
    
    void calcPoints(int subdivisions) {
        // Copy points
        for (int i = 0; i < cp->size; i++) {
            points[i] = cp->points[i];
        }
        size = cp->size;
        for(int i = 0; i < subdivisions; i++) {
            subdivision();
        }
    }
};