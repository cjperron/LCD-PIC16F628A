#ifndef __LCD_H
#define	__LCD_H



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Cambiar segun frecuencia del CLK.
#define _XTAL_FREQ 4000000
#include <xc.h>



typedef struct {
    volatile unsigned char* port;
    unsigned char pin;
} pin_ref_t;

typedef struct {
    pin_ref_t rs;
    pin_ref_t rw;
    pin_ref_t en;
    pin_ref_t data_pins[4];
    uint8_t displayControl;
    uint8_t cols, rows;
    int8_t cCol, cRow;
} lcd_config_t;

//ayuda para hacer la definicion de la configuracion mas amena.
#define INIT_LCD_CONFIG(rs_port, rs_pin, rw_port, rw_pin, en_port, en_pin, d4_port, d4_pin, d5_port, d5_pin, d6_port, d6_pin, d7_port, d7_pin) \
    { \
        .rs = { .port = rs_port, .pin = rs_pin },   \
        .rw = { .port = rw_port, .pin = rw_pin },   \
        .en = { .port = en_port, .pin = en_pin },   \
        .data_pins = { \
            { .port = d4_port, .pin = d4_pin },     \
            { .port = d5_port, .pin = d5_pin },     \
            { .port = d6_port, .pin = d6_pin },     \
            { .port = d7_port, .pin = d7_pin }      \
        }, \
        .displayControl = 0,                        \
        .cols = 16,                                 \
        .rows = 2                                   \
    }
//Macros
#define LCD_Home(lcd) setCursor(lcd, 0, 0)
#define lcdWriteCustomChar(lcd, location) writeData(lcd, location); lcd->cCol++;

//COnstantes
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_DISPLAYMOVE     0x08
#define LCD_CURSORMOVE      0x00
#define LCD_MOVERIGHT       0x04
#define LCD_MOVELEFT        0x00
#define LCD_SETCGRAMADDR    0x40

inline void togglePin(pin_ref_t* _pin);
inline void pinOff(pin_ref_t* _pin);
inline void pinOn(pin_ref_t* _pin);
bool getBit(pin_ref_t* _pin);
void pulseEnable(lcd_config_t* lcd);
void writeNibble(lcd_config_t* lcd, unsigned char nibble);
void writeCommand(lcd_config_t* lcd, unsigned char command);
inline void writeData(lcd_config_t* lcd, uint8_t data);
void lcdInit(lcd_config_t* lcd);
void setCursor(lcd_config_t* lcd, int8_t col, int8_t row);
void lcdWriteChar(lcd_config_t* lcd, char c);
void lcdPrint(lcd_config_t* lcd, const char* str);
inline void lcdClear(lcd_config_t* lcd);
inline void lcdShowCursor(lcd_config_t* lcd);
inline void lcdShowNoCursor(lcd_config_t* lcd);
inline void lcdBlink(lcd_config_t* lcd);
void lcdScrollDisplayRight(lcd_config_t* lcd);
void lcdScrollDisplayLeft(lcd_config_t* lcd);
void lcdCreateChar(lcd_config_t* lcd, uint8_t charmap[], uint8_t location);
void lcdCursorRight(lcd_config_t* lcd);
void lcdCursorLeft(lcd_config_t* lcd);

//Funcion puramente demostrativa, probablemente tenga que hacerse muchos "workarounds" 
//para que cumpla el proposito que todo el mundo quisiese..
void lcdScrollRowText(lcd_config_t* lcd, const char* str, uint8_t row, uint8_t direction/*, uint16_t speed*/);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* __LCD_H */

