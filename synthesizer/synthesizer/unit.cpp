#include <cstdlib>

#include "units.hpp"
#include "unit.hpp"
#include "util.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "parameter.hpp"

#include "status.hpp"

Unit::Unit(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store some variables
    framesPerBuffer = controller->getSettings()->bufferSize;
    sampleRate = controller->getSettings()->sampleRate;
    
    // Create output array
    output = new float[framesPerBuffer];
    memset(output, 0, sizeof(float) * framesPerBuffer);
}

Unit::~Unit() {
    delete[] output;
    
    for(auto it = parameters.begin(); it != parameters.end(); ++it)
        delete *it;
}

Unit* Unit::setId(std::string id) {
    //    // Only allowed to change id when not yet set
    //    if(this->id.compare(UNIT_DEFAULT_ID) != 0) {
    //        return false;
    //    }
    this->id = id;
    return this;
}

#include <iostream>

void Unit::update() {
    if(updated)
        return;
    
    updated = true;
    
    if(updateParameters) {
        // Update all unit parameters
        for(auto it = parameters.begin(); it != parameters.end(); ++it) {
            if((*it)->type == Parameter::UNIT || (*it)->type == Parameter::UNIT_KEY_INDEPENDENT)
                ((Unit*) ((*it)->pointer))->update();
        }
    }
    
    apply();
};

bool Unit::setParameter(std::string label, std::string value) {
    for(auto it = parameters.begin(); it != parameters.end(); ++it) {
        if(label.compare((*it)->label) == 0)
            return (*it)->set(value);
    }
    Status::addError("Parameter not found");
    return false;
}

Parameter* Unit::getParameter(std::string label) {
    for(auto it = parameters.begin(); it != parameters.end(); ++it) {
        if(label.compare((*it)->label) == 0)
            return *it;
    }
    return NULL;
}

