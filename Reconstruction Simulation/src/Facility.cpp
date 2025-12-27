#include "Facility.h"

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
: name(name), category(category),price(price),lifeQuality_score(lifeQuality_score),economy_score(economy_score),environment_score(environment_score)
{}

const string &FacilityType::getName() const
{
    return name;
}

int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}

int FacilityType::getEconomyScore() const
{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
: FacilityType( name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price)
{}

Facility::Facility(const FacilityType &type, const string &settlementName)
: FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()) {}

const string &Facility::getSettlementName() const
{
    return name;
};

const int Facility::getTimeLeft() const
{
    return timeLeft;
}

void Facility::step()
{
    if(timeLeft>0)
        timeLeft=timeLeft-1;
    if(timeLeft==0){
        status = FacilityStatus ::OPERATIONAL;
    }
}

void Facility::setStatus(FacilityStatus status)
{   
    if(timeLeft!=0)
        status = FacilityStatus ::UNDER_CONSTRUCTIONS;
    status = FacilityStatus ::OPERATIONAL;   
}

const FacilityStatus &Facility::getStatus() const
{
    return status;
}

const string Facility::toString() const 
{
    string strstatus;

    if (getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS)
        strstatus = "UNDER_CONSTRUCTION";
    else
        strstatus = "OPERATIONAL";

    string strcategory;

    if (category == FacilityCategory::LIFE_QUALITY)
        strcategory = "Life Quality";
    else if (category == FacilityCategory::ECONOMY)
        strcategory = "Economy";
    else if (category == FacilityCategory::ENVIRONMENT)
        strcategory = "Environment";

    return "FacilityName: " + getSettlementName() + "\n"+"FacilityStatus: " + strstatus;
}