#ifndef CITIZEN_H
#define CITIZEN_H

#include <algorithm>
#include <cassert>
#include <iostream>

template <typename T, const int lage, const int uage, bool B> class Citizen {
    static_assert(std::is_arithmetic<T>::value, "No arithemtic type");

  private:
    T _health;
    T _age;
    T _attackPower;

  public:
    template <typename D = T>
    Citizen(typename std::enable_if<B, D>::type health, D age, D attackPower)
        : _health(health), _age(age), _attackPower(attackPower) {
        assert((lage <= age && age <= uage));
    }
    template <typename D = T>
    Citizen(typename std::enable_if<!B, D>::type health, D age)
        : _health(health), _age(age) {
        assert((lage <= age && age <= uage));
    }
    template <typename D = T>
    typename std::enable_if<B, D>::type getAttackPower() {
        return _attackPower;
    }
    T getHealth() const { return _health; }
    T getAge() const { return _age; }
    void takeDamage(T damage) {
        T min = std::min(damage, _health);
        _health -= min;
    }
};

template <typename T> using Adult = Citizen<T, 18, 100, false>;

template <typename T> using Teenager = Citizen<T, 11, 17, false>;

template <typename T> using Sheriff = Citizen<T, 18, 100, true>;

#endif /* CITIZEN_H */
