// DHT and PIR sensor test code 
// Written by SupaCyanjin

#include "DHT.h"

#define LEDPIN 13       // led
#define DHTPIN 12       // DHT sensor data in
#define PIRPIN 11       // PIR sensor data in
#define BUZPIN 10       // buzzer digi out
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.

DHT dht(DHTPIN, DHTTYPE);
const int interval = 5000;        // interver for DHT sensor 5s
unsigned long previousMillis = 0; 
int pirState = LOW;               // we start, assuming no motion detected
int val = 0;                      // variable for reading the pin status

void setup() {
    Serial.begin(9600);
    Serial.println("DHT and PIR test!");
    pinMode(LEDPIN, OUTPUT);      // declare LED as output
    pinMode(PIRPIN, INPUT);       // declare sensor as input
    pinMode(BUZPIN, OUTPUT);      // declare buzzer as output
    digitalWrite(PIRPIN, LOW);
    dht.begin();

}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        float h = dht.readHumidity();
        // Read temperature as Celsius (the default)
        float t = dht.readTemperature();
        // Read temperature as Fahrenheit (isFahrenheit = true)
        float f = dht.readTemperature(true);

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t) || isnan(f)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        // Compute heat index in Fahrenheit (the default)
        float hif = dht.computeHeatIndex(f, h);
        // Compute heat index in Celsius (isFahreheit = false)
        float hic = dht.computeHeatIndex(t, h, false);

        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.print(" *C ");
        Serial.print(f);
        Serial.print(" *F\t");
        Serial.print("Heat index: ");
        Serial.print(hic);
        Serial.print(" *C ");
        Serial.print(hif);
        Serial.println(" *F");
    }
    else {
        val = digitalRead(PIRPIN);  // read input value
        if (val == HIGH) {            // check if the input is HIGH
            digitalWrite(LEDPIN, HIGH);  // turn LED ON
            playTone(300, 160);
            delay(50);


            if (pirState == LOW) {
                // we have just turned on
                Serial.println("Motion detected!");
                // We only want to print on the output change, not state
                pirState = HIGH;
            }
        } else {
            digitalWrite(LEDPIN, LOW); // turn LED OFF
            playTone(0, 0);
            delay(50);    
            if (pirState == HIGH){
                // we have just turned of
                Serial.println("Motion ended!");
                // We only want to print on the output change, not state
                pirState = LOW;
            }
        }
    }
}
// duration in mSecs, frequency in hertz
void playTone(long duration, int freq) {
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(BUZPIN,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(BUZPIN, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}
