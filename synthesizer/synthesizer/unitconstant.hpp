#ifndef unitconstant_hpp
#define unitconstant_hpp

#include "unit.hpp"

class UnitConstant : public Unit {

    double value;

public:
    
    UnitConstant(Controller*, double);
    
    inline void apply(Instrument*) {};
};

#endif