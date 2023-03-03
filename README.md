# LCD PIC16F628A

Libreria para controlar un display lcd 16x2 con el PIC16F628A

## ¿Por que?

Muchas veces uno se puede encontrar con la necesidad de tener un proyecto de menor escala, y que sea vistoso, y tambien se puede juntar con el hobby/estudio de la electronica. La poca documentacion, y disponibilidad de videos actualizados en el tema, hicieron que me dedique a crear esta libreria.

## Instalacion

Simple, los 2 archivos deben de arrastrarse, ya sea al /include del XC8, o donde sea, debe de linkearse con el compilador de XC8, asi se entera el mismo, la existencia de la libreria.

## Uso

Es similar a Liquid Crystal de Arduino, solo se deben de definir los pines a utilizar (En el P16F628A **NO** se puede utilizar RA5, ya que no tiene un __Output driver__)
```c
void main(){
    // [...] 👈 Configuracion del PIC, los pines a utilizar DEBEN estar configurados como salidas (en P16F62X, estableciendo '0').
    lcd_config_t lcd = LCD_INIT_CONFIG( //La macro auto-rellena el struct por vos...
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
    setCursor(&lcd, 0, 0); //A todas las funciones hay que pasarle la referencia al lcd..
    lcdPrint(&lcd, "Hola Mundo!"); // 👋 👋
    while(1);
    return;
}
```

La cantidad de funciones puede ser larga, recomiendo revisar los archivos LCD.h y LCD.c para saber bien como funcionan.