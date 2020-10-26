#include <RH_ASK.h> // https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h>
#endif

RH_ASK driver(2000, 11, 12, -1);  // Default settings (Speed = 2000bps; RX pin = 11; TX pin = 12; Ptt pin = 10) RX pin (receiver) - TX pin (transmitter)

struct Data {
    int x = 0;
    int y = 0;
    int motor = 0;
};

Data data;

/*
 * D12 emisor RF433 
 * A2 joystick x
 * A3 joystick y
 * A1 potenciometro
 */

void setup() {
    digitalWrite (LED_BUILTIN, HIGH);
    //pinMode(11, INPUT_PULLUP);
    Serial.begin(9600);

    if (!driver.init()){digitalWrite (LED_BUILTIN, LOW);}
    //else{Serial.println(F("Joystick INIT"));}

    delay(500);
}

void loop() {
    joystick();
    potenciometro();
    emitir();
    delay(100);

    Serial.print(data.x); Serial.print(" "); Serial.print(data.y); Serial.print(" "); Serial.println(data.motor);
}

void emitir() {
    bool envio = driver.send((uint8_t*)&data, sizeof(struct Data));
    driver.waitPacketSent();
    /*
     if (envio) {
        Serial.print("size: "); Serial.print(sizeof(struct Data)); Serial.print(" grados: "); Serial.print(data.servo);
        Serial.print(" x: "); Serial.print(data.x); Serial.print(" y: "); Serial.println(data.y);
    }
    */
}

void joystick() {
    // 0-512 513-1023
    data.x = analogRead(A2);
    data.y = analogRead(A3);

    /*
    static unsigned long ultima = 0;
    unsigned long tiempo = millis();
    if (tiempo - ultima > 20) {
        data.pulsador = digitalRead(11);
    }
    ultima = tiempo;
    */
}

void potenciometro(){
    data.motor = constrain(analogRead(A1), 0, 1023);
}
