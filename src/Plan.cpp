#include "../include/Plan.h"
#include "../include/Facility.h"
using namespace std;
#include <iostream> 
//Plan::

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
    _plan_id(planId),
    _settlement(settlement),
    _selectionPolicy(selectionPolicy),
    _status(PlanStatus::AVALIABLE),
    _facilities({}),
    _underConstruction({}),
    _facilityOptions(facilityOptions),
    _life_quality_score(0),
    _economy_score(0),
    _environment_score(0)
    
{};


const int Plan::getlifeQualityScore() const{
    return this->_life_quality_score;
};

const int Plan::getEconomyScore() const{
    return this->_economy_score;
};

const int Plan::getEnvironmentScore() const{
    return this->_environment_score;
};

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    this->_selectionPolicy = selectionPolicy;
};

void Plan::step(){
    // if plan is busy => step all of them
    // if not, step all underconstruction facilities and build another facility (check options for more than one)
    // check if some facility went OPERATIONAL them move it to facilities, change plan to avaliable.
    int i = 0;
    while(this->_status != PlanStatus :: BUSY && i < this->_settlement.getLimit()){
        this->addFacility(new Facility(this->_selectionPolicy->selectFacility(this->_facilityOptions),this->_settlement.getName()));
        if (static_cast<int>(this->_underConstruction.size()) >= _settlement.getLimit()){
            this->_status = PlanStatus :: BUSY;
        }
        i++;
    }

    auto facItr = _underConstruction.begin();
    while (facItr != _underConstruction.end()) {
        Facility* chosenFacility = *facItr;
        chosenFacility->step();
        if (chosenFacility->getStatus() == FacilityStatus::OPERATIONAL) {
            _facilities.push_back(chosenFacility);
            this->_economy_score += chosenFacility->getEconomyScore();
            this->_environment_score += chosenFacility->getEnvironmentScore();
            this->_life_quality_score += chosenFacility->getLifeQualityScore();
            facItr = _underConstruction.erase(facItr);
            this->_status = PlanStatus :: AVALIABLE;
        } else {
            ++facItr;
        }
    }
};

void Plan::printStatus(){
    if (this->_status == PlanStatus::BUSY){
        cout<<"BUSY"<<endl;
    }
    else{
        cout<<"AVALIABLE"<<endl;
    }    
};

const vector<Facility*> &Plan::getUnderConsturction() const{
    return this->_underConstruction;
};

const vector<Facility*> &Plan::getFacilities() const{
    return this->_facilities;
};

void Plan::addFacility(Facility* facility){
    this->_underConstruction.push_back(facility);
};

const int Plan::getPlanId() const{
    return this->_plan_id;
};

const string Plan::getSelectionPolicy() const{
    return this->_selectionPolicy->toString();
}

const string Plan::toString() const{
    std :: string status;
    if (this->_status == PlanStatus::BUSY){
        status = "BUSY";
    }
    else{
        status = "AVALIABLE";
    }

    return "PlanID: " + std::to_string(this->_plan_id) + 
    "\nSettlementName: "+ this->_settlement.getName() +
    "\nPlanStatus: " + status +
    "\nSelectionpolicy: " + this->_selectionPolicy->toString() +
    "\nLifeQualityScore: " + std::to_string(this->getlifeQualityScore()) + 
    "\nEconomyScore: " + std::to_string(this->getEconomyScore()) + 
    "\nEnvironmentScore: " + std::to_string(this->getEnvironmentScore());
};

Plan::Plan(const Plan& other):
    _plan_id(other._plan_id),
    _settlement(other._settlement),
    _selectionPolicy(other._selectionPolicy->clone()),
    _status(other._status),
    _facilities(),  // Start empty
    _underConstruction(),  // Start empty
    _facilityOptions(other._facilityOptions),
    _life_quality_score(other._life_quality_score),
    _economy_score(other._economy_score),
    _environment_score(other._environment_score){
    for(Facility* facility : other._facilities) {
        _facilities.push_back(new Facility(*facility));
    }
    for(Facility* facility : other._underConstruction) {
        _underConstruction.push_back(new Facility(*facility));
    }
}

Plan& Plan::operator=(const Plan& other) {
    if (this != &other) {
        for(Facility* facility : _facilities) {
            delete facility;
        }
        for(Facility* facility : _underConstruction) {
            delete facility;
        }
        delete _selectionPolicy;
        _facilities.clear();
        _underConstruction.clear();
        _selectionPolicy = other._selectionPolicy->clone();
        _status = other._status;
        _life_quality_score = other._life_quality_score;
        _economy_score = other._economy_score;
        _environment_score = other._environment_score;
        for(Facility* facility : other._facilities) {
            _facilities.push_back(new Facility(*facility));
        }
        for(Facility* facility : other._underConstruction) {
            _underConstruction.push_back(new Facility(*facility));
        }
    }
    return *this;
}


Plan :: ~Plan() {
    delete _selectionPolicy;
    for(Facility* facility : _facilities) {
        delete facility;
    }
    for(Facility* facility : _underConstruction) {
        delete facility;
    }
}