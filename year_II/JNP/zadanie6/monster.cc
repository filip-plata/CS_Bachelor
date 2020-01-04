#include <algorithm>
#include "monster.h"


Monster::Monster(types::HealthPoints health,
                 types::AttackPower attack)
    : health_(health), attack_(attack) {}


void Monster::takeDamage(types::AttackPower damage) {
    health_ = std::max(0, health_ - damage);
}


Mummy::Mummy(types::HealthPoints health, types::AttackPower attack)
    : Monster(health, attack) {}

std::string Mummy::getMonsterName() const {
    return "Mummy";
}

Zombie::Zombie(types::HealthPoints health, types::AttackPower attack)
    : Monster(health, attack) {}

std::string Zombie::getMonsterName() const {
    return "Zombie";
}

Vampire::Vampire(types::HealthPoints health, types::AttackPower attack)
    : Monster(health, attack) {}

std::string Vampire::getMonsterName() const {
    return "Vampire";
}

std::shared_ptr<Mummy> createMummy(types::HealthPoints health,
                  types::AttackPower attack) {
    return std::make_shared<Mummy>(Mummy(health, attack));
}

std::shared_ptr<Zombie> createZombie(types::HealthPoints health,
                   types::AttackPower attack) {
    return std::make_shared<Zombie>(Zombie(health, attack));
}

std::shared_ptr<Vampire> createVampire(types::HealthPoints health,
                    types::AttackPower attack) {
    return std::make_shared<Vampire>(Vampire(health, attack));
}


GroupOfMonsters::GroupOfMonsters(std::vector<std::shared_ptr<Monster>> monsters) {
    for (auto monster : monsters) {
        group_.push_back(monster);
    }
}


GroupOfMonsters::GroupOfMonsters(std::initializer_list<std::shared_ptr<Monster>> monsters) {
    for (auto monster : monsters) {
        group_.push_back(monster);
    }
}


std::string GroupOfMonsters::getMonsterName() const {
    return "GroupOfMonsters";
}

types::HealthPoints GroupOfMonsters::getHealth() const {
    types::HealthPoints result(0);
    
    for (auto & m : group_) {
        result += m->getHealth();
    }

    return result;
}

types::AttackPower GroupOfMonsters::getAttackPower() const {
    types::AttackPower result(0);
    
    for (auto & m : group_) {
        if (m->getHealth() > 0) {
            result += m->getAttackPower();
        }
    }

    return result;
}

void GroupOfMonsters::takeDamage(types::AttackPower damage) {
    for (auto & m : group_) {
        m->takeDamage(damage);
    }
}


std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(std::vector<std::shared_ptr<Monster>> monsters) {
    return std::make_shared<GroupOfMonsters>(GroupOfMonsters(monsters));
}

std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(std::initializer_list<std::shared_ptr<Monster>> monsters) {
    return std::make_shared<GroupOfMonsters>(GroupOfMonsters(monsters));
}

