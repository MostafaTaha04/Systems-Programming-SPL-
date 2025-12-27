#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        vector<Facility*> &getFacilities1();
        vector<Facility*> &getUnderconstruction();
        void addFacility(Facility* facility);
        const string toString() const;
        //helper function
        int getplanId();
        SelectionPolicy *getSelectionPolicy();
        const string strStatus() const;
        string getName();  
        
        Plan(const Plan &other); // copy constructor
        Plan& operator=(const Plan &other); // copy assignment
        ~Plan(); // destructor


    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;

        // Auxiliary methods for Rule of 5
        void deepCopy(const Plan &other);
        void cleanUp();

};