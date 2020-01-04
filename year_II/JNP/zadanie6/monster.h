#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include "helper.h"

class Attacker {
public:
    virtual types::HealthPoints getHealth() const = 0;
    virtual types::AttackPower getAttackPower() const = 0;
    virtual void takeDamage(types::AttackPower damage) = 0;
    virtual std::string getMonsterName() const = 0;
};

class Monster : public Attacker {
private:
    types::HealthPoints health_;
    types::AttackPower attack_;
public:
    Monster(types::HealthPoints health,
            types::AttackPower attack);
    
    types::HealthPoints getHealth() const override {
        return health_;
    }

    types::AttackPower getAttackPower() const override {
        return attack_;
    }

    void takeDamage(types::AttackPower damage) override;
    std::string getMonsterName() const override { return "monster" ; }
    
};

class Zombie : public Monster {
public:
    Zombie(types::HealthPoints health, types::AttackPower attack);
    std::string getMonsterName() const;
};

class Vampire : public Monster {
public:
    Vampire(types::HealthPoints health, types::AttackPower attack);
    std::string getMonsterName() const;
};

class Mummy : public Monster {
public:
    Mummy(types::HealthPoints health, types::AttackPower attack);
    std::string getMonsterName() const;
};


std::shared_ptr<Mummy> createMummy(types::HealthPoints health, types::AttackPower attack);

std::shared_ptr<Zombie> createZombie(types::HealthPoints health, types::AttackPower attack);

std::shared_ptr <Vampire> createVampire(types::HealthPoints health, types::AttackPower attack);

class GroupOfMonsters : public Attacker {
private:
    std::vector<std::shared_ptr<Monster>> group_;
public:
    GroupOfMonsters(std::vector<std::shared_ptr<Monster>> monsters);
    GroupOfMonsters(std::initializer_list<std::shared_ptr<Monster>> monsters);
    ~GroupOfMonsters() {};

    types::HealthPoints getHealth() const;
    types::AttackPower getAttackPower() const;
    void takeDamage(types::AttackPower damage);
    std::string getMonsterName() const;
};

std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(std::vector<std::shared_ptr<Monster>> monsters);

std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(std::initializer_list<std::shared_ptr<Monster>> monsters);

#endif /* MONSTER_H */
