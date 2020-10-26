// Radiofrecuencia https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver(2000, 11, 12, -1); //Default settings (Speed = 2000bps; RX pin = 11; TX pin = 12; Ptt pin = 10) RX pin (receiver) - TX pin (transmitter)


// Controlador motor MX1919 2v - 10v. PWM de 0 - 255
int D_delante = 4;
int D_atras = 5;
int I_delante = 7;
int I_atras = 6;

int horizontal = 0;
int vertical = 0;

struct Data {
    int centerx1 = -1;
    int centery1 = -1;
    int x1 = 0;
    int y1 = 0;
};

Data data;


void setup(){
    digitalWrite (LED_BUILTIN, HIGH);
    
    // Controlador motor MX1919
    pinMode(D_delante, OUTPUT);
    pinMode(D_atras, OUTPUT);
    pinMode(I_delante, OUTPUT);
    pinMode(I_atras, OUTPUT);
    
    detener();
    
    //Serial.begin(9600);

    // Radio Frecuencia
    if (driver.init()){digitalWrite (LED_BUILTIN, LOW);}
    
    delay(500);
}


void loop(){
    uint8_t buflen = sizeof(data);
    if (driver.recv((uint8_t*)&data, &buflen)){
        if (data.x1 != 0 && data.centerx1 == -1){data.centerx1 = data.x1;}
        if (data.y1 != 0 && data.centery1 == -1){data.centery1 = data.y1;}

        process(data.x1, data.y1);
    }
}


void detener(){
    analogWrite(D_atras, 0);
    analogWrite(D_delante, 0);
    analogWrite(I_atras, 0);
    analogWrite(I_delante, 0);
}

/*
void avanzar(int pwm){
    analogWrite(D_atras, 0);
    analogWrite(D_delante, pwm);
    analogWrite(I_atras, 0);
    analogWrite(I_delante, pwm);
}

void retroceder(int pwm){
    analogWrite(D_atras, pwm);
    analogWrite(D_delante, 0);
    analogWrite(I_atras, pwm);
    analogWrite(I_delante, 0);
}
*/

void mover(int vertical, int horizontal){
    if (horizontal < 0){
    // izquierda

        // Adelante
        if (vertical > 0){
            int val1 = constrain(vertical, 0, 255);
            analogWrite(D_atras, 0);
            analogWrite(D_delante, val1);

            int val2 = constrain(val1 + horizontal, 0, 255);
            analogWrite(I_atras, 0);
            analogWrite(I_delante, val2);
        }
        // Atras
        else if (vertical < 0){
            int val1 = constrain(vertical*-1, 0, 255);
            analogWrite(I_atras, val1);
            analogWrite(I_delante, 0);
            
            int val2 = constrain(val1 + horizontal, 0, 255);
            analogWrite(D_atras, val2);
            analogWrite(D_delante, 0);
        }
    }
    
    else if (horizontal > 0){
    // derecha
        if (vertical > 0){
            int val1 = constrain(vertical, 0, 255);
            analogWrite(I_atras, 0);
            analogWrite(I_delante, val1);

            int val2 = constrain(val1 - horizontal, 0, 255);
            analogWrite(D_atras, 0);
            analogWrite(D_delante, val2);
        }
        else if (vertical < 0){
            int val1 = constrain(vertical*-1, 0, 255);
            analogWrite(D_atras, val1);
            analogWrite(D_delante, 0);
            
            int val2 = constrain(val1 - horizontal, 0, 255);
            analogWrite(I_atras, val2);
            analogWrite(I_delante, 0);
        }

    }
    else{
        if (vertical > 0){
            int val = constrain(vertical, 0, 255);
            analogWrite(I_atras, 0);
            analogWrite(I_delante, val);
            analogWrite(D_atras, 0);
            analogWrite(D_delante, val);
        }
        else if (vertical < 0){
            int val = constrain(vertical*-1, 0, 255);
            analogWrite(I_atras, val);
            analogWrite(I_delante, 0);
            analogWrite(D_atras, val);
            analogWrite(D_delante, 0);
        }  
    }
  
}


void process(int x, int y){
    horizontal = constrain(map(x, 0, 1023, -255, 255), -255, 255);
    vertical = constrain(map(y, 0, 1023, -255, 255), -255, 255);
    
    if (vertical < 110 && vertical > -110){vertical = 0;}
    if (horizontal < 110 && horizontal > -110){horizontal = 0;}
    
    /*
    Serial.print("Centro: ");Serial.print(data.centerx1);Serial.print(" ");Serial.print(data.centery1);
    Serial.print(" x: "); Serial.print(data.x1); Serial.print(" "); Serial.print(horizontal);
    Serial.print(" y: "); Serial.print(data.y1); Serial.print(" "); Serial.println(vertical);
    */
    
    if (vertical == 0){detener();}
    else{mover(vertical, horizontal);}
}
