#ifndef settings_hpp
#define settings_hpp

#define AMOUNT_OF_KEYS (128)
#define NOTE_A4 (69)

class Settings {

public:
    
    double frequencies[AMOUNT_OF_KEYS];
    int pitchWheelRange;
    bool invertedSustainPedal;
    
    Settings();
    
};

#endif