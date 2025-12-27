#pragma once
#include <string>
#include <vector>
#include "../include/Facility.h"
#include "../include/Plan.h"
#include "../include/Settlement.h"
#include "../include/Action.h"

using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        //helper function
        SelectionPolicy *getSelectionPolicy(const string &selectedPolicy);
        vector<BaseAction *> getActionLog();
        vector<Plan> getPlans();

        //Rule of 5:
        Simulation(const Simulation &other); // copy constructor
        Simulation(Simulation&& other); // move constructor
        Simulation& operator=(const Simulation& other); // copy assignment
        Simulation& operator=(Simulation&& other); // move assignment
        ~Simulation(); // destructor

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

        //helper methods for rule of 5:
        void deepCopy(const Simulation& other);
        void cleanUp();
};