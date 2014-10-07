//
//  tests.cpp
//  grafika-1
//
//  Created by Andras Szell on 10/7/14.
//  Copyright (c) 2014 Andras Szell. All rights reserved.
//

#include "imps.cpp"
#include <iostream>

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
}

int main(int argc, char **argv) {
    
    testVector2D();
    testPoint2D();
    
    if(errorcount)
        cout<<endl<<errorcount<<" ERROR(S) - TEST FAILED!"<<endl;
    else
        cout<<endl<<"NO ERORRS - TEST SUCCESSFUL"<<endl;
    
    return 0;
}
