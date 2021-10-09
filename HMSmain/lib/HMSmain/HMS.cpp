#include <BluetoothSerial.h>
#include <Wire.h>
#include <ACS712.h>
#include <Adafruit_SHT31.h>
#include <HMS.h>
#include <Arduino.h>

HMS::HMS()
{
}

int voltageValues[10];

//float input_voltage;

Adafruit_SHT31 sht31 = Adafruit_SHT31();
byte degree[8] =
    {
        0b00011,
        0b00011,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000};

float readVoltage(int pinnumber)
{
    return (float)((analogRead(pinnumber) * 5.0) / 1024.0);
}

float *HMS::readSensAndCondition()
{
    float sval[10] = {0};

    for (int i = 0; i < 5; i++)
    {
        sval[0] += readVoltage(39); // sensor on analog pin
        sval[1] += readVoltage(34);
        sval[2] += readVoltage(35);
        sval[3] += readVoltage(32);
        sval[4] += readVoltage(33);
        sval[5] += readVoltage(25);
        sval[6] += readVoltage(26);
        sval[7] += readVoltage(27);
        sval[8] += readVoltage(14);
        sval[9] += readVoltage(12);

        delay(100);
    }

    for (int i = 0; i < sizeof(sval); i++)
    {
        sval[i] = sval[i] / 5;
    }

    return sval;
}

//if (input_voltage < 0.50 && input_voltage >= 0.00 )
//{
//digitalWrite(2, HIGH);
//delay (30);
//digitalWrite(2, LOW);
//delay (30);
//}
//else if (input_voltage < 1.00 && input_voltage >= 0.50)
//{
// DO STUFF
//}
//else if (input_voltage < 1.50 && input_voltage >= 1.00)
//{
// DO STUFF
//}
//else if (input_voltage < 2.00 && input_voltage >= 1.50)
//{
// DO STUFF
//}
//else if (input_voltage < 2.50 && input_voltage >= 2.00)
//{
// DO STUFF
//}
//else if (input_voltage < 3.00 && input_voltage >= 2.50)
//{
// DO STUFF
//}
//else if (input_voltage < 3.50 && input_voltage >= 3.00)
//{
// DO STUFF
//}
//else if (input_voltage < 4.00 && input_voltage >= 3.50)
//{
// DO STUFF
//}
//else if (input_voltage < 4.50 && input_voltage >= 4.00)
//{
// DO STUFF
//}
//else if (input_voltage < 5.00 && input_voltage >= 4.50)
//{
// DO STUFF
//}

float *HMS::climate()
{
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();

    float climatedata[2];

    if (!isnan(t))
    { // check if 'is not a number'
        Serial.print("Temp *C = ");
        climatedata[0] = t;
    }
    else
    {
        Serial.println("Failed to read temperature");
    }

    if (!isnan(h))
    { // check if 'is not a number'
        Serial.print("Hum. % = ");
        climatedata[1] = h;
    }
    else
    {
        Serial.println("Failed to read humidity");
    }
    Serial.println();
    delay(1000);
    return climatedata;
}