#ifndef mididevices_hpp
#define mididevices_hpp

#include <vector>
#include <mutex>

class Controller;
class MIDIDevice;

class MIDIDevices {
    
    Controller* controller;

    std::vector<MIDIDevice*> devices;

    std::mutex mutex;
    
    bool active;
    
public:
    
    MIDIDevices(Controller*);
    ~MIDIDevices();
    
    bool start();
    bool stop();
    bool update();
    
    bool add(int);
    bool remove(int);
    MIDIDevice* get(int);
    
    static int amountOfDevices();
    static const char* deviceName(int);
    static bool isInput(int);
    
    // Status
    void printMIDIDevices();
    
};

#endif
