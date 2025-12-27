#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>
#include "../include/Simulation.h"
#include "../include/Plan.h"
#include "../include/Auxiliary.h"
#include "Simulation.h"
using namespace std;


Simulation:: Simulation(const string &configFilePath):isRunning(false), planCounter(0),actionsLog(),plans(),settlements(),facilitiesOptions(){
    std::ifstream inputFile(configFilePath);

    string line;
    while (std::getline(inputFile, line)) {
        if (!line.empty() && line[0] != '#') {
            vector<string> args = Auxiliary:: parseArguments(line);
            if (!args.empty() && args[0] == "settlement" && args.size() == 3) {
                this->addSettlement(new Settlement(args[1], static_cast<SettlementType>(std::stoi(args[2]))));
            }
            if(!args.empty() && args[0] == "facility" && args.size() == 7) {
                this-> addFacility(std::move(FacilityType(args[1], (FacilityCategory)std::stoi(args[2]), std::stoi(args[3]), std::stoi(args[4]), std::stoi(args[5]), std::stoi(args[6]))));
            }
            if(!args.empty() && args[0] == "plan" && args.size() == 3) {
                this-> addPlan(getSettlement(args[1]), getSelectionPolicy(args[2]));
            }
        }   
    }
    inputFile.close();
}

void Simulation::start()
{
    std::cout << "The simulation has started\n";
    isRunning = true;

    while (isRunning)
    {
        std::string line;
        std::getline(std::cin, line);

        std::vector<std::string> args = Auxiliary::parseArguments(line);
        if (args.empty())
        {
            continue;
        }

        std::string choose = args[0];
        BaseAction *action = nullptr;

        
        if (choose == "step")
        {
            int numOfSteps = std::stoi(args[1]);
            action = new SimulateStep(numOfSteps);
        }
        else if (choose == "plan")
        {
            std::string settlement_name = args[1];
            std::string selection_policy = args[2];
            action = new AddPlan(settlement_name, selection_policy);
        }
        else if (choose == "settlement")
        {
            std::string settlement_name = args[1];
            SettlementType settlement_type = static_cast<SettlementType>(std::stoi(args[2]));
            action = new AddSettlement(settlement_name, settlement_type);
        }
        else if (choose == "facility")
        {
            std::string facilityName = args[1];
            int price = std::stoi(args[3]);
            int lifeQuality = std::stoi(args[4]);
            int economy = std::stoi(args[5]);
            int environment = std::stoi(args[6]);
            action = new AddFacility(facilityName, FacilityCategory(std::stoi(args[2])) , price, lifeQuality, economy, environment);
        }
        else if (choose == "planStatus")
        {
            int planId = std::stoi(args[1]);
            action = new PrintPlanStatus(planId);
        }
        else if (choose == "changePolicy")
        {
            int planId = std::stoi(args[1]);
            std::string newPolicy = args[2];
            action = new ChangePlanPolicy(planId, newPolicy);
        }
        else if (choose == "log")
        {
            action = new PrintActionsLog();
        }
        else if (choose == "backup")
        {
            action = new BackupSimulation();
        }
        else if (choose == "restore")
        {
            action = new RestoreSimulation();
        }
        else if (choose == "close")
        {
            action = new Close();
            isRunning = false;
        }
        if(action !=nullptr){
        action->act(*this);
        actionsLog.push_back(action);
        }
        else {
            cout << "Unknown command: " << line << endl;
        }
    }
}


void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    plans.emplace_back(planCounter,settlement,selectionPolicy,facilitiesOptions);
    planCounter++;
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
   if (!isSettlementExists(settlement->getName()))
    {
        settlements.push_back(settlement);
        return true;
    } 
    delete settlement;
    return false;
}

bool Simulation::addFacility(FacilityType facility)
{
    for (FacilityType facilities: facilitiesOptions)
    {
        if(facilities.getName()==facility.getName())
            return false;
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for (Settlement *settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return true;
        }
    }
    return false;
}


Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (Settlement *settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return *settlement;
        }
    }
    static Settlement defaultSettlement;
    return defaultSettlement;
}


Plan &Simulation::getPlan(const int planID)
{
    for (Plan &plan : plans) {
        if (plan.getplanId()==planID) {
            return plan;
        }
    }
    static Plan defaultPlan(0, Settlement("Unknown", SettlementType::VILLAGE), nullptr, {});
    return defaultPlan;
}

void Simulation::step()
{
       for(Plan &plan: plans){
            plan.step();
       }
}

void Simulation::close()
{
    isRunning = false;
           for (Plan& plan : plans)
           {
            cout << "PlanID: " << std::to_string(plan.getplanId()) << endl;
            cout <<  "SettlementName: " << plan.getName() << endl;
            cout << "LifeQualityScore: " <<  std::to_string(plan.getlifeQualityScore()) << endl;
            cout << "EconomyScore: " <<  std::to_string(plan.getEconomyScore()) << endl;
            cout << "EnvironmentScore: " <<  std::to_string(plan.getEnvironmentScore()) << endl;
           }
}

void Simulation::open()
{
    isRunning = true;
}

SelectionPolicy* Simulation::getSelectionPolicy(const string &selectedPolicy)
{
    if(selectedPolicy == "nve")
        return new NaiveSelection();
    if(selectedPolicy == "bal")
        return new BalancedSelection(0,0,0);
    if(selectedPolicy == "eco")
        return new EconomySelection();
    if(selectedPolicy == "env")
        return new SustainabilitySelection();
    return nullptr;
}

vector<BaseAction*> Simulation:: getActionLog()
{
    return actionsLog;
}

vector<Plan> Simulation::getPlans()
{
    return plans;
}

//Rule of 5:
Simulation::Simulation(const Simulation& other)
: isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()
{
    deepCopy(other);
}

Simulation::Simulation(Simulation&& other) // move constructor
    : isRunning(other.isRunning), planCounter(other.planCounter),actionsLog(),plans(),settlements(),facilitiesOptions()
    {
    deepCopy(other);
    other.isRunning = false;
    other.planCounter = 0;
}


Simulation& Simulation::operator=(const Simulation& other) // copy assignment
{
    if (this != &other) {
        cleanUp();
        deepCopy(other);  
    }
    return *this;
};

// move assignment
Simulation& Simulation::operator=(Simulation&& other) {
    if (this != &other) {
        cleanUp();
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        plans = std::move(other.plans);
        settlements = std::move(other.settlements);
        facilitiesOptions = std::move(other.facilitiesOptions);
        actionsLog = std::move(other.actionsLog);

        other.isRunning = false;
        other.planCounter = 0;
    }
    return *this;
}


Simulation::~Simulation() // destructor
{
    cleanUp();
}



void Simulation::deepCopy(const Simulation& other) {
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    

    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    for (FacilityType facility : other.facilitiesOptions) {
        facilitiesOptions.push_back(facility);
    }

    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    for (Plan plan : other.plans) {
        Plan p(plan.getplanId(), getSettlement(plan.getName()), plan.getSelectionPolicy()->clone(), facilitiesOptions);
        for(Facility* f: plan.getFacilities()){
            p.getFacilities1().push_back(new Facility(*f));
        }
        for(Facility* f: plan.getUnderconstruction()){
            p.getUnderconstruction().push_back(new Facility(*f));
        }
        plans.push_back(p);
    }
}


void Simulation::cleanUp() {
    for (auto action : actionsLog) {
        delete action;
        action = nullptr;
    }

    for (auto settlement : settlements) {
        delete settlement;
        settlement=nullptr;
    }
    actionsLog.clear();
    settlements.clear();
    facilitiesOptions.clear();
    plans.clear();
}