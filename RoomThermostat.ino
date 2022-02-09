//Librariile folosite

#include <string>
#include <Arduino.h>

#include <FS.h>
#include <rtc_memory.h> //RTC library

#include <Bounce2.h>
#include <Wire.h>
#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h> 
#include <U8g2lib.h> //Display library

// Create Amplitude Shift Keying Object
RH_ASK rf_driver(2000, 12, 15, 0);

// Define a struct that maps what's inside the RTC memory
// Max size is 508 bytes.
typedef struct {
  float counter;
} MyData;
RtcMemory rtcMemory("/etc/trial.bin");
MyData *data = rtcMemory.getData<MyData>();

#define U8X8_HAVE_HW_I2C

#define SDA D1
#define SCL D2
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

Bounce debouncerUp = Bounce();  
Bounce debouncerDown = Bounce();

//Variabile globale
byte buttonPinUp = D3;
byte buttonPinDown = D4;
uint8_t debaunceTime = 20;
byte displaySuply = D5;
float setTemp = 5.0;
double temperature;

bool relayState;

const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 10000;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741; 

unsigned long lastMillis, startMillis, buttonMillis, currentMillis, lastUpdateDisplay, lastUpdateDht,dSleep;

bool lastButtonUp, lastButtonDown, lastButtonMode;
bool stateButtonUp, stateButtonDown, stateButtonMode;
bool displayOn=0; 


/////////////////////////////////////// SETUP ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(void) 
{ 
  Serial.begin(9600);
 // Initialize ASK Object
  rf_driver.init();
  while (!Serial);
  Serial.println();
  Serial.println("RTCMemory - Basic with Backup");
  // Remember to initialize the SPIFFS memory
  Serial.print("Filesystem initialization... ");
  if (SPIFFS.begin()) {
    Serial.println("Done!");
  } else {
    Serial.println("Error");
  }

  if (data == nullptr) {
    Serial.println("Error: In this case nullptr is correct, you have to "
                   "explicitly initialize the class!");
  }

  if (rtcMemory.begin()) {
    Serial.println("Initialization done!");
  } else {
    Serial.println("No previous data found. The memory is reset to zeros!");
    // Here you can initialize your data structure.
  }

  // Get the data
  data = rtcMemory.getData<MyData>();
  Serial.println(String("Value read: ") + data->counter);
 
  setTemp = data->counter;
  if (setTemp - temperature >= 0.2 || temperature - setTemp >= 0.2)
  {
    sendMessage();
  }

  
  u8g2.begin();  
  u8g2.enableUTF8Print();
  pinMode(displaySuply, OUTPUT);
  digitalWrite(displaySuply, HIGH);
  pinMode(buttonPinUp, INPUT_PULLUP);
  pinMode(buttonPinDown, INPUT_PULLUP);
  debouncerUp.attach(buttonPinUp);
  debouncerDown.attach(buttonPinDown);
  debouncerUp.interval(debaunceTime);
  debouncerDown.interval(debaunceTime);
  
  
  
  //updateDisplay();
  
}

/////////////////////////////////////  LOOOOOOP ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


void loop(void) 
{   
  currentMillis = millis();
  if (debouncerUp.update()) 
  {
    // Get the update value.
    stateButtonUp = debouncerUp.read();
    // Send in the new value.
    if(stateButtonUp == LOW)
    {
      buttonMillis=millis();
      if(displayOn)
      {
        setTemp += 0.5;
        updateSetpoint();
        modifyData();
      }
    }
  }
  if (debouncerDown.update()) 
  {
    // Get the update value.
    stateButtonDown = debouncerDown.read();
    // Send in the new value.
    if(stateButtonDown == LOW)
    {
      buttonMillis=millis();
      if(displayOn)
      {
        setTemp -= 0.5;
        updateSetpoint();
        modifyData();
      }
    }
  }

  if(millis() - buttonMillis > 5000)
  {
    if(displayOn)
    {
      u8g2.setPowerSave(1);
      displayOn = 0;
    }
  }
  else
  {
    if(!displayOn)
    {
      u8g2.setPowerSave(0);
      displayOn = 1;
    }
  }
  if(millis() - buttonMillis > 1000 && millis() - lastUpdateDisplay > 1000 && displayOn)
  {
    updateDisplay();
    lastUpdateDisplay = millis();
  }
   // unsigned long currentMillis = millis(); // grab current time. 
  
// if (millis() - lastUpdateDht > 10000) // 31 seconds wait in this Loop before next temperature read cycle. 
 //{ 
    
   //  updateRelayState();
   //  lastUpdateDht = millis();
// }
   if (currentMillis - buttonMillis > 5000  )
{
  u8g2.setPowerSave(1);
  displayOn = 0;
  Serial.println("\n Somn de voie.......");
  ESP.deepSleep(12e7);
 }
}
