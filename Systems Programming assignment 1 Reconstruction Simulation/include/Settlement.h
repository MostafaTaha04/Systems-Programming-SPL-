#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        Settlement& operator=(const Settlement& other); //copy assignment
        const string &getName() const;
        SettlementType getType() const;
        const string toString() const;

        int getCapacity() const;
        //We added this for getSettlement
        Settlement() : name("unknown"), type(SettlementType::VILLAGE) {}


        private:
            const string name;
            SettlementType type;
};