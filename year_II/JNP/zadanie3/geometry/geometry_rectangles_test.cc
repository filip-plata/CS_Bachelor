#include<iostream>
#include<algorithm>
#include<assert.h>
#include"geometry.h"


using namespace std;

int main() {
    Rectangles rectangles = Rectangles();
    Position pos = Position(10, 10);

    Rectangle rec1 = Rectangle(10, 20);
    Rectangle rec2 = Rectangle(10, 20, pos);
    Vector vec = Vector(10, 10);

    assert(rectangles.size() == 0);

    Rectangles recs = Rectangles({rec1, rec2});


    assert(recs.size() == 2);

    assert(recs[1] == rec2);
    assert(recs[0] == rec1);


    Rectangles shifted = vec + recs;

    assert(!(shifted[0] == rec1));
    assert(!(shifted[1] == rec2));

    assert(!(shifted[0] == recs[0]));
    assert(!(shifted[1] == recs[1]));

    assert(shifted[0].pos().x() == 10);

    shifted.split_horizontally(1, 5);

    assert(shifted.size() == 3);


    assert(shifted[2].width() == 10);
    
    
    return 0;
}
