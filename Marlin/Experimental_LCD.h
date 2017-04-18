// Experimental_LCD.h

#ifndef _EXPERIMENTAL_LCD_h
#define _EXPERIMENTAL_LCD_h

//#include "Marlin.h"
//#include "utility.h"
//#include "temperature.h"
#include "macros.h"
#include <Nextion.h>
#include <Time.h>  

//#if ENABLED(EXPERIMENTAL_LCD)



void lcd_setstatuspgm(const char* message, const uint8_t level = 0);

//#define lcd_setstatuspgm(x) lcd_setstatuspgm(PSTR(x))

#define LCD_ALERTMESSAGEPGM(x) NOOP
//#define LCD_MESSAGEPGM(x) lcd_setstatuspgm(PSTR(x))
#define LCD_MESSAGEPGM(x) lcd_setstatus(x)

void lcd_update();
void lcd_init();
FORCE_INLINE bool lcd_hasstatus() { return false; }
void lcd_setstatus(const char* message, const bool persist = false);
FORCE_INLINE void lcd_buttons_update() {}
FORCE_INLINE void lcd_reset_alert_level() {}

inline void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) { UNUSED(level); UNUSED(fmt); }
inline void status_printf(uint8_t level, const char *status, ...) { UNUSED(level); UNUSED(status); }

void mySerialEvent();
void ProcessPage(char * inputString, uint8_t receivedBytes);
void processBuffer(const char* receivedString);
void processMessage(const char * message);
//#endif //Experimental_LCD



#endif

