#include <Arduino.h>
#include <BluetoothSerial.h>
#include "HMS.h"
#include "Adafruit_SHT31.h"
#include "ACS712.h"

#define DEBUG 0

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x) Serial.printf(x)
#define debugCalibrateAmps()
void calibrateAmps()
{
  if (SerialBT.available())
  {
    char c = SerialBT.read();
    switch (c)
    {
    case '+':
      ACS.incMidPoint();
      break;
    case '-':
      ACS.decMidPoint();
      break;
    case '0':
      ACS.setMidPoint(512);
      SerialBT.print("," + ACS.getMidPoint());
      break;
    case '*':
      ACS.setmVperAmp(ACS.getmVperAmp() * 1.05);
      break;
    case '/':
      ACS.setmVperAmp(ACS.getmVperAmp() / 1.05);
      SerialBT.print("," + ACS.getmVperAmp());
      break;
    default:
      SerialandBT("No input detected");
    }
  }
  delay(1000);
}
#else
#define debug(x)
#define debugln(x)
#define debugf(x)
#define debugCalibrateAmps()
#endif

#define LED1 12
#define LED2 9
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 30       /* Time ESP32 will go to sleep (in seconds) */
bool runningState;

// ACS712 5A  uses 185 mV per A
// ACS712 20A uses 100 mV per A
// ACS712 30A uses  66 mV per A

RTC_DATA_ATTR int bootCount = 0;

//Method to print the reason by which ESP32
//has been awaken from sleep

void print_wakeup_reason()
{
  runningState = true;
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    debugf("Wakeup was not caused by deep sleep: %d\n" + wakeup_reason);
    break;
  }
}

uint8_t _amppin = 18;
ACS712 ACS(_amppin, 5.0, 4095, 100);
// ESP 32 example (requires resistors to step down the logic voltage)
//ACS712  ACS(25, 5.0, 4095, 185);

HMS HMSmain = HMS();

BluetoothSerial SerialBT;

int received;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

/* void led2OnOff(int time)
{
  digitalWrite(LED2, HIGH);
  delay(time);
  digitalWrite(LED2, LOW);
  delay(time);
} */

void ledFade(int LED)
{
  runningState = true;
  int freq = 1000;
  int ledChannel = 0;
  int resolution = 16;
  int dutyCycle = 0;

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
  dutyCycle = map(3, 0, 4095, 0, 32767);
  ledcWrite(ledChannel, dutyCycle);
  // increase the LED brightness
  for (int dutyCycle = 0; dutyCycle <= 4095; dutyCycle++)
  {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }

  // decrease the LED brightness
  for (int dutyCycle = 4095; dutyCycle >= 0; dutyCycle--)
  {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
}

void SerialandBT(String data)
{
  runningState = true;
/* #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` enable it
#endif */
  bool BTwritestate = false;
  if (BTwritestate = true)
  {
    ledFade(LED2);
  }
  while (true)
  {
    BTwritestate = true;

    if (SerialBT.available())
    {
      SerialBT.print(data); //write on BT app
      Serial.write(SerialBT.read());
    }
    Serial.print(data);
  }
}

void readAmps()
{
  runningState = 1;
  int mA = ACS.mA_DC();
  String Amps = String(mA);
  SerialandBT(Amps);
}

void floattostring()
{
  runningState = true;
  float *climatedata = HMSmain.climate();
  char climateData[100];
  sprintf(climateData, "%3d, %3d", climatedata[0], climatedata[1]);
  SerialandBT(climateData);

  String voltageaverage = "";
  float *readvoltage = HMSmain.readSensAndCondition();
  for (int i = 0; i < sizeof(readvoltage); i++)
  {
    char temp[100];
    sprintf(temp, "%s, %3f", voltageaverage, readvoltage[i]);
    voltageaverage = temp;
  }
  SerialandBT(voltageaverage);
}

void setup()
{
  runningState = true;
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  +bootCount;
  char bootChar[100];
  sprintf(bootChar, "Boot number: %s", String(bootCount));
  debugf(bootChar);

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  //First we configure the wake up source
  //We set our ESP32 to wake up every 5 seconds

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  char sleeptime[1];
  sprintf(sleeptime, "Setup ESP32 to sleep for every ", String(TIME_TO_SLEEP) + " Seconds");
  debugf(sleeptime);
  debugf("HMS booting - please wait");
  if (!sht31.begin(0x44))
  { // Set to 0x45 for alternate i2c addr
    debugf("Couldn't find SHT31");
    while (1)
      delay(1);
  }
  SerialBT.begin("ESP32_HMS");
  debugf("Device now Discoverable");
  debugf(__FILE__);
  ACS.autoMidPoint();
  debugf("Setup Complete");
  delay(100);

  /*
  Next we decide what all peripherals to shut down/keep on
  By default, ESP32 will automatically power down the peripherals
  not needed by the wakeup source, but if you want to be a poweruser
  this is for you. Read in detail at the API docs
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  Left the line commented as an example of how to configure peripherals.
  The line below turns off all RTC peripherals in deep sleep.
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");
  Now that we have setup a wake cause and if needed setup the
  peripherals state in deep sleep, we can now start going to
  deep sleep.
  In the case that no wake up sources were provided but deep
  sleep was started, it will sleep forever unless hardware
  reset occurs
  

  ledtestOnOff(500); //comment out when not testing - Blink led from Unity Terminal over BTSerial */
  SerialandBT("Connection Successful");
  delay(100);
  floattostring();
  delay(100);
  debugCalibrateAmps(); // only needed for manual calibration of sensor
  debugf("Going to sleep now");
  delay(100);
  runningState = false;
  Serial.flush();
  if (!runningState)
  {
    esp_deep_sleep_start();
  }
}

void loop()
{
  //block does not execute
}
