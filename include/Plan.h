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
        const int getPlanId() const;
        const string getSelectionPolicy() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step(); // if facility went operational, load another one
        void printStatus();
        const vector<Facility*> &getUnderConsturction() const;
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        Plan(const Plan& other);
        Plan& operator=(const Plan& other);
        ~Plan();


    private:
        int _plan_id;
        const Settlement &_settlement;
        SelectionPolicy *_selectionPolicy; //What happens if we change this to a reference?
        PlanStatus _status;
        vector<Facility*> _facilities;
        vector<Facility*> _underConstruction;
        const vector<FacilityType> &_facilityOptions;
        int _life_quality_score, _economy_score, _environment_score;
};