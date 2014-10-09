//
//  tests.cpp
//  grafika-1
//
//  Created by Andras Szell on 10/7/14.
//  Copyright (c) 2014 Andras Szell. All rights reserved.
//
#include <iostream>

#define private public
#define protected public
#define class struct
#include "imps.cpp"


using namespace std;

int errorcount = 0;

void assert(bool condition){
    if(condition) cout<<"OK"<<endl;
    else {
        cout<<"ERROR"<<endl;
        errorcount++;
    }
}

void assertFloat(float f, float expected){
    assert(f>=expected-0.00001 && f<=expected+0.00001);
}

//------------------------------------------------
// Vector2D
//------------------------------------------------
void testVector2D() {
    cout<<endl<<"[ Testing Vector2D ]"<<endl;
    
    auto v1 = Vector2D(1,2);
    auto v2 = Vector2D(3,5);
    
    cout<<"operator+ ";
    auto v3 = v1 + v2;
    assert(v3.x == 4 && v3.y == 7);
    
    cout<<"operator- ";
    auto v4 = v1 - v2;
    assert(v4.x == -2 && v4.y == -3);
    
    cout<<"operator+= ";
    auto v5 = v1;
    v5 += v2;
    assert(v5.x == 4 && v5.y == 7);
    
    cout<<"operator-= ";
    auto v6 = v1;
    v6 -= v2;
    assert(v6.x == -2 && v6.y == -3);
    
    cout<<"operator* ";
    auto v7 = v1 * 5.0;
    assert(v7.x == 5 && v7.y == 10);
    
    cout<<"abs ";
    float l = v2.abs();
    assertFloat(l, 5.83095);
    
}

//------------------------------------------------
// Point2D
//------------------------------------------------
void testPoint2D() {
    cout<<endl<<"[ Testing Point2D ]"<<endl;
    
    auto vt = Vector2D(2,3);
    auto p1 = Point2D(1,2);
    auto p2 = Point2D(3,5);
    
    cout<<"operator+ ";
    auto p3 = p1 + vt;
    assert(p3.x == 2 && p3.y == 6);
    
    cout<<"operator- ";
    auto v1 = p1 - p2;
    assert(v1.x == -2 && v1.y == -3);
    
    cout<<"dist ";
    float d = p1.dist(p2);
    assertFloat(d, 3.60555);
    
    cout<<"average ";
    auto p4 = p1.average(p2);
    assert(p4 == Point2D(2.0f, 3.5f));
}

//--------------------------------------------------------
// ControllPoints
//--------------------------------------------------------
ControllPoints testControllPoints(){
    cout<<endl<<"[ Testing ControllPoints ]"<<endl;
    ControllPoints cp;
    
    cp.add(Point2D(1,2));
    cout<<"add ";
    assert(cp.points[0].dist(Point2D(1,2))==0);
    cp.add(Point2D(3,5));
    cp.add(Point2D(7,11));
    cp.add(Point2D(-3,6));
    cp.add(Point2D(-7,5));
    
    cout<<"size ";
    assert(cp.size==5);
    
    return cp;
}

//--------------------------------------------------------
// Brezier curve
//--------------------------------------------------------
void testBrezierCurve(ControllPoints& cp) {
    cout<<endl<<"[ Testing Brezier curve ]"<<endl;
    
    BrezierCurve bc;
    bc.setControllPoints(&cp);
    
    cout<<"nck ";
    assert(bc.nCk(3, 0) == 1);
    assert(bc.nCk(3, 1) == 3);
    assert(bc.nCk(3, 2) == 3);
    assert(bc.nCk(3, 3) == 1);
    assert(bc.nCk(3, 4) == 0);
    
    cout<<endl<<"B calculation tests "<<endl;
    for(int it = 0; it<=10; it++){
        float t = (float)it/10.0;
        cout<<"t="<<t<<" ->";
        
        float sum = 0;
        for(int i = 0; i < bc.cp->size; i++) {
            auto b = bc.B(i,t);
            sum += b;
            cout<<" "<<i<<"="<<b;
        }
        cout <<" ";
        assertFloat(sum, 1);
    }
    
    cout<<endl<<"r calculation tests "<<endl;
    for(int it = 0; it<=10; it++){
        float t = (float)it/10.0;
        cout<<"t="<<t<<" -> ";
        Vector2D r = bc.r(t);
        cout<<"("<<r.x<<";"<<r.y<<")"<<endl;
    }

}

//--------------------------------------------------------
// Convex hull
//--------------------------------------------------------
void testConvexHull(ControllPoints& cp) {
    cout<<endl<<"[ Testing Convex hull ]"<<endl;
    
    ConvexHull ch(&cp);
    ch.calcHull();
    
    for(int i = 0; i < ch.size; i++) {
        cout<<"("<<ch.hull[i].x<<";"<<ch.hull[i].y<<") ";
        
        switch (i) {
            case 0:
                assert(ch.hull[i] == Point2D(7, 11));
                break;
            case 1:
                assert(ch.hull[i] == Point2D(-7, 5));
                break;
            case 2:
                assert(ch.hull[i] == Point2D(1, 2));
                break;
            case 3:
                assert(ch.hull[i] == Point2D(7, 11));
                break;
            default:
                assert(false);
                break;
        }
    }
}

//--------------------------------------------------------
// Catmull clark
//--------------------------------------------------------
void testCatmullClarkCurve(ControllPoints& cp){
    cout<<endl<<"[ Testing Catmull-Clark Curve ]"<<endl;
    
    CatmullClarkCurve cc(&cp);
    cc.calcPoints(1);
    
    for(int i = 0; i < cc.size; i++) {
        cout<<"("<<cc.points[i].x<<";"<<cc.points[i].y<<") "<<endl;
    }
    
    cc = CatmullClarkCurve(&cp);
    cc.calcPoints(2);
    
    for(int i = 0; i < cc.size; i++) {
        cout<<"("<<cc.points[i].x<<";"<<cc.points[i].y<<") "<<endl;
    }
}

//--------------------------------------------------------
// Catmull rom
//--------------------------------------------------------
void testCatmullRomCurve(ControllPoints& cp) {
    cout<<endl<<"[ Testing Catmull-Rom Curve ]"<<endl;
    
    CatmullRomSpline cr(cp);
    
    //Print 1 segment
    cout<<endl<<"calculation tests "<<endl;
    for(int it = 0; it<=10; it++){
        float t = (float)it/10.0;
        cout<<"t="<<t<<" ->";
        
        auto r = cr.r(t, 2);
        cout<<"("<<r.x<<";"<<r.y<<") "<<endl;
    }
}

int main(int argc, char **argv) {
    
    testVector2D();
    testPoint2D();
    auto cntrPnts = testControllPoints();
    testBrezierCurve(cntrPnts);
    testConvexHull(cntrPnts);
    testCatmullClarkCurve(cntrPnts);
    testCatmullRomCurve(cntrPnts);
    
    if(errorcount)
        cout<<endl<<errorcount<<" ERROR(S) - TEST FAILED!"<<endl;
    else
        cout<<endl<<"NO ERORRS - TEST SUCCESSFUL"<<endl;
    
    return 0;
}
