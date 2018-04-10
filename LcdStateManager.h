#ifndef lcd_state
#define lcd_state

#include "Arduino.h"
#include "LCD.h"

typedef void (*ScreenRenderer)(LCD*, bool);

class LcdStateManager {
    public:
        LcdStateManager(LCD* lcd, ScreenRenderer* renderers);

        void setState(byte state);
        byte getState();

        void update();

        bool should_render;

    private:
        LCD* _lcd_instance_pointer;

        bool _first_render;

        ScreenRenderer* _renderers;
        byte _current_state;
};

#endif
