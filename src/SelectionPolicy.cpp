#include "../include/SelectionPolicy.h"
#include <algorithm>
#include <iostream>

//NaiveSelection::

NaiveSelection::NaiveSelection(): _lastSelectedIndex(0){};

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
            
    if (this->_lastSelectedIndex >= static_cast<int>(facilitiesOptions.size()) -1 ){
        this->_lastSelectedIndex = 0;
    }
    else {
       this->_lastSelectedIndex = _lastSelectedIndex + 1; 
    }
    return facilitiesOptions[this->_lastSelectedIndex];
};

const string NaiveSelection::toString() const{
    return "nve";
};

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
};


// BalancedSelection::

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
_LifeQualityScore(LifeQualityScore),_EconomyScore(EconomyScore),_EnvironmentScore(EnvironmentScore){};

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){

    int minDistance = 0;
    for (int i = 1; i < static_cast<int>(facilitiesOptions.size()); i++){
        if (distanceCalc(facilitiesOptions[minDistance]) > distanceCalc(facilitiesOptions[i])){
            minDistance = i; 
        }
    }
    this->_EconomyScore += facilitiesOptions[minDistance].getEconomyScore();
    this->_EnvironmentScore += facilitiesOptions[minDistance].getEnvironmentScore();
    this->_LifeQualityScore += facilitiesOptions[minDistance].getLifeQualityScore();
    return facilitiesOptions[minDistance];

};

const string BalancedSelection::toString() const{
    return "bal";
};

BalancedSelection* BalancedSelection::clone() const{
    return new BalancedSelection(*this);
};

int BalancedSelection::distanceCalc(const FacilityType &facilityOption){
    int newEconomyScore = this->_EconomyScore + facilityOption.getEconomyScore();
    int newEnvironmentScore = this->_EnvironmentScore + facilityOption.getEnvironmentScore();
    int newLifeQualityScore = this->_LifeQualityScore + facilityOption.getLifeQualityScore();
    int maxScore = std::max({newEconomyScore, newEnvironmentScore, newLifeQualityScore});
    int minScore = std::min({newEconomyScore, newEnvironmentScore, newLifeQualityScore});
    return maxScore - minScore;
    };

    
//EconomySelection::

EconomySelection::EconomySelection(): _lastSelectedIndex(0){};

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int i = _lastSelectedIndex + 1;
    while (i != _lastSelectedIndex)
    {
        if (i == static_cast<int>(facilitiesOptions.size()))
        {
            i = 0;
        }
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY){
                _lastSelectedIndex = i;
                return facilitiesOptions[i];
        } 
        i++;
    }
    return facilitiesOptions[_lastSelectedIndex]; // We assumed that there is at least one economy facility in the list
};
        
const string EconomySelection::toString() const{
    return "eco";
};

EconomySelection* EconomySelection::clone() const{
    return new EconomySelection(*this);
};

//SustainabilitySelection::

SustainabilitySelection::SustainabilitySelection(): _lastSelectedIndex(0){};

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int i = _lastSelectedIndex + 1;
    while (i != _lastSelectedIndex)
    {
        if (i == static_cast<int>(facilitiesOptions.size()))
        {
            i = 0;
        }
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT){
                _lastSelectedIndex = i;
                return facilitiesOptions[i];
        } 
        i++;
    }
    return facilitiesOptions[_lastSelectedIndex]; // We assumed that there is at least one environment facility in the list
};

const string SustainabilitySelection::toString() const{
    return "env";
};

SustainabilitySelection* SustainabilitySelection::clone() const{
    return new SustainabilitySelection(*this);
};
