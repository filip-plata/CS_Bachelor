#include<iostream>
#include<algorithm>
#include<assert.h>
#include"geometry.h"


using namespace std;


int main() {

    // testowanie splitowania prostokątów za treścią
    Position pos(0, 0);
    Position pos1(0, 20);
    Rectangle rec(100, 200);
    pair<Rectangle, Rectangle> split = rec.split_horizontally(20);

    Position pos2(0, 0);
    Position pos3(0, 20);
    Rectangle rec1 = rec.reflection();
    pair<Rectangle, Rectangle> split1 = rec1.split_vertically(20);
    
    assert(split.first.pos() == pos);
    assert(split.first.width() == 100);
    assert(split.first.height() == 20);

    assert(split.second.pos() == pos1);
    assert(split.second.width() == 100);
    assert(split.second.height() == 180);

    // test reflection

    assert(rec1.pos() == rec.pos().reflection());
    assert(rec1.width() == rec.height());
    assert(rec1.height() == rec.width());

    assert(split1.first.reflection() == split.first);
    assert(split1.second.reflection() == split.second);

    // overflow na area
    Rectangle rec2(10000000, 10000000, pos);

    assert(rec2.area() == ((unsigned long) 10000000) * ((unsigned long) 10000000));

    // testowanie mergowania

    Position position1(20, 20);
    Rectangle rectangle1(20, 10, position1);
    Position position2(20, 30);
    Rectangle rectangle2(20, 40, position2);

    Rectangle merged_horizontally = merge_horizontally(rectangle1, rectangle2);

    assert(merged_horizontally.width() == 20);
    assert(merged_horizontally.height() == 50);
    assert(merged_horizontally.pos() == position1);
    
    Position position3(20, 40);
    Rectangle rectangle3(20, 10, position3);
    Position position4(40, 40);
    Rectangle rectangle4(30, 10, position4);

    Rectangle merged_vertically = merge_vertically(rectangle3, rectangle4);

    assert(merged_vertically.width() == 50);
    assert(merged_vertically.height() == 10);
    assert(merged_vertically.pos() == position3);
    
    return 0;
}
