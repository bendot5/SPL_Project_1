#include "../include/Simulation.h"
using std::string;
using std::vector;
using namespace std;
#include <iostream> 
#include "../include/Auxiliary.h"
#include <fstream>
#include <string>
#include "../include/Settlement.h"
#include "../include/Action.h"

class BaseAction;
class SelectionPolicy;

Simulation::Simulation(const string &configFilePath): isRunning(false), planCounter(0),
actionsLog({}), plans({}), settlements({}), facilitiesOptions({}){
    std::ifstream file(configFilePath);
    std::string line;
    vector<std::string> arguments;
    while (std::getline(file, line)) {
         arguments = Auxiliary::parseArguments(line);
         if (arguments[0][0] != '#' && arguments[0] == "settlement" && !isSettlementExists(arguments[1])){
            Settlement *s1 = new Settlement(arguments[1], static_cast<SettlementType>(std::stoi(arguments[2])));
            this->addSettlement(s1);
         }
         if (arguments[0][0] != '#' && arguments[0] == "facility" && !isFacilityExists(arguments[1])){
            FacilityType f1(arguments[1], static_cast<FacilityCategory>(std::stoi(arguments[2])), 
                   std::stoi(arguments[3]), std::stoi(arguments[4]), 
                   std::stoi(arguments[5]), std::stoi(arguments[6]));
            this->addFacility(f1);
         }
         if (arguments[0][0] != '#' && arguments[0] == "plan" && isSettlementExists(arguments[1]) && isPolicyExists(arguments[2])){
            if (arguments[2]=="env"){
                SustainabilitySelection *policy = new SustainabilitySelection();
                this->addPlan(this->getSettlement(arguments[1]), policy);
            }
            else if (arguments[2]=="nve"){
                NaiveSelection *policy = new NaiveSelection();
                this->addPlan(this->getSettlement(arguments[1]), policy);
            }
            else if (arguments[2]=="eco"){
                EconomySelection *policy = new EconomySelection();
                this->addPlan(this->getSettlement(arguments[1]), policy);

            }
            else {
                BalancedSelection *policy = new BalancedSelection(0, 0, 0);
                this->addPlan(this->getSettlement(arguments[1]), policy);
            }      
         }

         
    }
};

Simulation::Simulation(const Simulation& other): 
    isRunning(other.isRunning),
    planCounter(other.planCounter),
    actionsLog(),
    plans(other.plans),
    settlements(),
    facilitiesOptions(other.facilitiesOptions){
    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
}


void Simulation::start(){
    this->isRunning = true;
    cout << "The simulation has started" << endl;    
    this->getInput();
};

void Simulation::getInput() {
   string input;
   while (this->isRunning) {
       getline(cin, input);
       vector<string> args = Auxiliary::parseArguments(input);
       BaseAction* action = nullptr;
       
       if (args[0] == "settlement") {
           action = new AddSettlement(args[1], static_cast<SettlementType>(stoi(args[2])));
       }
       else if (args[0] == "facility") {
           action = new AddFacility(args[1], static_cast<FacilityCategory>(stoi(args[2])), 
                                  stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]));
       }
       else if (args[0] == "plan") {
           action = new AddPlan(args[1], args[2]);
       }
       else if (args[0] == "step") {
           action = new SimulateStep(stoi(args[1]));
       }
       else if (args[0] == "planStatus") {
           action = new PrintPlanStatus(stoi(args[1]));
       }
       else if (args[0] == "changePolicy") {
           action = new ChangePlanPolicy(stoi(args[1]), args[2]);
       }
       else if (args[0] == "backup") {
           action = new BackupSimulation();
       }
       else if (args[0] == "restore") {
           action = new RestoreSimulation();
       }
       else if (args[0] == "log") {
           action = new PrintActionsLog();
       }
       else if (args[0] == "close") {
           action = new Close();
       }

       if (action) {
           action->act(*this); 
       }
   }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    if (this->isSettlementExists(settlement.getName()) && isPolicyExists(selectionPolicy->toString())){
        this->plans.push_back(Plan(this->planCounter, settlement, selectionPolicy, this->facilitiesOptions));
        this->planCounter++;
    }
    else{
        cout << "Cannot create this plan" << endl;
    }
};

void Simulation::addAction(BaseAction *action){
    this->actionsLog.push_back(action);
};

bool Simulation::addSettlement(Settlement *settlement){
    if(isSettlementExists(settlement->getName())){
        cout << "Settlement already exists" << endl;
        return false;
    }
    else{
        this->settlements.push_back(settlement);
        return true;
    }
};

bool Simulation::addFacility(FacilityType facility){ // check
    if (!this->isFacilityExists(facility.getName())){
        this->facilitiesOptions.push_back(facility); 
        return true;
    }
    else {
        cout << "Facility already exists" << endl;
        return false;
    }
};

bool Simulation::isSettlementExists(const string &settlementName){
    for (Settlement* set : this->settlements){
        if (set->getName() == settlementName){
            return true;
        }
    }
    return false;    
};

bool Simulation::isFacilityExists(const string &facilityName){
    for (FacilityType fac : this->facilitiesOptions){
        if (fac.getName() == facilityName){
            return true;
        }
    }
    return false;    
};

bool Simulation::isPolicyExists(const string &policyName){
    if((policyName == "nve")||(policyName == "env")||(policyName == "eco")||(policyName == "bal")){
        return true;
    }    
    return false;
};

bool Simulation::isPlanExists(const int planID){
    return planID < this->planCounter;
};

Settlement &Simulation::getSettlement(const string &settlementName){
    for (Settlement* set : this->settlements){
        if (set->getName() == settlementName){
            return *set;
        } 
    }
    return (*settlements[0]);
};

Plan &Simulation::getPlan(const int planID){
    for (Plan& plan : this->plans){
        if(plan.getPlanId() == planID){
            return plan;
        }
    }
    return plans[0];
};

void Simulation::step(){
    for (Plan& plan:this->plans){
        plan.step();
    }
};

void Simulation::close(){
    for (Plan plan : this->plans){
        cout << plan.toString() << endl;
    }
    this->isRunning = false;
};
void Simulation::open(){
    this->isRunning = true;
};

const vector<BaseAction*> Simulation::getActionsLog() const{
    return this->actionsLog;
};

Simulation& Simulation::operator=(const Simulation& other) {
    if (this != &other) {
        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        settlements.clear();
        actionsLog.clear();
        plans.clear();
        facilitiesOptions.clear();
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for (Settlement* settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        plans = other.plans;
        for (const FacilityType& facility : other.facilitiesOptions) {
            facilitiesOptions.push_back(FacilityType(
                facility.getName(), 
                facility.getCategory(),
                facility.getCost(),
                facility.getLifeQualityScore(),
                facility.getEconomyScore(),
                facility.getEnvironmentScore()
            ));
        }
    }
    return *this;
}

Simulation::~Simulation(){
    for (Settlement* set : this->settlements){
        delete set;
    }
    for (BaseAction *actionlog : this->actionsLog){
        delete actionlog;
    }
};


