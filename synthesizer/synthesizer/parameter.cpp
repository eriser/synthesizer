#include <string>

#include "parameter.hpp"
#include "controller.hpp"
#include "units.hpp"
#include "unit.hpp"
#include "blocks.hpp"
#include "block.hpp"
#include "sample.hpp"
#include "unitconstant.hpp"
#include "util.hpp"
#include "curve.hpp"
#include "function.hpp"

#include "error.hpp"

Parameter::Parameter(Controller* controller, Type type, std::string label, std::string defaultValue) : type(type), label(label) {
    // Store stuff and set default value
    this->controller = controller;
    
    pointer = NULL;
    set(defaultValue);
}

bool Parameter::set(std::string value) {
    switch(type) {

        case SAMPLE:
            if(value.compare("sine") == 0)      { pointer = Sample::sine; return true; }
            if(value.compare("square") == 0)    { pointer = Sample::square; return true; }
            if(value.compare("triangle") == 0)  { pointer = Sample::triangle; return true; }
            if(value.compare("sawtooth") == 0)  { pointer = Sample::sawtooth; return true; }
            Error::addError(Error::SAMPLE_NOT_FOUND);
            return false;
            
        case CURVE:
            if(value.compare("linear") == 0)        { pointer = Curve::Linear; return true; }
            if(value.compare("sine") == 0)          { pointer = Curve::Sine; return true; }
            if(value.compare("quadin") == 0)        { pointer = Curve::QuadIn; return true; }
            if(value.compare("quadout") == 0)       { pointer = Curve::QuadOut; return true; }
            if(value.compare("quartin") == 0)       { pointer = Curve::QuartIn; return true; }
            if(value.compare("quartout") == 0)      { pointer = Curve::QuartOut; return true; }
            if(value.compare("exponential") == 0)   { pointer = Curve::Exponential; return true; }
            Error::addError(Error::CURVE_NOT_FOUND);
            return false;
            
            
        case FUNCTION:
            if(value.compare("identity") == 0)  { pointer = Function::Identity; return true; }
            if(value.compare("pow10") == 0)     { pointer = Function::Pow10; return true; }
            Error::addError(Error::FUNCTION_NOT_FOUND);
            return false;
            
        case UNIT:
        case UNIT_KEY_INDEPENDENT:
            
            Unit* unit = NULL;
            // Check if it is a number
            if(Util::isNumber(value)) {
                // Create a new unit constant
                unit = controller->getUnits()->createConstant(stod(value));
            }
            
            // Check if it is a unit
            else if((unit = controller->getUnits()->getUnit(value)) != NULL) {
                // Check for key dependence
                if(type == UNIT_KEY_INDEPENDENT && unit->isKeyDependent()) { Error::addError(Error::EXPECTED_KEY_INDEPENDENT); return false; }
            }
            
            // Check if it is a block output
            else {
                unsigned long position;
                if((position = value.find(":")) != std::string::npos) {
                    std::string blockId = value.substr(0, position);
                    std::string label = value.substr(position + 1);
                    
                    Block* block = controller->getBlocks()->getBlock(blockId);
                    if(block == NULL) return false; // TODO
                    
                    unit = block->getOutput(label);
                    if(unit == NULL) return false; // TODO
                }
            }
            
            if(unit == NULL) return false; // TODO
            
            // If we are overwriting a unit constant, delete the old constant
            if(pointer != NULL && controller->getUnits()->isConstant((class Unit*) pointer))
                controller->getUnits()->deleteConstant((UnitConstant*) pointer);
            
            pointer = unit;
            return true;
    }
}

std::string Parameter::valueToString() {
    switch(type) {
        case UNIT:
        case UNIT_KEY_INDEPENDENT:
            return ((Unit*) pointer)->getId();
        case SAMPLE: return ((Sample*) pointer)->getId();
        case CURVE: return ((Curve*) pointer)->getId();
        case FUNCTION: return ((Function*) pointer)->getId();
    }
}

std::string Parameter::typeToString(Type type) {
    switch(type) {
        case UNIT: return "unit_key_dependent";
        case UNIT_KEY_INDEPENDENT: return "unit";
        case SAMPLE: return "sample";
        case CURVE: return "curve";
        case FUNCTION: return "function";
    }
}


