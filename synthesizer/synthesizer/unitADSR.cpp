#include "unitADSR.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitADSR::UnitADSR(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // An ADSR is key dependent
    keyDependent = true;
    
    // Set default values
    Unit::set(controller, &startLevel, "0.0", keyDependent);
    Unit::set(controller, &attackLevel, "1.0", keyDependent);
    Unit::set(controller, &sustainLevel, "1.0", keyDependent);
    Unit::set(controller, &releaseLevel, "0.0", keyDependent);
    
    Unit::set(controller, &attackTime, "0.0", keyDependent);
    Unit::set(controller, &decayTime, "0.0", keyDependent);
    Unit::set(controller, &releaseTime, "0.0", keyDependent);
    
    attackType = Interpolation::Linear;
    decayType = Interpolation::Linear;
    releaseType = Interpolation::Linear;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitADSR::~UnitADSR() {
    delete[] output;
}

void UnitADSR::apply(Instrument* instrument) {
    startLevel->update(instrument);
    attackLevel->update(instrument);
    sustainLevel->update(instrument);
    releaseLevel->update(instrument);
    attackTime->update(instrument);
    decayTime->update(instrument);
    releaseTime->update(instrument);
    
    Instrument::Stage stage = instrument->currentStage;
    double t = 1.0 / controller->getSampleRate();
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        
        double attack = attackTime->output[x];
        double decay = decayTime->output[x];
        double release = releaseTime->output[x];
        
        double d = instrument->currentDuration;
        double r = instrument->currentRelease;
        
        if(stage == Instrument::Press || stage == Instrument::Sustain) d += t * x;
        if(stage == Instrument::Released) r += t * x;
        
        float amplitude;
        
        // Attack stage
        if(d < attack)
            amplitude = startLevel->output[x] + (attackLevel->output[x] - startLevel->output[x]) * Interpolation::ease(attackType, d / attack);
        
        // Decay stage
        else if(d < attack + decay)
            amplitude = attackLevel->output[x] + (sustainLevel->output[x] - attackLevel->output[x]) * Interpolation::ease(decayType, (d - attack) / decay);
        
        // Sustain stage
        else amplitude = sustainLevel->output[x];
        
        // Release stage
        if(stage != Instrument::Press && stage != Instrument::Sustain) {
            if(r >= release)
                amplitude = releaseLevel->output[x];
            else
                amplitude += (releaseLevel->output[x] - amplitude) * Interpolation::ease(releaseType, r / release);
        }
        
        output[x] = amplitude;
    }
}

bool UnitADSR::setValue(std::string parameter, std::string value) {
    if(parameter.compare("start_level") == 0)
        return Unit::set(controller, &startLevel, value, keyDependent);
    
    if(parameter.compare("attack_level") == 0)
        return Unit::set(controller, &attackLevel, value, keyDependent);

    if(parameter.compare("sustain_level") == 0)
        return Unit::set(controller, &sustainLevel, value, keyDependent);
    
    if(parameter.compare("release_level") == 0)
        return Unit::set(controller, &releaseLevel, value, keyDependent);
    
    if(parameter.compare("attack_time") == 0)
        return Unit::set(controller, &attackTime, value, keyDependent);
    
    if(parameter.compare("decay_time") == 0)
        return Unit::set(controller, &decayTime, value, keyDependent);
    
    if(parameter.compare("release_time") == 0)
        return Unit::set(controller, &releaseTime, value, keyDependent);
    
    if(parameter.compare("attack_type") == 0)
        return Interpolation::set(controller, &attackType, value);
    
    if(parameter.compare("decay_type") == 0)
        return Interpolation::set(controller, &decayType, value);
    
    if(parameter.compare("release_type") == 0)
        return Interpolation::set(controller, &releaseType, value);
    
    return false;
};
