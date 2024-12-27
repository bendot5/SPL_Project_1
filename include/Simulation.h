#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation& other);
        void start();
        void getInput();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isPolicyExists(const string &policyName);
        bool isFacilityExists(const string &facilityName);
        bool isPlanExists(const int planID);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const vector<BaseAction*> getActionsLog() const;
        void step();
        void close();
        void open();
        Simulation& operator=(const Simulation& other);
        ~Simulation();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};