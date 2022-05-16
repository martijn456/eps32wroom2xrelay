#include <sys/unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "bow.h"

#include "cu2.h"

// The amount of button updates (100ms) each for a long press
#define LONG_PRESS_UPDATES 50

static TimerHandle_t buttonCheckTimer;

static EventGroupHandle_t _eventGroupHandle;

static int _buttonModeShortPressBit;
static int _buttonModeLongPressBit;
static int _buttonLightShortPressBit;
static int _buttonLightLongPressBit;
static int _checkButtonBit;
static int _ignoreHeldBit;

static void buttonCheckTimerCallback(TimerHandle_t xTimer) { xEventGroupSetBits(_eventGroupHandle, _checkButtonBit); }

void initCu2(EventGroupHandle_t eventGroupHandle,
             const int buttonModeShortPressBit,
             const int buttonModeLongPressBit,
             const int buttonLightShortPressBit,
             const int buttonLightLongPressBit,
             const int checkButtonBit,
             const int ignoreHeldBit) {
    _eventGroupHandle = eventGroupHandle;
    _buttonModeShortPressBit = buttonModeShortPressBit;
    _buttonModeLongPressBit = buttonModeLongPressBit;
    _buttonLightShortPressBit = buttonLightShortPressBit;
    _buttonLightLongPressBit = buttonLightLongPressBit;
    _checkButtonBit = checkButtonBit;
    _ignoreHeldBit = ignoreHeldBit;

    buttonCheckTimer = xTimerCreate("buttonCheckTimer", (100 / portTICK_PERIOD_MS), pdTRUE, (void *)0, buttonCheckTimerCallback);
}

void buttonCheck() {
    static uint8_t count = 1;

    static uint32_t heldMode = 0;
    static uint32_t heldLight = 0;

    static bool ignoreFirst = false;

    EventBits_t bits = xEventGroupWaitBits(_eventGroupHandle, _ignoreHeldBit, true, false, 0);
    if((bits & _ignoreHeldBit) != 0) {
        ignoreFirst = true;
    }

    // Button check command, should run every 100ms.
    // Reply indicates if/which button is pressed.
    uint8_t cmd[] = {0xc1, 0x21, 0x22, count};

    messageType response = {};
    readResult result = exchange(cmd, sizeof(cmd), &response);

    // The first is '00','01','02' or '03', depending on whether the top, bottom, or both buttons are presse

    bool pressMode = (response.payload[0] & BIT1) != 0;
    bool pressLight = (response.payload[0] & BIT0) != 0;

    if(pressMode) {
        heldMode++;
    }
    if(pressLight) {
        heldLight++;
    }

    if(heldMode == LONG_PRESS_UPDATES) {
        xEventGroupSetBits(_eventGroupHandle, _buttonModeLongPressBit);
    }

    if(heldLight == LONG_PRESS_UPDATES) {
        xEventGroupSetBits(_eventGroupHandle, _buttonLightLongPressBit);
    }

    if(heldMode > 0 && !pressMode) {
        if(ignoreFirst) {
            ignoreFirst = false;
        } else if(heldMode < LONG_PRESS_UPDATES) {
            xEventGroupSetBits(_eventGroupHandle, _buttonModeShortPressBit);
        }
        heldMode = 0;
    }

    if(heldLight > 0 && !pressLight) {
        if(ignoreFirst) {
            ignoreFirst = false;
        } else if(heldLight < LONG_PRESS_UPDATES) {
            xEventGroupSetBits(_eventGroupHandle, _buttonLightShortPressBit);
        }
        heldLight = 0;
    }

    count += 1;
    count %= 0x10;
}

void startButtonCheck() { xTimerStart(buttonCheckTimer, 0); }

void stopButtonCheck() { xTimerStop(buttonCheckTimer, 0); }

static uint32_t digits(uint32_t value, size_t digits, size_t atleast) {
    uint32_t result = 0;
    uint32_t divider = 1;
    for(int pos = 0; pos < digits; pos++) {
        uint32_t digit = ((pos + 1 > atleast) && value < divider) ? 0xc : ((value / divider) % 10);
        result |= digit << (4 * pos);
        divider *= 10;
    }
    return result;
}

void showState(uint8_t level, bool lightOn, uint16_t speed, uint32_t trip) {
    uint16_t numTop = digits(speed, 3, 2);
    uint32_t numBottom = digits(trip / 100, 5, 1);
    displayUpdate(false, (assist_level)level, BLNK_SOLID, BLNK_OFF, BLNK_OFF, BLNK_SOLID, lightOn ? BLNK_SOLID : BLNK_OFF, BLNK_SOLID, BLNK_OFF, BLNK_SOLID, BLNK_SOLID, BLNK_SOLID,
                  false, 50, numTop, numBottom);
}

void displayUpdate(bool setDefault,
                   assist_level assistLevel,
                   blink_speed assistBlink,
                   blink_speed wrench,
                   blink_speed total,
                   blink_speed trip,
                   blink_speed light,
                   blink_speed bars,
                   blink_speed comma,
                   blink_speed km,
                   blink_speed top,
                   blink_speed bottom,
                   bool miles,
                   uint8_t battery,
                   uint16_t topVal,
                   uint32_t bottomVal) {
    uint8_t assist = assistBlink;
    assist <<= assistLevel * 2;

    uint8_t segments1 = (wrench << 0) | (total << 2) | (trip << 4) | (light << 6);
    uint8_t segments2 = (bars << 0) | (comma << 4) | (km << 6);

    // Each digit is a nibble in the value:
    // Numbers are 0-9, a='-', b='b', c=' ', d='d', e='e', f='f'
    uint8_t numTop1 = ((uint8_t)(topVal >> 8) & 0x0f) | (miles ? 0xe0 : 0x00);
    uint8_t numTop2 = (uint8_t)(topVal >> 0);

    uint8_t numBottom1 = ((uint8_t)(bottomVal >> 16) & 0x0f) | (bottom << 4) | (top << 6);
    uint8_t numBottom2 = (uint8_t)(bottomVal >> 8);
    uint8_t numBottom3 = (uint8_t)(bottomVal >> 0);

    uint8_t cmd[] = {0xc1, 0x29, (uint8_t)(setDefault ? 0x27 : 0x26), assist, segments1, segments2, battery, numTop1, numTop2, numBottom1, numBottom2, numBottom3};

    exchange(cmd, sizeof(cmd));
}
