#ifndef input_hpp
#define input_hpp

#include <portmidi.h>

class Controller;

class Input {
    
    Controller* controller;

    PmStream* inputStream;
    
    bool active;

public:
    
    Input(Controller*);
//    ~Input();
    
    bool start();
    bool stop();
    
    void update();
    
    int amountOfDevices();
    const char* deviceName(int);
    bool isInput(int);
    
};

#endif
