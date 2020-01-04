#include<iostream>
#include<assert.h>
#include"geometry.h"



// class Position: public Pair {
// public:
//     Position(int x, int y) : Pair(x, y) {};
//     static const Position origin();
//     Position & operator+=(Vector vec);
//     Position & operator+(const Vector & rhs);
//     Position & operator=(const Position & rhs);
//     bool operator==(const Position & rhs);
// };

//Nie wiem, czy nie trzeba dodac do kodu ?
//explicit Position() {}


//nie wiem tez, jak ma dzialac to dziedziczenie
//po Pair konstruktora xD
Position(int x, int y) : Pair(x, y) {
}

//nie jestem pewna, czy nie wymagaja od nas, zeby stworzyc tak
//w poprzednim zadaniu niemodyfikowalny obiekt..
const Position Position::origin() {
    return Position(0,0);
}

Position & Position::operator+=(const Vector & vec) {
    _x += vec.x();
    _y += vec.y();
    return *this;
}

Position & Position::operator=(const Position &rhs) {
    _x = rhs._x;
    _y = rhs._y;
    return *this;
}

//wersja z goemetry.cc nie dzialala, bo jak zrobisz:
//bo teraz jak masz
//Position a(1, 2);
//Position b(3, 4);
//Position c = a + b;
//nagle a == Position(4, 6)
//operatory binarne nie zmieniają obiektu na którym działają
//tylko tworzą nowy obiekt
Position & Position::operator+(const Vector& rhs) {
    Position copy(*this);
    copy += rhs;
    return copy;
}

bool Position::operator==(const Position & pos) const {
  return (pos._x == _x) && (pos._y == _y);
}