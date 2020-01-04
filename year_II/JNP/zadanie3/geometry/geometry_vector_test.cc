#include"geometry.h"
#include<iostream>
#include<cassert>

using namespace std;


int main() {
    Vector vec1(10, 10);
    Vector vec2(-10, 10);

    Vector vec3 = vec1 + vec2;

    assert(vec1.x() == 10);
    assert(vec2.x() == -10);

    assert((vec1 += vec2).x() == 0);

    assert(vec2.x() == -10);
    assert(vec3.y() == 20);

    Vector ref1 = vec3.reflection();

    assert(ref1.x() == vec3.y() && ref1.y() == vec3.x());

    ref1 = vec3;

    assert(ref1.y() == vec3.y() && ref1.x() == vec3.x());

    Vector vec4(vec3);

    vec3 += vec1;

    assert(vec4.x() == 0 && vec4.y() == 20);

    Position pos = Position(10, 10);

    Vector vec5 = Vector(10, 10);

    Position pos1 = pos + vec5;

    assert(pos.x() == 10 && pos.y() == 10 );

    assert(pos1.x() == 20 && pos1.y() == 20);
    
    return 0;
}
