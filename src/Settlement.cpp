#include "../include/Settlement.h"

Settlement::Settlement(const string &name, SettlementType type): _name(name),_type(type),_limit(static_cast<int>(_type) + 1){};

const string &Settlement::getName() const{
    return this->_name;
};
    
SettlementType Settlement::getType() const{
    return this->_type;
};  

const int Settlement::getLimit() const{
    return this->_limit;
};  
    
const string Settlement::toString() const{
    std::string setType;
    switch (this->_type) {
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
    return "settlement " + this->_name + " " + setType;
};