#include "LCD.h"

inline void togglePin(pin_ref_t* _pin) {
    *((*_pin).port) ^= (1 << (*_pin).pin);
}

inline void pinOff(pin_ref_t* pin) {
    *((*pin).port) &= ~(1 << (*pin).pin);
}

inline void pinOn(pin_ref_t* pin) {
    *((*pin).port) |= (1 << (*pin).pin);
}

bool getBit(pin_ref_t* _pin) {
    return ((*((*_pin).port) >> (*_pin).pin) & 0x01);
}

void pulseEnable(lcd_config_t* lcd) {
    pinOn(&(*lcd).en);
    __delay_us(1);
    pinOff(&(*lcd).en);
    __delay_us(100);
}

void writeNibble(lcd_config_t* lcd, unsigned char nibble) {
    for (unsigned char i = 0; i < 4; i++) {
        pinOff(&lcd->data_pins[i]);
        if (nibble & (1 << i)) {
            pinOn(&lcd->data_pins[i]);
        }
    }
    pulseEnable(lcd);
}

void writeCommand(lcd_config_t* lcd, unsigned char command) {
    pinOff(&lcd->rs);
    pinOff(&lcd->rw);

    writeNibble(lcd, command >> 4);
    writeNibble(lcd, command & 0x0F);

    __delay_us(40);
}

void lcdInit(lcd_config_t* lcd) {
    __delay_ms(15); // wait for more than 15ms after VDD rises to 4.5V

    // First initialization sequence
    writeNibble(lcd, 0b0011);
    __delay_ms(5); // wait for more than 4.1ms
    writeNibble(lcd, 0b0011);
    __delay_us(100); // wait for more than 100us
    writeNibble(lcd, 0b0011);
    __delay_us(40);

    // Function set command
    writeNibble(lcd, 0b0010);
    writeCommand(lcd, 0b00101000); // Set interface to 4 bits long, 2 lines, 5x8 font

    // Display on/off control command
    writeCommand(lcd, 0b00001100); // Turn display on, cursor off, blink off

    // Clear display command
    writeCommand(lcd, 0b00000001); // Clear display, move cursor to home

    __delay_ms(2); // wait for more than 1.52ms
}

void setCursor(lcd_config_t* lcd, int8_t col, int8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd->cCol = col; //Seguimiento del cursor.
    lcd->cRow = row;
    writeCommand(lcd, 0x80 | (col + row_offsets[row]));
}

void lcdWriteChar(lcd_config_t* lcd, char c) {
    writeData(lcd, c);
    lcd->cCol++;
}

void lcdPrint(lcd_config_t* lcd, const char* str) {
    while (*str) {
        writeData(lcd, *str++);
        lcd->cCol++;
    }
}

inline void lcdClear(lcd_config_t* lcd) {
    writeCommand(lcd, 0x01);
    __delay_ms(2);
}

inline void lcdShowCursor(lcd_config_t* lcd) {
    writeCommand(lcd, 0x0E);
}

inline void lcdShowNoCursor(lcd_config_t* lcd) {
    writeCommand(lcd, 0x0C);
}

void lcdBlink(lcd_config_t* lcd) {
    lcd->displayControl |= 0x0F;
    writeCommand(lcd, 0x08 | lcd->displayControl);
}

void lcdNoBlink(lcd_config_t* lcd) {
    lcd->displayControl &= ~0x0F;
    writeCommand(lcd, 0x08 | lcd->displayControl);
}

void lcdScrollDisplayLeft(lcd_config_t* lcd) {
    writeCommand(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcdScrollDisplayRight(lcd_config_t* lcd) {
    writeCommand(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcdCursorRight(lcd_config_t* lcd) {
    lcd->cCol++;
    writeCommand(lcd, LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
}

void lcdCursorLeft(lcd_config_t* lcd) {
    lcd->cCol--;
    writeCommand(lcd, LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
}

void lcdCreateChar(lcd_config_t* lcd, uint8_t charmap[], uint8_t location) {
    location &= 0x7; // We only have 8 locations 0-7
    writeCommand(lcd, LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        writeData(lcd, charmap[i]);
    }
}

inline void writeData(lcd_config_t* lcd, uint8_t data) {
    pinOn(&lcd->rs);
    pinOff(&lcd->rw);
    writeNibble(lcd, data >> 4);
    writeNibble(lcd, data & 0x0F);
}

void lcdScrollRowText(lcd_config_t* lcd, const char* str, uint8_t row, uint8_t direction/*, uint16_t speed*/) {
    size_t length = strlen(str);
    lcdNoBlink(lcd);
    lcdShowNoCursor(lcd);
    switch (direction) {
        case LCD_MOVELEFT: // a mejorar...
            for (uint8_t i = 0; i <= length - lcd->cols; i++) {
                setCursor(lcd, 0, row);
                lcdPrint(lcd, str + i);
                __delay_ms(300); //A cambiar...
                //lcdClear(lcd); AFECTA AL RESTO DEL TEXTO.
            }
            break;
        case LCD_MOVERIGHT:
        default:
            str += length;
            for (uint8_t i = 0; i < length + 1; i++) {
                setCursor(lcd, 0, row);
                lcdPrint(lcd, str - i);
                __delay_ms(300);
            }
            break;
    }
}



