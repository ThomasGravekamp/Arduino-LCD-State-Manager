#include "LcdStateManager.h"

LcdStateManager::LcdStateManager (LCD* lcd, ScreenRenderer* renderers) {
    _lcd_instance_pointer = lcd;

    _renderers = renderers;
    _current_state = 0;

    should_render = true;
    _first_render = true;
};

byte LcdStateManager::getState () {
    return _current_state;
};

void LcdStateManager::setState (byte state) {
    _current_state = state;
    _first_render = true;
    should_render = true;
};

void LcdStateManager::update () {
    if (!should_render) {
        return;
    }

    _renderers[_current_state](_lcd_instance_pointer, _first_render);

    _first_render = false;
    should_render = false;
};
