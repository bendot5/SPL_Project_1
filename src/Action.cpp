#include "../include/Action.h"
enum class SettlementType;
enum class FacilityCategory;
using namespace std;
#include <iostream>
extern Simulation* backup;

BaseAction::BaseAction(): _errorMsg(""), _status(ActionStatus::COMPLETED){};
ActionStatus BaseAction::getStatus() const{
    return this->_status;
};
const string BaseAction::getStatusString() const{
    if (this->_status == ActionStatus::COMPLETED){
        return "COMPLETED";
    }
    else{
        return "ERROR";
    }
};
void BaseAction::complete(){
    this->_status = ActionStatus::COMPLETED;
};
void BaseAction::error(string errorMsg){
    this->_errorMsg = errorMsg;
    this->_status = ActionStatus::ERROR;
    cout << "ERROR: " + this->getErrorMsg() << endl;
};
const string &BaseAction::getErrorMsg() const{
    return this->_errorMsg;
};

SimulateStep::SimulateStep(const int numOfSteps): _numOfSteps(numOfSteps) {};
void SimulateStep::act(Simulation &simulation){
    for (int i = 0; i < this->_numOfSteps; i++){
        simulation.step();
    }       
    this->complete();
    simulation.addAction(this);
};
const string SimulateStep::toString() const{
    return "step " + to_string(this->_numOfSteps) + " " + this->getStatusString();
};
SimulateStep *SimulateStep::clone() const{
    return new SimulateStep(*this);
};

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy): _settlementName(settlementName), _selectionPolicy(selectionPolicy) {};
void AddPlan::act(Simulation &simulation){
    bool setExist = simulation.isSettlementExists(this->_settlementName);
    if (!setExist){
        this->error("Cannot create this plan");
    }
    else {
        if (this->_selectionPolicy=="env"){
            SustainabilitySelection *policy = new SustainabilitySelection();
            simulation.addPlan(simulation.getSettlement(this->_settlementName), policy);
            this->complete();
        }
        else if (this->_selectionPolicy=="nve"){
            NaiveSelection *policy = new NaiveSelection();
            simulation.addPlan(simulation.getSettlement(this->_settlementName), policy);
            this->complete();
        }
        else if (this->_selectionPolicy=="eco"){
            EconomySelection *policy = new EconomySelection();
            simulation.addPlan(simulation.getSettlement(this->_settlementName), policy);
            this->complete();
        }
        else if (this->_selectionPolicy=="bal"){
            BalancedSelection *policy = new BalancedSelection(0, 0, 0);
            simulation.addPlan(simulation.getSettlement(this->_settlementName), policy);
            this->complete();
        }
        else {
            this->error("Cannot create this plan");
        } 
    }
    simulation.addAction(this);
};
const string AddPlan::toString() const{
    return "plan " + this->_settlementName + " " + this->_selectionPolicy + " " + this->getStatusString();
};
AddPlan *AddPlan::clone() const{
    return new AddPlan(*this);
};

AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType): 
_settlementName(settlementName),
_settlementType(settlementType)
{};
void AddSettlement::act(Simulation &simulation){
    bool exist = simulation.isSettlementExists(this->_settlementName);
    if (exist){
        this->error("settlement already exists"); //Settlement already exist has been printed in the function isSettlementExist
    }
    else{
        simulation.addSettlement(new Settlement(this->_settlementName, this->_settlementType));
        this->complete();
    }
    simulation.addAction(this);
};
AddSettlement *AddSettlement::clone() const{
    return new AddSettlement(*this);
};
const string AddSettlement::toString() const{
    std::string setType;
    switch (this->_settlementType) {
        case SettlementType::VILLAGE:
            setType = "0";
            break;
        case SettlementType::CITY:
            setType = "1";
            break;
        case SettlementType::METROPOLIS:
            setType = "2";
            break;
        default:
            setType = "-1";
}

    return "settlement " + this->_settlementName + " " + setType + " " + this->getStatusString();
};


AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore): 
    _facilityName(facilityName),
    _facilityCategory(facilityCategory),
    _price(price),
    _lifeQualityScore(lifeQualityScore),
    _economyScore(economyScore),
    _environmentScore(environmentScore){};
void AddFacility::act(Simulation &simulation){
    bool exist = simulation.isFacilityExists(this->_facilityName);
    if(exist){
        this->error("Facility already exists");
    }
    else{
        simulation.addFacility(FacilityType(this->_facilityName, this->_facilityCategory, this->_price, this->_lifeQualityScore, this->_economyScore, this->_environmentScore)); //here
        this->complete();
    }
    simulation.addAction(this);
};
AddFacility *AddFacility::clone() const {
        return new AddFacility(*this);
};
const string AddFacility::toString() const{
    std :: string facCat;
    switch (this->_facilityCategory) {
        case FacilityCategory::LIFE_QUALITY:
            facCat = "0";
            break;
        case FacilityCategory::ECONOMY:
            facCat = "1";
            break;
        case FacilityCategory::ENVIRONMENT:
            facCat = "2";
            break;
        default:
            facCat = "-1";
    }
    return "facility " + this->_facilityName + " " + facCat + " " + std :: to_string(this->_price) + " " + 
    std :: to_string(this->_lifeQualityScore) + " " + std :: to_string(this->_economyScore) + " " + 
    std :: to_string(this->_environmentScore) + " " + this->getStatusString();
};

PrintPlanStatus::PrintPlanStatus(int planId): 
_planId(planId){};
void PrintPlanStatus::act(Simulation &simulation){
    bool exist = simulation.isPlanExists(this->_planId);
    if(exist){
        cout << simulation.getPlan(this->_planId).toString() << endl;
        for (Facility *facility : simulation.getPlan(this->_planId).getFacilities()){
            cout << facility->toString() << endl;
        }   
        for (Facility *underConstruction : simulation.getPlan(this->_planId).getUnderConsturction()){
            cout << underConstruction->toString() << endl;
        }  
        this->complete();
    }
    else{
        this->error("Plan doesn't exist");
    }
    simulation.addAction(this);
};
PrintPlanStatus *PrintPlanStatus::clone() const{
    return new PrintPlanStatus(*this);
};
const string PrintPlanStatus::toString() const{
    return "printPlanStatus " + to_string(this->_planId) + " " + this->getStatusString(); 
};

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
    _planId(planId),
    _newPolicy(newPolicy){};
void ChangePlanPolicy::act(Simulation &simulation){
    if(simulation.isPlanExists(this->_planId)){
        if(simulation.getPlan(this->_planId).getSelectionPolicy() != this->_newPolicy){
            if (this->_newPolicy=="env"){
            SustainabilitySelection *policy = new SustainabilitySelection();
            simulation.getPlan(this->_planId).setSelectionPolicy(policy);
            this->complete();
            }
            else if (this->_newPolicy=="nve"){
            NaiveSelection *policy = new NaiveSelection();
            simulation.getPlan(this->_planId).setSelectionPolicy(policy);
            this->complete();
            }
            else if (this->_newPolicy=="eco"){
            EconomySelection *policy = new EconomySelection();
            simulation.getPlan(this->_planId).setSelectionPolicy(policy);
            this->complete();
            }
            else{
            BalancedSelection *policy = new BalancedSelection(0, 0, 0);
            simulation.getPlan(this->_planId).setSelectionPolicy(policy);
            this->complete();
            }
        }
        else{
            this->error("Cannot change selection policy");
        }     
    }
    else{
        this->error("Cannot change selection policy");
    }
    simulation.addAction(this);
};
ChangePlanPolicy *ChangePlanPolicy::clone() const{
    return new ChangePlanPolicy(*this);
};
const string ChangePlanPolicy::toString() const{
    return "changePolicy " + std::to_string(this->_planId) + " " + this->_newPolicy + " " + this->getStatusString();
};

PrintActionsLog::PrintActionsLog(){};
void PrintActionsLog::act(Simulation &simulation){
    for (BaseAction *baseAction : simulation.getActionsLog()){
        cout << baseAction->toString() << endl;
    }
    this->complete();  
    simulation.addAction(this);
};
PrintActionsLog *PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
};
const string PrintActionsLog::toString() const{
    return "printActionsLog " + this->getStatusString();
};

Close::Close(){};
void Close::act(Simulation &simulation){
    simulation.close();
    this->complete();
    simulation.addAction(this);
};
Close *Close::clone() const{
    return new Close(*this);
};
const string Close::toString() const{
    return "close " + this->getStatusString();
};

BackupSimulation::BackupSimulation(){};
void BackupSimulation::act(Simulation &simulation){
    if (backup==nullptr){
        backup = new Simulation(simulation);
    }
    else{
        *backup = simulation;
    }
    this->complete();
    simulation.addAction(this);
    
};
BackupSimulation *BackupSimulation::clone() const{
    return new BackupSimulation(*this);

};
const string BackupSimulation::toString() const{
    return "BackupSimulation " + this->getStatusString();
};

RestoreSimulation::RestoreSimulation(){};
void RestoreSimulation::act(Simulation &simulation) {
    if(backup == nullptr) {
        this->error("No backup available");
    }
    else {
        // Complete the action before any changes
        this->complete();
        simulation.addAction(this);
        
        // Perform the restore
        simulation = *backup;
    }
}

RestoreSimulation *RestoreSimulation::clone() const{
    return new RestoreSimulation(*this);
};
const string RestoreSimulation::toString() const{
    return "RestoreSimulation " + this->getStatusString();
};
