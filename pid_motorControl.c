//Introduction: PID control, DC motor control via PWM pin, on ATmega328 With
//setpoint & sensor.
//Software: Arduino, Wiring-language
//Hardware: Arduino-Uno
//**************************************************************************************************/

//Define Variables we'll be connecting to
double Setpoint, Input, tmp = 0;
const int numReadings = 10;
int readings[numReadings]; // the readings from the analog input
int index = 0;             // the index of the current reading
int total = 0;             // the running total
int average = 0;           // the average
int Duty_C = 0;
int error = 0;
int prev_error = 0;
int out = 0;
double P_Gain = 2, I_Gain = 0.3, D_Gain = 0.3; // PID Coefficients example
//I/O pin numbers
int sensorPin = 0;
int setpointPin = 1;
int PWM_Pin = 3;

//------------------------------------PID Function ----------------------
int pid(int error_ctrl, int prev_error_ctrl)
{
    double p = 0, i = 0, d = 0;
    int sum = 0;

    p = P_Gain * error;                          //Proportional part
    i = I_Gain * (error_ctrl + prev_error_ctrl); //integral part
    d = D_Gain * (error_ctrl - prev_error_ctrl); //derivative part

    sum = p + i + d; // PID
    return sum;
} //

void setup()
{
    //initialize the variables we're linked to
    //read from sensor (LM35DH)
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
        readings[thisReading] = 0;

    Setpoint = analogRead(1); //potentiometer 0-5 V
    pinMode(PWM_Pin, out); // DC motor control
}

//----------------------------------- "Main" function ----------------------------------
void loop()
{
    Setpoint = analogRead(setpointPin); //reading setpoint
    delay(100);                         //100ms delay
                                        //Reading temperature average of 10 samples
    // subtract the last reading:
    total = total - readings[index];
    // read from the sensor:
    readings[index] = analogRead(sensorPin);
    // add the reading to the total:
    total = total + readings[index];
    // advance to the next position in the array:
    index = index + 1;
    delay(50); //50ms delay
    // if we're at the end of the array...
    if (index >= numReadings)
        // ...wrap around to the beginning:
        index = 0;
    // calculate the average:
    average = total / numReadings;
    // send it to the computer (as ASCII digits)

    delay(20); //20ms delay

    //Temperature = (5*average*100)/1024
    if (average <= 20) // if the temperature is too low (equal to or less than 10 degrees)
    {
        digitalWrite(13, HIGH); //turn the blue LED on, which is connected to the digital pin 13
        out = 0;
        analogWrite(PWM_Pin, out); // Fan off
        delay(100);
    }
    /*
 else if (average > 52) /
 {
 analogWrite(PWM_Pin, 255); //Fan Full speed , it's too hot!
 delay(100);
 }*/

    else //otherwise calculate the needed speed
    {
        tmp = analogRead(1); //reading setpoint
        Setpoint = tmp / 10;
        delay(100);                 //100ms delay
        error = average - Setpoint; //calculate the difference
        delay(10);                  //10ms delay

        if (error > 1)
        {
            Duty_C = pid(error, prev_error); // call pid function
            out = error;                     //+ 32; //run fan , calculated error + minimum output to run the fan
            delay(200);                      //200ms delay

            analogWrite(PWM_Pin, out); // write to the Fan
            digitalWrite(13, HIGH);    // write to the blue cool diod
            delay(500);                //0.5s delay

            prev_error = error; //save old error to be used by the PID function
            Duty_C = 0;         // reset register
        }
        else
        {
            out = 0;
            analogWrite(PWM_Pin, out); // motor off it's too cold and start blinking the blue diod
            digitalWrite(13, HIGH);    // write to the blue cool diod
            delay(200);
            digitalWrite(13, LOW); // write to the blue cool diod
            delay(200);
        }
    }
}
//End Main
// End Program