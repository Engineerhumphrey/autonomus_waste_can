#include <Servo.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define IR_SENSOR_PIN 3
#define LED_PIN 7
#define MOTOR_DIR 4
#define MOTOR_PWM 5
#define MOTOR_ENCODER 6
#define LDR_PIN A0

int distance;
int lightLevel;
String wasteType = "";

void setup() {
    Serial.begin(9600);  // UART Communication via FT232R
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(MOTOR_DIR, OUTPUT);
    pinMode(MOTOR_PWM, OUTPUT);
    pinMode(MOTOR_ENCODER, INPUT);
    pinMode(LDR_PIN, INPUT);
}

void loop() {
    // Measure distance using HC-SR04
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    distance = pulseIn(ECHO_PIN, HIGH) / 58;  // Convert to cm

    // Read AI Camera Data from Serial (FT232R)
    if (Serial.available()) {
        wasteType = Serial.readString();
        Serial.print("Detected Waste: ");
        Serial.println(wasteType);
    }

    // Check IR Proximity Sensor
    if (digitalRead(IR_SENSOR_PIN) == HIGH) {
        Serial.println("Waste detected!");

        // Read Light Level (Torch LDR)
        lightLevel = analogRead(LDR_PIN);
        Serial.print("Light Level: ");
        Serial.println(lightLevel);

        // Control Motor Based on Waste Type
        if (wasteType == "PLASTIC") {
            Serial.println("Sorting Plastic...");
            digitalWrite(MOTOR_DIR, HIGH);
            analogWrite(MOTOR_PWM, 150);
            delay(2000);
            analogWrite(MOTOR_PWM, 0);
        }
        else if (wasteType == "METAL") {
            Serial.println("Sorting Metal...");
            digitalWrite(MOTOR_DIR, LOW);
            analogWrite(MOTOR_PWM, 180);
            delay(2000);
            analogWrite(MOTOR_PWM, 0);
        }
        else {
            Serial.println("Unknown Waste Type!");
        }

        // LED Indicator
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
    }

    delay(500);
}
