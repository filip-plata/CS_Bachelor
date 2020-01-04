#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <set>
#include <memory>
#include <string>
#include "helper.h"
#include "monster.h"
#include "citizen.h"

class Status {
private:
    std::weak_ptr<Attacker> attacker_;
    unsigned int alive_;
public:
    Status(std::shared_ptr<Attacker> &attacker, unsigned int alive);
    std::string getMonsterName() const;
    types::HealthPoints getMonsterHealth() const;
    unsigned int getAliveCitizens() const;
};

class Strategy {
public:
    bool attack(types::Time time) {
        return (time % 7 != 0) && (time % 3 == 0 || time % 13 == 0);
    }
};

class SmallTown {
private:
    unsigned int alive_;
    types::Time time_;
    types::Time clock_;
    std::shared_ptr<Attacker> attacker_;
    std::vector<std::shared_ptr<Citizen>> citizens_;
    Strategy strategy_attack_;
public:
    SmallTown() = delete;
    SmallTown(types::Time clock, types::Time start,
              std::shared_ptr<Attacker> attacker,
              std::vector<std::shared_ptr<Citizen>> citizens);
    Status getStatus();
    void tick(types::Time timeStep);
    
    class Builder {
    private:
        bool time_set;
        bool clock_set;
        bool attacker_set;
        bool citizen_added;
        std::set<std::shared_ptr<Citizen>> previous_;
        std::shared_ptr<Attacker> attacker_;
        types::Time time_;
        types::Time clock_;
        std::vector<std::shared_ptr<Citizen>> citizens_;
        bool all_set() const;
    public:
        Builder();
        SmallTown build() const;
        Builder startTime(types::Time time);
        Builder maxTime(types::Time time);
        Builder citizen(std::shared_ptr<Citizen> citizen);
        Builder monster(std::shared_ptr<Attacker> attacker);
    };
};

#endif /* SMALLTOWN_H */
