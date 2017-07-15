#include <cstdlib>

#include "nodes.hpp"
#include "node.hpp"
#include "util.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"

#include "status.hpp"

Node::Node(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store some variables
    framesPerBuffer = controller->getSettings()->bufferSize;
    sampleRate = controller->getSettings()->sampleRate;
}

Node::~Node() {
    // Delete all inputs and outputs
    for(auto it = inputs.begin(); it != inputs.end(); ++it)
        delete it->second;

    for(auto it = outputs.begin(); it != outputs.end(); ++it)
        delete it->second;
}

Node* Node::setId(std::string id) {
    //    // Only allowed to change id when not yet set
    //    if(this->id.compare(NODE_DEFAULT_ID) != 0) {
    //        return false;
    //    }
    this->id = id;
    return this;
}

void Node::update() {
    if(updated)
        return;
    
    updated = true;
    
    if(updateNodeInputs) {
        // Update all node inputs
        for(auto it = inputs.begin(); it != inputs.end(); ++it) {
            NodeInput* input = it->second;
            if(input->getType() == NodeInput::NODE || input->getType() == NodeInput::NODE_KEY_INDEPENDENT) {
                ((NodeOutput*) input->pointer)->getNode()->update();
            }

        }
    }
    
    apply();
};

bool Node::addInput(std::string label, NodeInput* input) {
    auto position = inputs.find(label);
    if(position != inputs.end()) return false; // TODO
    
    inputs[label] = input;
    return true;
}

bool Node::addOutput(std::string label, NodeOutput* output) {
    auto position = outputs.find(label);
    if(position != outputs.end()) return false; // TODO
    
    outputs[label] = output;
    return true;
}

bool Node::removeInput(NodeInput* input) {
    bool found = false;
    for(auto it = inputs.begin();it != inputs.end(); ++it) {
        if(it->second == input) {
            inputs.erase(it);
            found = true;
            break;
        }
    }
    return found;
}

bool Node::removeOutput(NodeOutput* output) {
    bool found = false;
    for(auto it = outputs.begin();it != outputs.end(); ++it) {
        if(it->second == output) {
            outputs.erase(it);
            found = true;
            break;
        }
    }
    return found;
}

NodeInput* Node::getInput(std::string label) {
    auto position = inputs.find(label);
    if(position == inputs.end()) return NULL;
    
    return position->second;
}

NodeOutput* Node::getOutput(std::string label) {
    auto position = outputs.find(label);
    if(position == outputs.end()) return NULL;
    
    return position->second;
}
