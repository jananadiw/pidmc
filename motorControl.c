
#include <PID_v1.h>
#define triggerPin 11
#define echoPin 10

double Setpoint, Input, Output;
PID myPID(&input, &Output, &setPoint, 1, 2, 3, DIRECT);
int ledPin = 13; // LED conectado al pin digital 13
int pinA1 = 7;   // entrada Motor A 1
//int pinB1 = 8; // entrada Motor B 1
int pinEN1 = 9; // entrada EN 1
int pinA2 = 3;  // entrada a motor A2
//int pinB2= 4; // entrada a motor B2
//int pinEN2=6;

void setup()
{
    Serial.begin(9600);
    Input = analogRead(0);
    Setpoint = 100;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(pinA1, OUTPUT);
    //pinMode( pinB1, OUTPUT);
    pinMode(pinEN1, OUTPUT);
    digitalWrite(pinEN1, HIGH);
    pinMode(pinA2, OUTPUT);
    //pinMode(pinB2, OUTPUT);
    //pinMode(pinEN2, HIGH);
    Serial.begin(9600);
    myPID.SetMode(AUTOMATIC);
}

void loop()
{

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    float cm = microsecondsToCentimeters(duration);
    Input = analogRead(0);
    myPID.Compute();
    analogWrite(3, Output);

    if (cm = 7)
    {
        digitalWrite(pinA1, HIGH);
        //digitalWrite( pinB1, LOW);
        digitalWrite(pinA2, LOW);
        // digitalWrite( pinB2, LOW);
        delayMicroseconds(300);
    }
    else
    {
        digitalWrite(pinA1, HIGH);
        //digitalWrite( pinB1,LOW);
        digitalWrite(pinA2, HIGH);
        //digitalWrite( pinA2, HIGH);
    }
}
float microsecondsToCentimeters(long microseconds)
{

    float seconds = (float)microseconds / 1000000.0;
    float distance = seconds * 340;
    distance = distance / 2;
    distance = distance * 100;
    return distance;
}
