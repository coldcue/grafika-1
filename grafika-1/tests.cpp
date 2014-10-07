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

//------------------------------------------------
// Vector2d
//------------------------------------------------
void testVector2D() {
    cout<<"[ Testing Vector2D ]"<<endl;
    
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
    
}

int main(int argc, char **argv) {
    
    testVector2D();
    
    if(errorcount)
        cout<<endl<<errorcount<<" ERROR(S) - TEST FAILED!"<<endl;
    else
        cout<<endl<<"NO ERORRS - TEST SUCCESSFUL"<<endl;
    
    return 0;
}
