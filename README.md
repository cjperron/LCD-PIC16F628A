# LCD PIC16F628A

Libreria para controlar un display lcd 16x2 con el PIC16F628A

## ¿Por que?

Muchas veces uno se puede encontrar con la necesidad de tener un proyecto de menor escala, y que sea vistoso, y tambien se puede juntar con el hobby/estudio de la electronica. La poca documentacion, y disponibilidad de videos actualizados en el tema, hicieron que me dedique a crear esta libreria.

## Instalacion

Simple, los 2 archivos deben de arrastrarse, ya sea al /include del XC8, o donde sea, debe de linkearse con el compilador de XC8, asi se entera el mismo, la existencia de la libreria.

Aclaracion:
> Esta linea debe de ser cambiada previa a su compilacion, dependiendo de la frecuencia del clock, ya que las rutinas de inicializacion dependen de la misma.
> ```c
>   // LCD.h
>   #define _XTAL_FREQ 4000000 👈
>   #include <xc.h>
> ```

## Uso

Es similar a Liquid Crystal de Arduino, solo se deben de definir los pines a utilizar (En el P16F628A **NO** se puede utilizar RA5, ya que no tiene un **Output driver**)

```c
void main(){
    // [...] 👈 Configuracion del PIC, los pines a utilizar DEBEN estar configurados como salidas (en P16F62X, estableciendo '0').
    lcd_config_t lcd = INIT_LCD_CONFIG( //La macro auto-rellena el struct por vos...
        rs_port, rs_pin,
        rw_port, rw_pin,
        en_port, en_pin,
        d4_port, d4_pin,
        d5_port, d5_pin,
        d6_port, d6_pin,
        d7_port, d7_pin
        );
        lcdInit(&lcd); // Hace la rutina de inicializacion del lcd, mientras el lcd tenga tension, no va a ser necesario volver a ejecutar la funcion.
    //Ejemplo      col row
        LCD_Home(&lcd); //A todas las funciones hay que pasarle la referencia al lcd..
        lcdPrint(&lcd, "Hola Mundo!"); // 👋 👋
    while(1);
    return;
}
```

# Funciones

> - <font size="3">Inicializa el LCD. Debe ser la primera funcion a ejecutar (luego de crear el struct lcd_config_t)</font>
>
> ```c
> void lcdInit(lcd_config_t* lcd);
> ```

> - <font size="3">Ubica el cursor en la posicion deseada. En caso de escribir un caracter, se va a escribir desde el cursor (incluido) hacia adelante.</font>
>
> ```c
> void setCursor(lcd_config_t* lcd, uint8_t col, uint8_t row);
> ```

> - <font size="3">Escribe en la posicion del cursor el caracter deseado</font>
>
> ```c
> void lcdWriteChar(lcd_config_t* lcd, char c);
> ```

> - <font size="3">Escribe la string desde la posicion del cursor (incluido), en caso de sobrepasar la longitud del lcd, se escribira al buffer del mismo (se borra haciendo clear)</font>
>
> ```c
> void lcdPrint(lcd_config_t* lcd, const char* str);
> ```

> - <font size="3">Limpia la pantalla + el buffer, tarda ~1.3ms</font>
>
> ```c
> inline void lcdClear(lcd_config_t* lcd);
> ```

> - <font size="3">Muestra el cursor (es un '\_' debajo de los caracteres)</font>
>
> ```c
> inline void lcdShowCursor(lcd_config_t* lcd);
> ```

> - <font size="3">Lo contrario a la anterior, es la opcion default.</font>
>
> ```c
> inline void lcdShowNoCursor(lcd_config_t* lcd);
> ```

> - <font size="3">Titila en la posicion del cursor (como en las calculadoras cientificas)</font>
>
> ```c
> inline void lcdBlink(lcd_config_t* lcd);
> ```

> - <font size="3">Desplaza todo el contenido de la pantalla (buffer y cursor incluidos) hacia la derecha</font>
>
> ```c
> void lcdScrollDisplayRight(lcd_config_t* lcd);
> ```

> - <font size="3">Desplaza todo el contenido de la pantalla (buffer y cursor incluidos) hacia la izquierda</font>
>
> ```c
> void lcdScrollDisplayLeft(lcd_config_t* lcd);
> ```

> - <font size="3">Permite la creacion de un caracter personalizado, el charmap[] esta constituido por 8 numeros de 5 bits, para ocupar toda una "casilla" del lcd.</font>
>
> ```c
> void lcdCreateChar(lcd_config_t* lcd, uint8_t charmap[], uint8_t location);
> ```

> - <font size="3">Mueve el cursor a la derecha</font>
>
> ```c
> void lcdCursorRight(lcd_config_t* lcd);
> ```

> - <font size="3">Mueve el cursor a la iuzquierda</font>
>
> ```c
> void lcdCursorLeft(lcd_config_t* lcd);
> ```

> - <font size="3">Es una funcion extra _DEMOSTRATIVA_, imprime la string en la fila indicada, y la desplaza en la direccion indicada (LCD_MOVELEFT o LCD_MOVERIGHT, default MOVE_RIGHT en caso de mandar 🗑️)</font>
>
> ```c
> void lcdScrollRowText(lcd_config_t* lcd, const char* str, uint8_t row, uint8_t direction);
> ```

# Macros

Para el que no sepa que son, son "auto-rellenados" de codigo, el struct del lcd se llena con una, y el resto disponibles son:

> - <font size="3">Es un atajo para ir a [0,0], va con referencia del <strong>&</strong>lcd </font>
>
> ```c
>  LCD_Home(lcd)
> ```

> - <font size="3">Escribe el caracter personalizado, guardado en la ubicacion "location"</font>
>
> ```c
>  lcdWriteCustomChar(lcd, location)
> ```

# Ejemplos

Estos son un par de ejemplos que pueden servir como plantilla...

> <font size="4">Contador en pantalla 🔢</font>
>
> ```c
>   #include "LCD.h" //Incluir la mayoria de las librerias relevantes...
>   #include <stdio.h>
>
>   void escribirNumero(uint16_t* numero, lcd_config_t* lcd){
>      char buf[16];
>      sprintf(buf, "%u", *numero); // O usar otro metodo de convertir de uint16_t a char*
>      LCD_Home(lcd);
>      lcdPrint(lcd, buf);
>  }
>
>
>  void main(){
>      nRBPU = 0;
>      TRISB = 0b10000000; //RB7 entrada, con pull-up.
>      PORTB = 0;
>      lcd_config_t lcd = INIT_LCD_CONFIG( // RB0-6 al lcd.
>          &PORTB, 0,
>          &PORTB, 1,
>          &PORTB, 2,
>          &PORTB, 3,
>          &PORTB, 4,
>          &PORTB, 5,
>          &PORTB, 6
>      );
>      lcdInit(&lcd);
>      LCD_Home(&lcd);
>
>      lcdPrint(&lcd, "0");
>
>     uint16_t contador = 0;
>
>      while(1){
>          if(!RB7){    // Boton en RB7.
>              while(!RB7);
>              contador++;
>              escribirNumero(&contador, &lcd);
>          }
>      }
>  }
> ```

> <font size="4">Llenar LCD 😱</font>
>
> ```c
>   #include "LCD.h" //Incluir la mayoria de las librerias relevantes...
>
> uint8_t caracter_ejemplo[8] = {
>    0b11111,
>    0b11111,
>    0b00100,
>    0b01110,
>    0b11111,
>    0b01110,
>    0b00100,
>    0b00000, //A testear la ultima fila, en simulacion no figura...
> };
>
> void check(lcd_config_t* lcd, int8_t* ubicacion) {
>    if (ubicacion[0] > 15) {
>        ubicacion[0] = 0;
>        setCursor(lcd, 0, ubicacion[1]);
>    } else if (ubicacion[0] < 0) {
>        ubicacion[0] = 15;
>        setCursor(lcd, 15, ubicacion[1]);
>    } else if (ubicacion[1] == 1 || ubicacion[1] < 0) {
>        setCursor(lcd, ubicacion[0], 1);
>        ubicacion[1] = 1;
>    }
>    else if (ubicacion[1] == 0 || ubicacion[1] > 1) {
>        setCursor(lcd, ubicacion[0], 0);
>        ubicacion[1] = 0;
>    }
> }
>
> void main() {
>    nRBPU = 0;
>    CMCON = 7;
>    TRISA = 0b00001111; //RA0-3 Entradas. (pull-up externo 10kΩ)
>    PORTA = 0;
>    TRISB = 0b10000000; //RB7 entrada, con pull-up.
>    PORTB = 0;
>    lcd_config_t lcd = INIT_LCD_CONFIG(// RB0-6 al lcd.
>            &PORTB, 0,
>            &PORTB, 1,
>            &PORTB, 2,
>            &PORTB, 3,
>            &PORTB, 4,
>            &PORTB, 5,
>            &PORTB, 6
>            );
>    lcdInit(&lcd);
>    lcdCreateChar(&lcd, caracter_ejemplo, 0);
>    LCD_Home(&lcd);
>    lcdShowCursor(&lcd); //cl  rw
>   int8_t ubicacion[2] = {0, 0};
>
>
>    while (1) {
>        if (!RB7) { // Boton en RB7.
>            while (!RB7);
>            lcdWriteCustomChar(&lcd, 0); // El cursor se mueve.
>            ubicacion[0]++;
>            check(&lcd, ubicacion);
>        } else if (!RA0) { //RIGHT
>            while (!RA0);
>            lcdCursorRight(&lcd);
>            ubicacion[0]++;
>            check(&lcd, ubicacion);
>        } else if (!RA1) { //LEFT
>            while (!RA1);
>            lcdCursorLeft(&lcd);
>            ubicacion[0]--;
>            check(&lcd, ubicacion);
>        } else if (!RA2) { // UP
>            while (!RA2);
>            ubicacion[1]--;
>            check(&lcd, ubicacion);
>        } else if (!RA3) { // DOWN
>            while (!RA3);
>            lcdCursorLeft(&lcd);
>            ubicacion[1]++;
>            check(&lcd, ubicacion);
>        }
>
>
>    }
>
> }
>
>
>
> ```

> 
> <font size="4">Seguimiento de Cursor 🖱️</font>
>
> ```c
>   #include "LCD.h"
>   uint8_t cuadrado_vacio[8] = {
>    0b11111,
>    0b10001,
>    0b10001,
>    0b10001,
>    0b10001,
>    0b10001,
>    0b11111,
>    0b00000, //A testear la ultima fila, en simulacion no figura...
>};
>
>void menu(lcd_config_t* lcd) {
>    lcdClear(lcd);
>    setCursor(lcd, 1, 0);
>    lcdPrint(lcd, "Menu ejemplo");
>    setCursor(lcd, 0, 1);
>    lcdPrint(lcd, "1  2  3  4  5");
>    for (uint8_t i = 1; i < lcd->cols; i += 3) {
>        setCursor(lcd, i, lcd->cRow);
>        lcdWriteCustomChar(lcd, 0)
>    }
>    setCursor(lcd, 1, lcd->cRow);
>}
>
>void main() {
>    nRBPU = 0;
>    CMCON = 7;
>    TRISA = 0b00001111; //RA0-3 Entradas. (pull-up externo 10k?)
>    PORTA = 0;
>    TRISB = 0b10000000; //RB7 entrada, con pull-up.
>    PORTB = 0;
>    lcd_config_t lcd = INIT_LCD_CONFIG(// RB0-6 al lcd.
>            &PORTB, 0,
>            &PORTB, 1,
>            &PORTB, 2,
>            &PORTB, 3,
>            &PORTB, 4,
>            &PORTB, 5,
>            &PORTB, 6
>            );
>    lcdInit(&lcd);
>    lcdCreateChar(&lcd, cuadrado_vacio, 0);
>    LCD_Home(&lcd);
>    lcdShowCursor(&lcd); //cl  rw
>    lcdBlink(&lcd);
>    menu(&lcd);
>    while (1) {
>        if (!RB7) { // Boton en RB7.
>            while (!RB7);
>            //Imprimo ubicacion.
>            lcdClear(&lcd);
>            uint8_t col = lcd.cCol, row = lcd.cRow;
>            setCursor(&lcd, 0, 0);
>            lcdPrint(&lcd, "Ubicacion: ");
>            setCursor(&lcd, 1, 1);
>            char buf[6];
>            sprintf(buf, "%d,%d", col, row);
>            lcdPrint(&lcd, buf);
>            __delay_ms(900);
>            menu(&lcd);
>        } else if (!RA0) { //RIGHT
>            setCursor(&lcd, lcd.cCol + 3, lcd.cRow);
>            if (lcd.cCol >= lcd.cols) setCursor(&lcd, 1, lcd.cRow);
>            while (!RA0);
>        } else if (!RA1) { //LEFT
>            setCursor(&lcd, lcd.cCol - 3, lcd.cRow);
>            if (lcd.cCol < 0) setCursor(&lcd, 13, lcd.cRow);
>            while (!RA1);
>        }
>    }
>}
>
> ```
>
> El codigo previo, segun el dashboard de MPLAB X, llena un 46%, y 65%, la memoria, y la memoria de programa respectivamente. De ser optimizado el codigo de main() y menu() podria reducirse, la libreria por si sola solo ocupa el espacio de struct (en memoria de uso general), el programa se expande mientras mas llamados a funciones hagamos
>
>
>
