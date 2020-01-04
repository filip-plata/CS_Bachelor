#include "geometry.h"
#include <cassert>
#include <tuple>
#include <iostream>

void vector_tests() {
    /* Constructor test */
    Vector vector(2, 1);
    assert(vector.x() == 2 && vector.y() == 1);

    /* Reflection test */
    Vector vector_reflection = vector.reflection();
    assert(vector_reflection.x() == 1 && vector_reflection.y() == 2);

    /* Operator += test */
    vector += vector_reflection;
    assert(vector.x() == 3 && vector.y() == 3);

    /* Operator == test */
    assert(vector == vector.reflection());
    assert(!(vector == Vector(0, 0)));

    /* Operator + test */
    vector = Vector(1, 0) + Vector(0, 1);
    assert(vector.x() == vector.y() == 1);
}

void vector_fail_tests() {
    /* This should fail the build
     * Uncomment to test */
    /*
    Vector invalid_vector_construct;
    */
}

void position_tests() {
    /* Constructor test */
    Position position(2, 1);
    assert(position.x() == 2 && position.y() == 1);

    /* Reflection test */
    Position position_reflection = position.reflection();
    assert(position_reflection.x() == 1 && position_reflection.y() == 2);

    /* Operator += test */
    position += Vector(1, 2);
    assert(position.x() == 3 && position.y() == 3);

    /* Operator == test */
    assert(position == position.reflection());
    assert(!(position == Position(0, 0)));

    /* Origin test */
    assert(Position::origin() == Position(0, 0));

    /* Operator + test */
    position = Position(-1, -1) + Vector(1, 1);
    assert(position == Position::origin());
}

void position_fail_tests() {
    /* This should fail the build
     * Uncomment to test */
    /*
    Position position(0, 0);
    Position invalid_position_construct;
    position += Position(0, 0);
    position == Vector(0, 0);
    */
}

Rectangle make_square(long long size) {
    return Rectangle(size, size);
}

void rectangle_tests() {
    /* Constructor without position test */
    Rectangle rectangle(10, 10);
    assert(rectangle.pos() == Position::origin());
    assert(rectangle.width() == 10);
    assert(rectangle.height() == 10);

    /* Constructor with position test */
    Rectangle rectanglePosition(20, 20, Position(1, 1));
    assert(rectanglePosition.pos() == Position(1, 1));
    assert(rectanglePosition.width() == 20);
    assert(rectanglePosition.height() == 20);

    /* Copy constructor test */
    Rectangle rectangleCopy = rectangle;
    assert(rectangleCopy.pos() == Position::origin());
    assert(rectangleCopy.width() == 10);
    assert(rectangleCopy.height() == 10);

    /* Copy = operator test */
    rectangleCopy = rectanglePosition;
    assert(rectangleCopy.pos() == Position(1, 1));
    assert(rectangleCopy.width() == 20);
    assert(rectangleCopy.height() == 20);

    /* Move constructor test */
    Rectangle rectangleMove = std::move(make_square(10));
    assert(rectangleMove.pos() == Position::origin());
    assert(rectangleMove.width() == 10);
    assert(rectangleMove.height() == 10);

    /* Move = operator test */
    rectangleMove = make_square(20);
    assert(rectangleMove.pos() == Position::origin());
    assert(rectangleMove.width() == 20);
    assert(rectangleMove.height() == 20);

    /* Area test */
    Rectangle rectangleArea(10, 10);
    assert(rectangleArea.area() == 100);

    /* Reflection test */
    Rectangle rectangleReflection = Rectangle(5, 10, Position(1, 0)).reflection();
    assert(rectangleReflection.width() == 10);
    assert(rectangleReflection.height() == 5);
    assert(rectangleReflection.pos() == Position(0, 1));

    /* Operator == test */
    Rectangle rectangleEqualsA(5, 10);
    Rectangle rectangleEqualsB(5, 10);
    assert(rectangleEqualsA == rectangleEqualsB);
    rectangleEqualsA += Vector(1, 1);
    assert(!(rectangleEqualsA == rectangleEqualsB));
    rectangleEqualsA = Rectangle(10, 5);
    assert(!(rectangleEqualsA == rectangleEqualsB));

    /* Split test */
    Rectangle rectangleSplit(10, 10);
    Rectangle rectangleSplitA(1, 1), rectangleSplitB(1, 1);

    std::tie(rectangleSplitA, rectangleSplitB) = rectangleSplit.split_vertically(5);
    assert(rectangleSplitA.pos() == Position::origin());
    assert(rectangleSplitA.width() == 5);
    assert(rectangleSplitA.height() == 10);
    assert(rectangleSplitB.pos() == Position::origin() + Vector(5, 0));
    assert(rectangleSplitB.width() == 5);
    assert(rectangleSplitB.height() == 10);

    std::tie(rectangleSplitA, rectangleSplitB) = rectangleSplit.split_horizontally(5);
    assert(rectangleSplitA.pos() == Position::origin());
    assert(rectangleSplitA.width() == 10);
    assert(rectangleSplitA.height() == 5);
    assert(rectangleSplitB.pos() == Position::origin() + Vector(0, 5));
    assert(rectangleSplitB.width() == 10);
    assert(rectangleSplitB.height() == 5);

    /* Operator += test */
    Rectangle rectanglePlusEquals(10, 10);
    rectanglePlusEquals += Vector(1, 1);
    assert(rectanglePlusEquals.pos() == Position(1, 1));
    rectanglePlusEquals += Vector(10, 0);
    assert(rectanglePlusEquals.pos() == Position(11, 1));

    /* Operator + test */
    Rectangle rectanglePlus(10, 10, Position(5, 5));
    assert(rectanglePlus == Rectangle(10, 10) + Vector(5, 5));
    assert(rectanglePlus == Vector(5, 5) + Rectangle(10, 10));

    /* Merge test */
    Rectangle rectangleA(10, 10);
    Rectangle rectangleB(10, 35, Position(0, 10));
    Rectangle rectangleC(10, 45);
    //Rectangle rectangleAB = merge_horizontally(rectangleA, rectangleB);
    //Rectangle rectangleBA = merge_horizontally(rectangleB, rectangleA);
    //assert(rectangleAB == rectangleC);
    //assert(rectangleBA == rectangleC);

    Rectangle rectangleD(15, 5, Position(10, 10));
    Rectangle rectangleE(30, 5, Position(25, 10));
    Rectangle rectangleF(45, 5, Position(10, 10));
    //Rectangle rectangleDE = merge_vertically(rectangleD, rectangleE);
    //Rectangle rectangleED = merge_vertically(rectangleE, rectangleD);
    //assert(rectangleDE == rectangleF);
    //assert(rectangleED == rectangleF);
}

void rectangles_tests() {
    /* Some rectangles */
    Rectangle rectangle1(10, 15);
    Rectangle rectangle2(15, 20);
    Rectangle rectangle3(25, 5, Position(60, 35));
    Rectangle rectangle4(15, 10, Position(25, 10));
    Rectangle rectangle5(30, 20, Position(5, 5));
    Rectangle rectangle6(15, 20, Position(15, 10));
    Rectangle rectangle7(30, 20, Position(20, 15));

    /* Constructor test */
    Rectangles rectangles({rectangle1, rectangle3});
    assert(rectangles[0].width() == 10);
    assert(rectangles[0].height() == 15);
    assert(rectangles[0].pos().x() == 0);
    assert(rectangles[1].width() == 25);
    assert(rectangles[1].height() == 5);
    assert(rectangles[1].pos().y() == 35);
    assert(rectangles[1].pos().x() == 60);

    /* Size test */
    Rectangles rectanglesSizeA({rectangle1, rectangle2});
    Rectangles rectanglesSizeB({rectangle1, rectangle2, rectangle3, rectangle4});
    assert(rectanglesSizeA.size() == 2);
    assert(rectanglesSizeB.size() == 4);

    /* Operator == test */
    Rectangles rectanglesEqualsA({rectangle2, rectangle4, rectangle5});
    Rectangles rectanglesEqualsB({rectangle2, rectangle4, rectangle5});
    assert(rectanglesEqualsA == rectanglesEqualsB);
    rectanglesEqualsA += Vector(5, 5);
    assert(!(rectanglesEqualsA == rectanglesEqualsB));

    /* Operator += test */
    Rectangles rectanglesPlusEquals({rectangle1, rectangle4});
    rectanglesPlusEquals += Vector(10, 10);
    assert(rectanglesPlusEquals[0] == Rectangle(10, 15, Position(10, 10)));
    assert(rectanglesPlusEquals[1] == Rectangle(15, 10, Position(35, 20)));

    /* Operator + test */
    Rectangles rectanglesPlus({rectangle2, rectangle5});
    Rectangles rectanglesSum({rectangle6, rectangle7});
    assert(rectanglesSum == rectanglesPlus + Vector(15, 10));
    assert(rectanglesSum == Vector(15, 10) + rectanglesPlus);

    /* Split test */
    Rectangles rectanglesToSplit({rectangle1, rectangle4, rectangle5});
    Rectangle rectangleSplitA(15, 5, Position(25, 10));
    Rectangle rectangleSplitB(15, 5, Position(25, 15));
    Rectangles rectanglesAfterHorizontalSplit({rectangle1, rectangleSplitA,
                                    rectangleSplitB, rectangle5});
    rectanglesToSplit.split_horizontally(1, 5);
    assert(rectanglesToSplit == rectanglesAfterHorizontalSplit);

    /* Copy constructor test */
    Rectangles rectanglesCopy = rectangles;
    assert(rectanglesCopy[0].width() == 10);
    assert(rectanglesCopy[0].pos().x() == 0);
    assert(rectanglesCopy[1].pos().y() == 35);
    assert(rectanglesCopy[1].height() == 5);

    /* Copy = operator test */
    rectanglesCopy = rectanglesSizeA;
    assert(rectanglesCopy[0].width() == 10);
    assert(rectanglesCopy[1].height() == 20);
    assert(rectanglesCopy[1].pos().y() == 0);

    /* Move constructor test */
    Rectangles rectanglesMove = std::move(rectanglesSizeB);
    assert(rectanglesMove.size() == 4);
    assert(rectanglesMove[1].width() == 15);
    assert(rectanglesMove[1].pos().x() == 0);
    assert(rectanglesMove[3].pos().y() == 10);

    /* Move = operator test */
    rectanglesMove = rectanglesPlusEquals;
    assert(rectanglesMove.size() == 2);
    assert(rectanglesMove[1].width() == 15);
    assert(rectanglesMove[1].pos().x() == 35);
}

int main() {
    vector_tests();
    vector_fail_tests();
    position_tests();
    position_fail_tests();
    rectangle_tests();
    rectangles_tests();

    // Point<Vector> abstract_class_instantiation(0, 0);
    // position == vector;
}
