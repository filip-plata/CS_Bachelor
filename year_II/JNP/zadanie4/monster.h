#ifndef MONSTER_H
#define MONSTER_H

#include "citizen.h"
#include <algorithm>
#include <iostream>

template <typename T, const char... name> class Monster {
    static_assert(std::is_arithmetic<T>::value, "No arithmetic type");

private:
    T _health;
    T _attack;

public:
    Monster(T health, T attack) : _health(health), _attack(attack) {}
    T getHealth() const { return _health; }
    T getAttackPower() const { return _attack; }
    void takeDamage(T damage) {
        T min = std::min(damage, _health);
        _health -= min;
    }
    
    std::string getName() {
        return {name...};
    }
    typedef T valueType;
};

template <typename T> using Vampire = Monster<T, 'V', 'a', 'm', 'p', 'i', 'r', 'e'>;

template <typename T> using Zombie = Monster<T, 'Z', 'o', 'm', 'b', 'i', 'e'>;

template <typename T> using Mummy = Monster<T, 'M', 'u', 'm', 'm', 'y'>;

template <typename M, typename U> void attack(M &monster, U &victim) {
    victim.takeDamage(monster.getAttackPower());
}

template <typename M, typename T> void attack(M &monster, Sheriff<T> &victim) {
    victim.takeDamage(monster.getAttackPower());
    monster.takeDamage(victim.getAttackPower());
}

#endif /* MONSTER_H */
