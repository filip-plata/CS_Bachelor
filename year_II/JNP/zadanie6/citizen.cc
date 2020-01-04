#include <algorithm>
#include "citizen.h"

void Citizen::takeDamage(types::AttackPower damage) {
    health_ = std::max(health_ - damage, 0);
}

Citizen::Citizen(types::HealthPoints health, types::Age age,
                 types::Age age_bound_lower, types::Age age_bound_upper)
    : health_(health), age_(age) {
    if (age_ > age_bound_upper || age_ < age_bound_lower) {
        throw AgeOutBounds();
    }
}

bool Citizen::attack(Attacker &monster) {
    if (health_ > 0) {
        takeDamage(monster.getAttackPower());
        return health_ == 0;
    }

    return false;
}

Teenager::Teenager(types::HealthPoints health, types::Age age)
    : Citizen(health, age, 11, 17) {}

Adult::Adult(types::HealthPoints health, types::Age age)
    : Citizen(health, age, 18, 100) {}

Sheriff::Sheriff(types::HealthPoints health, types::Age age,
                 types::AttackPower attack)
    : Citizen(health, age, 18, 100), attack_(attack) {}

bool Sheriff::attack(Attacker &monster) {
    if (health_ > 0) {
        takeDamage(monster.getAttackPower());
        monster.takeDamage(attack_);
        return health_ == 0;
    }

    return false;
}

std::shared_ptr<Teenager> createTeenager(types::HealthPoints health, types::Age age) {
    return std::make_shared<Teenager>(Teenager(health, age));
}

std::shared_ptr<Adult> createAdult(types::HealthPoints health, types::Age age) {
    return std::make_shared<Adult>(Adult(health, age));
}

std::shared_ptr<Sheriff> createSheriff(types::HealthPoints health, types::Age age,
                    types::AttackPower attack) {
    return std::make_shared<Sheriff>(Sheriff(health, age, attack));
}

