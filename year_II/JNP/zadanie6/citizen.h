#ifndef CITIZEN_H
#define CITIZEN_H

#include <exception>
#include <memory>
#include "monster.h"
#include "helper.h"

class AgeOutBounds : public std::exception {
public:
    const char* what() {
        return "age out of bounds";
    }
};

class Citizen {
protected:
    types::HealthPoints health_;
    types::Age age_;
public:
    Citizen(types::HealthPoints health, types::Age age,
            types::Age lower_bound, types::Age upper_bound);
    
    types::HealthPoints getHealth() const {
        return health_;
    }

    types::Age getAge() const {
        return age_;
    }

    void takeDamage(types::AttackPower damage);
    virtual bool attack(Attacker &monster);
};


class Adult : public Citizen {
public:
    Adult(types::HealthPoints health, types::Age age);
};

class Teenager : public Citizen {
public:
    Teenager(types::HealthPoints health, types::Age age);
};

class Sheriff : public Citizen {
private:
    types::AttackPower attack_;
public:
    Sheriff(types::HealthPoints health, types::Age age,
            types::AttackPower attack);
    
    types::AttackPower getAttackPower() const {
        return attack_;
    }
    bool attack(Attacker &monster);
};

std::shared_ptr<Teenager> createTeenager(types::HealthPoints health, types::Age age);

std::shared_ptr<Adult> createAdult(types::HealthPoints health, types::Age age);

std::shared_ptr<Sheriff> createSheriff(types::HealthPoints health, types::Age age,
                    types::AttackPower attack);

#endif /* CITIZEN_H */
