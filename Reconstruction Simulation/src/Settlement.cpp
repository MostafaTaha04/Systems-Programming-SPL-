#include "../include/Settlement.h"

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const 
{    
    string strtype;
    
    if (type == SettlementType::VILLAGE)
        strtype = "Village";
    else if (type == SettlementType::CITY)
        strtype = "City";
    else if (type == SettlementType::METROPOLIS)
        strtype = "Metropolis";

    return "SettlementName: " + name + ", SettlementType: " + strtype;
}
int Settlement::getCapacity() const{
    if (type == SettlementType:: VILLAGE) {
        return 1;
    }
    if (type == SettlementType:: CITY) {
        return 2;
    }
    if (type == SettlementType:: METROPOLIS) {
        return 3;
    }
    return -1;
}