#include <cmath>

#include "nodefollower.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeFollower::NodeFollower(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "follower";
    
    // Set options
    keyNode = options.getBool("key", false);
    RMSAverage = 0.0;
    
    // Set inputs and outputs
    addInput("output", input = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeFollower::apply() {
    float* input = ((NodeOutput*) this->input->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        RMSAverage = 0.998f * RMSAverage + 0.002f * (input[x]*input[x]); // TODO: coefficients? (see the Audio Effects book, chapter 9,10 or 11 somewhere. Also, it should be called a envelope follower)
        output[x] = std::sqrt(RMSAverage);
    }
}
