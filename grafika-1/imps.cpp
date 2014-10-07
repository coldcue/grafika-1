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
};

//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
    float r, g, b;
    
    Color( ) {
        r = g = b = 0;
    }
    
    Color(float r0, float g0, float b0) {
        r = r0; g = g0; b = b0;
    }
    Color operator*(float a) {
        return Color(r * a, g * a, b * a);
    }
    Color operator*(const Color& c) {
        return Color(r * c.r, g * c.g, b * c.b);
    }
    Color operator+(const Color& c) {
        return Color(r + c.r, g + c.g, b + c.b);
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
        Vector2D r = Vector2D();
        for(int i = 0; i < cp->size; i++)
            r += cp->points[i].vectorFromOrigo() * B(i,t);
        return r;
    }
};

//--------------------------------------------------------
// Catmull-Rom spline
//--------------------------------------------------------

//--------------------------------------------------------
// Catmull-Clark curve
//--------------------------------------------------------