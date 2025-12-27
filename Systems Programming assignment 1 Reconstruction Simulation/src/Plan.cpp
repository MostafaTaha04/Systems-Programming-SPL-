#include <vector>
#include <iostream>
#include "../include/Plan.h"
#include "../include/SelectionPolicy.h"
#include "../include/Action.h"
#include "Settlement.h"
#include "Plan.h"
using namespace std;

string Plan :: getName(){
    return settlement.getName();
}

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),
      facilities(), underConstruction(), facilityOptions(facilityOptions),
      life_quality_score(0), economy_score(0), environment_score(0) {}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    delete this->selectionPolicy;
    this->selectionPolicy=selectionPolicy;
}

void Plan::step() {
    if(this->status==PlanStatus::AVALIABLE){
        while ((int)underConstruction.size() < this->settlement.getCapacity()) {
            Facility* facility = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
            addFacility(facility);
        }
    }
    
    for (size_t i = 0; i < underConstruction.size(); i++) { 
        underConstruction[i]->step();
        if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL) {
            facilities.push_back(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
            life_quality_score=life_quality_score + underConstruction[i]-> getLifeQualityScore();
            economy_score = economy_score + underConstruction[i]-> getEconomyScore();
            environment_score = environment_score + underConstruction[i]-> getEnvironmentScore();
        }
    }

    if ((int)underConstruction.size() < settlement.getCapacity()) {
        this-> status = PlanStatus:: AVALIABLE;
    }
    else
        this-> status = PlanStatus:: BUSY;
}

void Plan::printStatus() 
{
    cout<<toString()<<endl;
    for(Facility* Facility: facilities)
        cout<<Facility->toString()<<endl;
    for(Facility* facility: underConstruction)
        cout<<facility->toString()<<endl;
}

const  vector<Facility*> &Plan:: getFacilities() const
{
    return facilities;
}

void Plan::addFacility(Facility* facility)
{
    if(status == PlanStatus::AVALIABLE){
        underConstruction.push_back(facility);
    } else {
        delete facility;
    }
}
const string Plan::strStatus()const{
    if (status == PlanStatus::AVALIABLE) {
        return "AVALIABLE";
    } else if (status == PlanStatus::BUSY) {
        return "BUSY";
    }
    return nullptr;
}

const string Plan::toString() const {
    return "PlanID: " + std::to_string(plan_id) + 
           "\n" + "SettlementName: " + settlement.getName() +
           "\n" + "PlanStatus: " + strStatus() + 
           "\n" + "SelectionPolicy: " + selectionPolicy->toString() +
           "\n" + "LifeQualityScore: " + std::to_string(life_quality_score) +
           "\n" + "EconomyScore: " + std::to_string(economy_score) +
           "\n" + "EnvironmentScore: " + std::to_string(environment_score);
}
int Plan::getplanId()
{
    return this->plan_id;
}
SelectionPolicy* Plan::getSelectionPolicy()
{
    return selectionPolicy;
}

// Destructor
Plan::~Plan() 
{
    cleanUp();
}

// Copy constructor
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(nullptr),
      status(other.status), facilities(), underConstruction(),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),environment_score(other.environment_score) 
{
    deepCopy(other);
}
Plan& Plan::operator=(const Plan& other) {
    if (this != &other) {
        cleanUp();
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;

        if (other.selectionPolicy) {
            selectionPolicy = other.selectionPolicy->clone();
        }

        for (Facility* facility : other.facilities) {
            facilities.push_back(new Facility(*facility));
        }

        for (Facility* facility : other.underConstruction) {
            underConstruction.push_back(new Facility(*facility));
        }
    }
    return *this;
}

void Plan::deepCopy(const Plan &other) {
    if (other.selectionPolicy) {
        selectionPolicy = other.selectionPolicy->clone();
    }

    for (Facility* facility : other.facilities) {
    facilities.push_back(new Facility(*facility));
    }
    for (Facility* facility : other.underConstruction) {
    underConstruction.push_back(new Facility(*facility));
    }
    
}

void Plan::cleanUp()
{
    delete selectionPolicy;
    
    for (Facility* facility : underConstruction) {
        delete facility;
        facility = nullptr;
    }
    for (Facility* facility : facilities) {
        delete facility;
        facility = nullptr;
    }
    facilities.clear();
    underConstruction.clear();
}


vector<Facility*> &Plan::getFacilities1(){
    return facilities;
}

vector<Facility*> &Plan::getUnderconstruction(){
    return underConstruction;
}