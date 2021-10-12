/* #include <Arduino.h> */
#include <BluetoothSerial.h>
#include <dummy.h>
#include <HardwareSerial.h>

#define DEBUG 1

#if DEBUG == 1

#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x) Serial.printf(x)

#else
#define debug(x)
#define debugln(x)
#define debugf(x)
#endif

#define LED1 12
#define LED2 15
/* #define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
/* #define TIME_TO_SLEEP 30   */     /* Time ESP32 will go to sleep (in seconds) */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

String message = "";

int runningState[10];

/* RTC_DATA_ATTR int bootCount = 0; */

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_SRV_OPEN_EVT)
  {
    Serial.println("Client Connected");
  }
}

/* void print_wakeup_reason()
  {
  runningState[1];
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.printf("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.printf("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.printf("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.printf("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.printf("Wakeup caused by ULP program");
    break;
  default:
    Serial.println("Wakeup was not caused by deep sleep: %d\n" + wakeup_reason);
    break;
  }
  } */

  void ledFade(int LED)
  {
    int freq = 1000;
    int ledChannel = 0;
    int resolution = 16;
    int dutyCycle = 0;

    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(LED, ledChannel);
    dutyCycle = map(0, 0, 4095, 0, 32767);
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

  //=======

  BluetoothSerial SerialBT;
  void SerialandBT(String data)
  {
    if (SerialBT.available())
    {
      runningState[5];
      try
      {
        delay(10);
        SerialBT.println(data); //write on BT app
        Serial.println(data);
        Serial.write(SerialBT.read());

        /* if (runningState[5])
          {
          ledFade(LED1);
          }
          else
          {
          digitalWrite(LED1, LOW);
          } */
      }
      catch (...)
      {
        debugf("Bluetooth cannot be accessed. Check permissions");
      }
    }
    else
    {
      debugf("Bluetooth is not available");
    }
  }

  void ledtestOnOff(int time, int LED)
  {
    runningState[2];
    if (SerialBT.available())
    {
      char c = SerialBT.read();
      if (c != '\n')
      {
        message += String(c);
        switch (c)
        {
          case 'O':
            SerialandBT("LED ON");
            digitalWrite(LED, HIGH);
            delay(time);
            break;
          case 'F':
            delay(time);
            digitalWrite(LED, LOW);
            SerialandBT("LED OFF");
            break;
          default:
            SerialandBT("No input detected");
            break;
        }
      }
      else
      {
        message = "";
      }
      Serial.write(c);
      debugln(message);
    }
  }

  void setup()
  {
    runningState[3];
    Serial.begin(115200);
    SerialBT.register_callback(callback);

    if (!SerialBT.begin("ESP32_HMS"))
    {
      debugln("An error occurred initializing Bluetooth");
    }
    else
    {
      debugln("Bluetooth initialized");
    }
    debugln("HMS booting - please wait");
    debugln("Device now Discoverable");

    /* +bootCount;
      char bootChar[100];
      sprintf(bootChar, "Boot number: %s", String(bootCount));
      debugf(bootChar); */

    //Print the wakeup reason for ESP32
    /* print_wakeup_reason(); */

    //First we configure the wake up source
    //We set our ESP32 to wake up every 5 seconds

    /* esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); */
    /* char sleeptime[1];
      sprintf(sleeptime, "Setup ESP32 to sleep for every ", String(TIME_TO_SLEEP) + " Seconds");
      debugf(sleeptime); */
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    debugln(__FILE__);
    debugln("Setup Complete");
    delay(100);
    Serial.flush();
  }

  void loopHighpower()
  {
    runningState[4];
    ledtestOnOff(100, LED2);
    delay(100);
  }

  /* void loopLowPower()
    {
    runningState[6];
    debugf("Going to sleep now");
    esp_deep_sleep_start();
    } */

  void loop()
  {
    loopHighpower();
  }