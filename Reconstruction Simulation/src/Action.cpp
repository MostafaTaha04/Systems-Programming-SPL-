#include "../include/Action.h"
#include <iostream>

string statusStr(ActionStatus status){
    if (status == ActionStatus::COMPLETED){
        return " COMPLETED";
    }
    if (status == ActionStatus::ERROR){
        return " ERROR";
    }
    return nullptr;
}

BaseAction::BaseAction()
: errorMsg(), status(){}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "\nError: " << errorMsg << std::endl;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps)
: numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const
{
    return "step " + std::to_string(numOfSteps) + statusStr(getStatus());
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
: settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy* policy = simulation.getSelectionPolicy(selectionPolicy);
    if (!simulation.isSettlementExists(settlementName) || policy == nullptr){
        error("Cannot create this plan");
    } else {
        Settlement &settlement = simulation.getSettlement(settlementName);
        simulation.addPlan(settlement, policy);
        complete();
    }
}

const string AddPlan::toString() const
{
    return "plan " + settlementName + " " + selectionPolicy + statusStr(getStatus());
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
: settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation)
{
    if (simulation.addSettlement(new Settlement(settlementName, settlementType)))
        complete();
    else
        error("Settlement already exists");
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const
{
    return "settlement " + settlementName + " " + std::to_string((int) settlementType) + statusStr(getStatus());
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
: facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation)
{
    if (simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore)))
        complete(); 
    else
        error("Facility already exists");
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

const string AddFacility::toString() const
{
    return "facility " + facilityName + " " + std::to_string((int)facilityCategory) + " " + std::to_string(price) 
            + " " + std::to_string(lifeQualityScore)  + " " + std::to_string(economyScore)  + " " + std::to_string(environmentScore)
            + statusStr(getStatus());
}

PrintPlanStatus::PrintPlanStatus(int planId)
: planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation)
{   
    if((int) simulation.getPlans().size() > planId && planId>=0)
    {
        simulation.getPlan(planId).printStatus();
        complete();
    }
    else
    {
        error("Plan does not exist");
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const
{
    return "planStatus " + std::to_string(planId) + statusStr(getStatus());
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
: planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation)
{
    Plan* planPtr = &simulation.getPlan(planId);
    Plan &plan = *planPtr;
    if (plan.getSelectionPolicy() == nullptr || plan.getSelectionPolicy()->toString() != newPolicy)
    {
        SelectionPolicy* policy = simulation.getSelectionPolicy(newPolicy);
        plan.setSelectionPolicy(policy);
        complete();
    }
    else
        error("Cannot change selection policy");
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const
{
    return "changePlan " + std::to_string(planId) + " " + newPolicy + statusStr(getStatus());
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation)
{
    for (BaseAction* action : simulation.getActionLog())
        std::cout << action->toString() << std::endl;
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const
{
    return "log" + statusStr(getStatus());
}

Close::Close() {}

void Close::act(Simulation &simulation)
{
    simulation.close();
    complete();
}

Close *Close::clone() const
{
    return new Close(*this);
}

const string Close::toString() const
{
    return "close" + statusStr(getStatus());
}

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation)
{
    if(backup != nullptr){
        delete backup;
    }
    backup = new Simulation(simulation);
    complete();
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const
{
    return "backup" + statusStr(getStatus());
}

RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        error("no backup available");
    }
    else
    {
        simulation = *backup;
        complete();
    }
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const
{
    return "restore" + statusStr(getStatus());
}