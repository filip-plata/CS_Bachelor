Rectangle::Rectangle(int width, int height, Position pos) {
  _width = width;
  _height = height;
  _pos = pos;
}

Rectangle::Rectangle(int width, int height)
  : _pos(0, 0) {
  _width = width;
  _height = height;
}

int Rectangle::width() const {
  return this->_width;
}

int Rectangle::height() const {
  return this->_height;
}

Position Rectangle::pos() const {
  return this._pos;
}

Rectangle Rectangle::reflection() {
    Rectangle temp = Rectangle(*this);
    temp._pos = temp._pos.reflection();
    std::swap(temp._height, temp._width);
    return temp;
}


unsigned long area(){
  return ((unsigned long)_width) * _height;
}

//nie wiem, czy nie trzeba w assercie sprawdzac, czy te prostokaty rzeczywiscie
//sa obok siebie
Rectangle merge_horizontally(const Rectangle & rec1, const Rectangle & rec2) {
  //zakladam, ze powinni miec ta sama szerokosc
  assert(rec1.width() == rec2.width);
  
  Rectangle rec(rec1.width(), rec1.height() + rec2.height(), rec1.pos());
  return rec;
}

//nie wiem, czy nie trzeba w assercie sprawdzac, czy te prostokaty rzeczywiscie
//sa obok siebie
Rectangle merge_vertically(const Rectangle & rec1, const Rectangle & rec2) {
  //zakladam, ze maja ta sama wysokosc
  //doklejam rec2 po prawej rec1, nie jest sprecyzowane w tresci zadania
  assert(rec1.height() == rec2.height());
  
  Rectangle rec(rec1.width() + rec2.width(), rec1.height(), rec1.pos());
  return rec;
}



std::pair<Rectangle, Rectangle> Rectangle::split_vertically(int place) {
    assert(place < _pos.y() + _height);
    assert(_height - place > 0);
    
    Rectangle rec1(_width, place - _pos.y(), Position(_pos));
    Rectangle rec2(_width, _height - place,
                   (Position(_pos) + Vector(0, place)));

    
    return std::make_pair(rec1, rec2);
}

std::pair<Rectangle, Rectangle> Rectangle::split_horizontally(int place) {
    assert(place < _pos.x() + _width);
    assert(_width - place > 0);
    
    Rectangle rec1(place - _pos.x(), _height, Position(_pos));
    Rectangle rec2(_width - place, _height,
                   (Position(_pos) + Vector(place, 0)));

    
    return std::make_pair(rec1, rec2);
}


bool Rectangle::operator==(const Rectangle &rec) const {
  return (rec._width == _width) && (rec._height == _height) && (rec._pos == _pos);
}


Rectangle & Rectangle::operator+=(const Vector & vec) {
  _pos += vec;
  return *this;
}
