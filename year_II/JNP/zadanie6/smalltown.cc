#include <iostream>
#include <exception>
#include "smalltown.h"

Status::Status(std::shared_ptr<Attacker> &attacker, unsigned int alive)
    : attacker_(attacker), alive_(alive) {}


types::HealthPoints Status::getMonsterHealth() const {
    return attacker_.lock()->getHealth();
}

std::string Status::getMonsterName() const {
    return attacker_.lock()->getMonsterName();
}

unsigned int Status::getAliveCitizens() const {
    return alive_;
}

SmallTown::SmallTown(types::Time clock,
                     types::Time start,
                     std::shared_ptr<Attacker> attacker,
                     std::vector<std::shared_ptr<Citizen>> citizens)
    : alive_(0), time_(start), clock_(clock), attacker_(attacker), citizens_(citizens), strategy_attack_() {
    for (auto &citizen : citizens) {
        if (citizen->getHealth() > 0) {
            alive_++;
        }
    }
}

void SmallTown::tick(types::Time timeStep) {
    bool a_ = attacker_->getHealth() > 0;
    bool c_ = alive_ > 0;

    if (a_ && !c_) {
        std::cout << "MONSTER WON" << std::endl;
        return;
    }

    if (!a_ && !c_) {
        std::cout << "DRAW" << std::endl;
        return;
    }

    if (!a_ && c_) {
        std::cout << "CITIZENS WON" << std::endl;
        return;
    }
    
    if (strategy_attack_.attack(time_)) {
        for (auto &citizen : citizens_) {
            if (attacker_->getHealth() > 0) {
                if (citizen->attack(*attacker_)) {
                    alive_--;
                }
            }
        }
    }
    time_ = (time_ + timeStep) % (clock_ + 1);
}

Status SmallTown::getStatus() {
    return Status(attacker_, alive_);
}

SmallTown::Builder::Builder()
    : time_set(false), clock_set(false),attacker_set(false), citizen_added(false) {
}

SmallTown::Builder SmallTown::Builder::maxTime(types::Time time) {
    clock_set = true;
    clock_ = time;
    return *this;
}

SmallTown::Builder SmallTown::Builder::startTime(types::Time time) {
    time_set = true;
    time_ = time;
    return *this;
}

SmallTown::Builder SmallTown::Builder::citizen(std::shared_ptr<Citizen> citizen) {
    citizen_added = true;
    if (previous_.find(citizen) == previous_.end()) {
        previous_.insert(citizen);
        citizens_.push_back(citizen);
    }
    return *this;
}

SmallTown::Builder SmallTown::Builder::monster(std::shared_ptr<Attacker> attacker) {
    attacker_set = true;
    attacker_ = attacker;
    return *this;
}

bool SmallTown::Builder::all_set() const {
    return attacker_set && citizen_added && time_set && clock_set;
}

SmallTown SmallTown::Builder::build() const {
    if (!all_set()) {
        throw std::invalid_argument("Values not set");
    }
    
    return SmallTown(clock_, time_, attacker_, citizens_);
}

