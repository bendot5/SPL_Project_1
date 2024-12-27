#include "../include/Facility.h"
#include <string>

//FacilityType::

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
    _name(name),
    _category(category),
    _price(price),
    _lifeQuality_score(lifeQuality_score),
    _economy_score(economy_score),
    _environment_score(environment_score){};

const string &FacilityType::getName() const{
    return this->_name;
};

int FacilityType::getCost() const{
    return this->_price;
};

int FacilityType::getLifeQualityScore() const{
    return this->_lifeQuality_score;
};

int FacilityType::getEnvironmentScore() const{
    return this->_environment_score;
};

int FacilityType::getEconomyScore() const{
    return this->_economy_score;
};

FacilityCategory FacilityType::getCategory() const{
    return this->_category;
};


//Facility::

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
    FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
    _settlementName(settlementName),
    _status(FacilityStatus::UNDER_CONSTRUCTIONS),
    _timeLeft(price)
{};
        
Facility::Facility(const FacilityType &type, const string &settlementName):
    FacilityType(type),
    _settlementName(settlementName),
    _status(FacilityStatus::UNDER_CONSTRUCTIONS),
    _timeLeft(type.getCost())
{};


        
const string &Facility::getSettlementName() const{
    return this->_settlementName;
};

const int Facility::getTimeLeft() const{
    return this->_timeLeft;
};

// needs to reduce the time left by 1
// if time left is 0, change the status to operational
FacilityStatus Facility::step(){
    if (this->_timeLeft != 0){
        this->_timeLeft--;
    }
    if (this->_timeLeft == 0){
        this->setStatus(FacilityStatus :: OPERATIONAL);
    }
    return this->_status;    
};

void Facility::setStatus(FacilityStatus status){
    this->_status = status;
};

const FacilityStatus& Facility::getStatus() const{
    return this->_status;
};

const string Facility::toString() const{
    std :: string facStat;
     switch (this->getStatus()) {
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            facStat = "UNDER_CONSTRUCTION";
            break;
        case FacilityStatus::OPERATIONAL:
            facStat = "OPERATIONAL";
            break;
    }


    //  facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
    return "FacilityName: " + this->getName() + 
    "\nFacilityStatus: " + facStat;
};

const string Facility::specialToString() const{
    std :: string facCat;
    switch (this->getCategory()) {
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
    std :: string numStatus;
    switch (this->getStatus()) {
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            numStatus = "0";
            break;
        case FacilityStatus::OPERATIONAL:
            numStatus = "1";
            break;
        default:
            numStatus = "-1";
    }
    //  facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
    return "facility " + this->_name + " " + facCat + " " + std :: to_string(getCost()) + " " + 
    std :: to_string(this->getLifeQualityScore()) + " " + std :: to_string(this->getEconomyScore()) + " " + 
    std :: to_string(this->getEnvironmentScore()) + " status: " + numStatus + " time left:" +
    std::to_string(this->getTimeLeft());
};
