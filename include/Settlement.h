#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        const string &getName() const;
        SettlementType getType() const;
        const string toString() const;
        const int getLimit() const;

        private:
            const string _name;
            SettlementType _type;
            const int _limit; // the number of facilities that can be built simultaneously
};