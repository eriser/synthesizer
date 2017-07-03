#ifndef unithighpass_hpp
#define unithighpass_hpp

#include "unit.hpp"

class IIRFilter;

class UnitHighpass : public Unit {
    
    Parameter* input;
    Parameter* cutOff;
    
    int order;
    double omegaC;
    
    IIRFilter* filter;
    
    void updateFilter();
    
public:
    
    UnitHighpass(Controller*, Arguments);
    ~UnitHighpass();
    
    void apply(Instrument*);
    
    static const int maxOrder;
    
};

#endif
