# LCD State management

This is a small and simple library for managing the state of an LCD.

This library should work with any Arduino-compatible board. Tested on an Arduino Uno clone.

## How to use

Include the header files for the [New LiquidCrystal library](https://bitbucket.org/fmalpartida/new-liquidcrystal/overview). Create an instance of the right LCD driver object. Refer to the [documentation](https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home) of the New LiquidCrystal library on how to do this.

Include the header file and create an instance of the `LcdStateManager` object. Create a render function for each state you want to display on your LCD. Each distinct "layout" on your display should be separate state.

The following example will initialise an instance of the LCD state manager and then scroll trought all the set states with a 200 milliseconds interval.

```
#include <LiquidCrystal.h>
#include <LcdStateManager.h>

// Initialise the LCD
LiquidCrystal_I2C lcd(LCD_ADDR, 2, 1, 0, 4, 5, 6, 7);
// Create a pointer to the LCD object
LCD* lcd_p = &lcd;

void screen_renderer_1 (LCD* lcd, bool first_render) {
  lcd->clear();
  lcd->print("Screen 1");
}

void screen_renderer_2 (LCD* lcd, bool first_render) {
  lcd->clear();
  lcd->print("Screen 2");
}

void screen_renderer_3 (LCD* lcd, bool first_render) {
  lcd->clear();
  lcd->print("Screen 3");
}

#define NUMBER_OF_SCREENS 3
#define SCREEN1 0
#define SCREEN2 1
#define SCREEN3 2

ScreenRenderer renderers[NUMBER_OF_SCREENS] = {
  screen_renderer_1,
  screen_renderer_2,
  screen_renderer_3
};

LcdStateManager lcd_state_manager(lcd_p, renderers);

setup () {
  // Configure the LCD
  lcd_p->begin(LCD_WIDTH, LCD_HEIGHT);
  lcd_p->setBacklightPin(3, POSITIVE);
  lcd_p->setBacklightPin(HIGH);
}

byte screen_index = 0;

loop () {
  lcd_state_manager.setState(screen_index);

  screen_index++;
  if (screen_index == NUMBER_OF_SCREENS) {
    screen_index = 0;
  }

  lcd_state_manager.update();

  delay(200);
}
```

## API

### `void ScreenRenderer(LCD* lcd, bool first_render)` function typedef

The renderers passed to the LcdStateManager object should accept a pointer to the LCD object and a boolean which specifies if this call is its first render. This first_render boolean can be used to determine if the screen should be rendered completeley or should only be updated partially.

| Argument | Type | Description |
| -------- | :--: | ----------- |
| lcd          | LCD* | A pointer to the LCD object. |
| first_render | bool | True if this is the first render of this screen. False if it is not the first render of this screen. |

### `LcdStateManager(LCD* lcd, ScreenRenderer* renderers)` constructor

Initialises the `LcdStateManager` object. Note: The array with renderers cannot be changed after initialisation.

```
LcdStateManager lcd_state_manager(lcd_p, renderers);
```

| Argument | Type | Description |
| -------- | :--: | ----------- |
| lcd       | LCD*            | A pointer to the LCD object. |
| renderers | ScreenRenderer* | The renderers for the different states. |

### `void setState(byte state)` method

Sets the state to the display. This method sets the `should_render` boolean to `true`. Note: This method does not have any bound checks. This means you can set the state to any number which may be larger than the number of elements in the array. I recommend using defines like this `#define STATE_NAME 0` to prevent this.

```
lcd_state_manager.setState(0);

// Recommended pattern:
#define STATE_NAME 0
lcd_state_manager.setState(STATE_NAME);
```

| Argument | Type | Description |
| -------- | :--: | ----------- |
| state | byte | The index of the state (screen). |

### `byte getState()` method

Returns the current state.

```
byte currentState = lcd_state_manager.getState();
```

### `void update()` method

Updates the LCD. It will render the current state if the `should_render` boolean is set to `true`. This boolean is set to true automatically when the state is changed. This boolean can also be set manually. The `should_render` property is set to `false` after rendering the current state.

```
lcd_state_manager.update();
```

### `bool should_render` property

When this property is true, the manager will call the render function of the current state when the `update()` method is called. This should be used when data being displayed changes.

```
lcd_state_manager.should_render = true;
```
