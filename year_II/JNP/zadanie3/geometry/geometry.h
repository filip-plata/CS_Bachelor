#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <algorithm>

class Vector;
class Position;
class Rectangle;
class Rectangles;

class Vector {
private:
    int _x;
    int _y;
public:
    Vector & operator+=(const Vector & vec);
    bool operator==(const Vector &vec) const;
    Vector operator+(const Vector & vec) const;
    Rectangles operator+(const Rectangles & recs);
    Rectangle operator+(const Rectangle & rec);
    Position operator+(const Position & pos);
    int x() const;
    int y() const;
    /* Returns a reflection of a vector along y = x axis */
    Vector reflection() const;
    Vector(int x, int y);
    Vector(const Vector &rhs);
    Vector& operator=(const Vector &rhs);
};

class Position {
private:
    int _x;
    int _y;
public:
    Position(int x, int y);
    Position(const Position & pos);
    static const Position origin();
    Position & operator+=(const Vector & vec);
    Position & operator=(const Position &rhs);
    bool operator==(const Position &p1) const;
    /* Ordered lexicographically */
    bool operator<(const Position &rec) const;
    int x() const;
    int y() const;
    /* Returns a reflection of position along y = x axis */
    Position reflection() const;
};

inline Position operator+(Position lhs, const Vector & rhs) {
    return lhs += rhs;
}

class Rectangle {
private:
    int _width;
    int _height;
    Position _pos;
public:
    Rectangle(int width, int height, Position pos);
    Rectangle(int width, int height);
    Rectangle(const Rectangle & rec);
    int width() const;
    int height() const;
    /* Returns a deep copy of a position */
    Position pos() const;
    /* Returns a reflection of rectangle along y = x axis */
    Rectangle reflection() const;
    unsigned long area() const;
    /* Splits rectangle to two rectangles by X axis on height
       pos() + (0, place). Argument is assumed to be resonable. */
    std::pair<Rectangle, Rectangle> split_horizontally(int place) const;
    /* Splits rectangle to two rectangles by Y axis on width
       pos() + (place, 0). Argument is assumed to be resonable. */
    std::pair<Rectangle, Rectangle> split_vertically(int place) const;
    bool operator==(const Rectangle &rec) const;
    /* Ordered lexicographically by position */
    bool operator<(const Rectangle &rec) const;
    Rectangle & operator+=(const Vector & vec);
};

inline Rectangle operator+(Rectangle lhs, const Vector & rhs) {
    return lhs += rhs;
}

class Rectangles {
private:
    std::vector<Rectangle> _rectangles;
public:
    Rectangles();
    Rectangles(std::vector<Rectangle> values);
    Rectangles(const Rectangles & recs);
    Rectangles(Rectangles&& recs);
    size_t size();
    /* Splits rectangle on position idx horizontally.
       Fails if idx is not within bounds */
    void split_horizontally(size_t idx, int place);
    /* Splits rectangle on position idx vertically.
       Fails if idx is not within bounds */
    void split_vertically(size_t idx, int place);
    Rectangles & operator=(const Rectangles & recs);
    Rectangles & operator=(Rectangles&& recs);
    Rectangles & operator+=(const Vector & vec);
    /* Returns whether two sets of rectangles are equal
       Works in linear - logarithmic time because of sorting.
     */
    bool operator==(const Rectangles &recs) const;
    Rectangle & operator[](size_t idx);
    const Rectangle & operator[](size_t idx) const;
};

inline Rectangles operator+(Rectangles lhs, const Vector & rhs) {
    return lhs += rhs;
}

/* Merges two rectangles by X axis. They are assumed to be of
   equal width and to be aligned with respecto to Y axis. */
Rectangle merge_horizontally(const Rectangle & rec1, const Rectangle & rec2);

/* Merges two rectangles by Y axis. They are assumed to be of
   equal height and to be aligned with respecto to X axis. */
Rectangle merge_vertically(const Rectangle & rec1, const Rectangle & rec2);

#endif // GEOMETRY_H
