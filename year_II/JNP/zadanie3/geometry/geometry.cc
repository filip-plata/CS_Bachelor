#include "geometry.h"
#include <cassert>

// ################# VECTOR


Vector::Vector(int x, int y) {
    _x = x;
    _y = y;
}

int Vector::x() const {
    return _x;
}

int Vector::y() const {
    return _y;
}

Vector Vector::reflection() const {
    Vector temp = Vector(*this);
    std::swap(temp._x, temp._y);
    return temp;
}

bool Vector::operator==(const Vector &p1) const {
    return (p1._x == _x) && (p1._y == _y);
}

Vector & Vector::operator=(const Vector &rhs) {
    _x = rhs._x;
    _y = rhs._y;
    return *this;
}

Vector & Vector::operator+=(const Vector &rhs) {
    this->_x += rhs._x;
    this->_y += rhs._y;
    return *this;
}

Vector Vector::operator+(const Vector &rhs) const {
    Vector temp(*this);
    temp += rhs;
    return temp;
}

Rectangles Vector::operator+(const Rectangles &recs) {
    Rectangles result = Rectangles(recs);
    return result += *this;
}

Rectangle Vector::operator+(const Rectangle &rec) {
    Rectangle result = Rectangle(rec);
    return result += *this;
}

Position Vector::operator+(const Position &pos) {
    Position result = Position(pos);
    return result += *this;
}

Vector::Vector(const Vector & rhs) {
    _x = rhs._x;
    _y = rhs._y;
}


// ################# END VECTOR

// ################# POSITION


Position::Position(int x, int y) {
    _x = x;
    _y = y;
}

Position::Position(const Position & pos) {
    _x = pos._x;
    _y = pos._y;
}

Position Position::reflection() const {
    Position temp = Position(*this);
    std::swap(temp._x, temp._y);
    return temp;
}

int Position::x() const {
    return this->_x;
}

int Position::y() const {
    return this->_y;
}

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

bool Position::operator==(const Position & pos) const {
    return (pos._x == _x) && (pos._y == _y);
}

bool Position::operator<(const Position &pos) const {
    return ((_x < pos._x) ||
            (_x == pos._x &&
             _y < pos._y));
}
    
// ################# END POSITION

// ################# RECTANGLE

Rectangle::Rectangle(int width, int height, Position pos)
    : _pos(pos) {
    assert(width > 0);
    assert(height > 0);
        
    _width = width;
    _height = height;
}

Rectangle::Rectangle(int width, int height)
    :   _pos(0, 0) {
    assert(width > 0);
    assert(height > 0);
    
    _width = width;
    _height = height;
}

Rectangle::Rectangle(const Rectangle & rec)
    : _pos(rec._pos) {
    _width = rec._width;
    _height = rec._height;
}

int Rectangle::width() const {
    return this->_width;
}

int Rectangle::height() const {
    return this->_height;
}

Position Rectangle::pos() const {
    return this->_pos;
}

Rectangle Rectangle::reflection() const {
    Position _pos = this->_pos.reflection();
    return Rectangle(_height, _width, _pos);
}

unsigned long Rectangle::area() const {
    return ((unsigned long)_width) * _height;
}


Rectangle merge_horizontally(const Rectangle & rec1, const Rectangle & rec2) {
    assert(rec1.width() == rec2.width() );
    assert((rec1.pos() + (Vector(0, rec1.height()))) == rec2.pos());
  
    Rectangle rec(rec1.width(), rec1.height() + rec2.height(), rec1.pos());
    return rec;
}


Rectangle merge_vertically(const Rectangle & rec1, const Rectangle & rec2) {
    assert(rec1.height() == rec2.height());
    
    assert(rec1.pos() + (Vector(rec1.width(), 0)) == rec2.pos());
  
    Rectangle rec(rec1.width() + rec2.width(), rec1.height(), rec1.pos());
    return rec;
}

std::pair<Rectangle, Rectangle> Rectangle::split_horizontally(int place) const {
    assert(place <  _height);
    assert(0 < place);
    
    Rectangle rec1(_width, place, Position(_pos));
    Rectangle rec2(_width, _height - place,
                   (Position(_pos) + Vector(0, place)));

    
    return std::make_pair(rec1, rec2);
}

std::pair<Rectangle, Rectangle> Rectangle::split_vertically(int place) const {
    assert(place < _width);
    assert(0 < place);
    
    Rectangle rec1(place, _height, Position(_pos));
    Rectangle rec2(_width - place, _height,
                   (Position(_pos) + Vector(place, 0)));

    
    return std::make_pair(rec1, rec2);
}


bool Rectangle::operator==(const Rectangle &rec) const {
    return (rec._width == _width) && (rec._height == _height) && (rec._pos == _pos);
}

bool Rectangle::operator<(const Rectangle &rec) const {
    return _pos < rec._pos ;
}

Rectangle & Rectangle::operator+=(const Vector & vec) {
    _pos += vec;
    return *this;
}

// ################# END RECTANGLE

// ################# RECTANGLES

Rectangles::Rectangles() {
}

Rectangles::Rectangles(std::vector<Rectangle> values)
    : _rectangles(values) {
}

Rectangles::Rectangles(const Rectangles & recs) {
    _rectangles = recs._rectangles;
}

Rectangles::Rectangles(Rectangles&& recs)
    : _rectangles(std::move(recs._rectangles)) {
}

size_t Rectangles::size() {
    return _rectangles.size();
}

Rectangles & Rectangles::operator=(const Rectangles & recs) {
    _rectangles = recs._rectangles;
    return *this;
}

Rectangles & Rectangles::operator=(Rectangles&& recs) {
    _rectangles = std::move(recs._rectangles);
    return *this;
}

Rectangles & Rectangles::operator+=(const Vector & vec) {
    for (Rectangle & rec : _rectangles) {
        rec += vec;
    }
    return *this;
}

Rectangle & Rectangles::operator[](size_t idx) {
    assert(idx >= 0);
    return _rectangles[idx];
}

const Rectangle & Rectangles::operator[](size_t idx) const {
    return _rectangles[idx];
}

void Rectangles::split_horizontally(size_t idx, int place) {
    assert(0 <= idx);
    assert(idx < _rectangles.size());
    
    std::pair<Rectangle, Rectangle> split =
        _rectangles[idx].split_horizontally(place);
    
    _rectangles[idx] = split.first;
    _rectangles.insert(_rectangles.begin() + idx + 1, split.second);
}

void Rectangles::split_vertically(size_t idx, int place) {
    assert(0 <= idx);
    assert(idx < _rectangles.size());
    
    std::pair<Rectangle, Rectangle> split =
        _rectangles[idx].split_vertically(place);
    
    _rectangles[idx] = split.first;
    _rectangles.insert(_rectangles.begin() + idx + 1, split.second);
}

bool Rectangles::operator==(const Rectangles &recs) const {
    /* Copy vectors for sorting and comparison */
    auto recs1 = _rectangles;
    auto recs2 = recs._rectangles;
    
    std::sort(recs1.begin(), recs1.end());
    std::sort(recs2.begin(), recs2.end());
       
    return recs1 == recs2;
}

// ################# END RECTANGLES

