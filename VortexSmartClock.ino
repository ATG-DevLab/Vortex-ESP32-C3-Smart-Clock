/*
===============================================================================
 Project Name : Vortex ESP32-C3 Smart Clock
 Developed By : Ashish Tech Guruji (ATG)
 Author       : Ashish Prajapati

 Description  :
 An IoT-based smart clock built using the ESP32-C3 Super Mini, SSD1306 OLED
 display, DS3231 RTC module, DHT11 temperature and humidity sensor, and TTP223
 touch sensor. Features include digital timekeeping, Wi-Fi/NTP synchronization
 support, touch-based controls, and environmental monitoring.

 YouTube      : Ashish Tech Guruji
 Channel Link : https://www.youtube.com/@AshishTechGuruji

 Repository   : Vortex ESP32-C3-Smart-Clock
 License      : MIT License

===============================================================================
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <DHT.h>
#include <WiFi.h>
#include "time.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc; // Use RTC_DS1307 rtc; if you have DS1307

#define DHTPIN 4     // DHT11 Data Pin
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// --- Wi-Fi Access Point and NTP Network Configuration ---
const char* apSsid     = "ATG Smart Clock";     // Clock Hotspot Name
const char* apPassword = "12345678";              // Default Hotspot Password

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;      // IST offset for India (+5:30 hours = 19800 seconds)
const int   daylightOffset_sec = 0;

bool timeSyncedOnce = false;

// --- Temperature and Humidity Calibration (Offsets) ---
const float TEMP_OFFSET = 0.0;     
const float HUMIDITY_OFFSET = 0.0; 

const int touchPin = 3; // TTP223 Touch Sensor Pin

// System Modes and States
enum DisplayMode {
  MODE_PERMANENT_CLOCK,    // Mode S: Permanent Clock Mode
  MODE_PERMANENT_WEATHER,  // Mode S: Permanent Weather Mode
  MODE_TEMP_TEMPORARY      // Mode N: Temporary 5-second mode via short touch
};

DisplayMode currentMode = MODE_PERMANENT_CLOCK;

// Time Format (True = 12-Hour format with AM/PM, False = 24-Hour format)
bool is12HourFormat = false; 

unsigned long pressStartTime = 0;
unsigned long weatherStartTime = 0;
const unsigned long weatherDisplayDuration = 5000; // 5 seconds temporary display duration
const unsigned long longPressTime = 2000;          // 2 seconds for long press

// Variables for 3-tap detection
int tapCount = 0;
unsigned long lastTapTime = 0;
const unsigned long tapWindow = 1000; // 3 taps within 1 second

bool lastTouchState = LOW;
bool actionTaken = false; 

void setup() {
  Serial.begin(115200);
  
  // ESP32-C3 I2C Pins (SDA=5, SCL=6)
  Wire.begin(5, 6);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Start Wi-Fi Access Point immediately without blocking normal clock startup
  WiFi.softAP(apSsid, apPassword);
  Serial.println("Hotspot started: ATG Smart Clock");

  rtc.begin();
  dht.begin();

  pinMode(touchPin, INPUT);

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // Check if a phone/device is connected to the clock's hotspot
  bool wifiConnectedFlag = (WiFi.softAPgetStationNum() > 0);

  // If connected and time hasn't been synced in this session, sync time via NTP
  if (wifiConnectedFlag && !timeSyncedOnce) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo, 2000)) {
      rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, 
                          timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
      timeSyncedOnce = true;
      Serial.println("Time synchronized successfully via connected phone!");
    }
  }

  // Reset sync flag if phone disconnects so it can re-sync next time it connects
  if (!wifiConnectedFlag) {
    timeSyncedOnce = false;
  }

  int currentTouchState = digitalRead(touchPin);
  unsigned long currentTime = millis();

  // Detect touch press down
  if (currentTouchState == HIGH && lastTouchState == LOW) {
    pressStartTime = currentTime;
    actionTaken = false;
  }
  
  // Long press (> 2 seconds) -> Toggle between permanent clock and permanent weather (Mode S)
  if (currentTouchState == HIGH && !actionTaken) {
    if (currentTime - pressStartTime >= longPressTime) {
      if (currentMode == MODE_PERMANENT_WEATHER) {
        currentMode = MODE_PERMANENT_CLOCK;
      } else {
        currentMode = MODE_PERMANENT_WEATHER;
      }
      actionTaken = true; 
      tapCount = 0; 
    }
  }

  // On touch release (Short Press)
  if (currentTouchState == LOW && lastTouchState == HIGH) {
    if (!actionTaken && (currentTime - pressStartTime < longPressTime)) {
      
      // --- 3-Tap Detection (12/24 Hour Format Toggle) ---
      if (currentTime - lastTapTime <= tapWindow) {
        tapCount++;
      } else {
        tapCount = 1; 
      }
      lastTapTime = currentTime;

      if (tapCount >= 3) {
        is12HourFormat = !is12HourFormat; 
        tapCount = 0; 
      } 
      else {
        // Normal short tap action
        if (currentMode == MODE_PERMANENT_CLOCK) {
          currentMode = MODE_TEMP_TEMPORARY; // Show weather for 5 seconds (Mode N)
          weatherStartTime = currentTime;
        }
        else if (currentMode == MODE_PERMANENT_WEATHER) {
          currentMode = MODE_PERMANENT_CLOCK; // Return to permanent clock
        }
      }
    }
  }
  
  lastTouchState = currentTouchState;

  if (tapCount > 0 && (millis() - lastTapTime > tapWindow)) {
    tapCount = 0;
  }

  // Temporary mode (5 seconds Mode N) timer check
  if (currentMode == MODE_TEMP_TEMPORARY) {
    if (millis() - weatherStartTime >= weatherDisplayDuration) {
      currentMode = MODE_PERMANENT_CLOCK;
    }
  }

  // --- Display Rendering ---
  DateTime now = rtc.now();
  display.clearDisplay();
  
  // 1. Mode indicator at top left corner ('Mode:S' or 'Mode:N')
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (currentMode == MODE_TEMP_TEMPORARY) {
    display.print("Mode:N"); 
  } else {
    display.print("Mode:S"); 
  }

  // 2. 'A T G' header at the center
  display.setCursor(53, 0); 
  display.print("A T G");

  // 3. Show AM/PM at top-right corner if 12-hour format is active
  if (is12HourFormat) {
    int displayHourForAmPm = now.hour();
    String topSuffix = (displayHourForAmPm >= 12) ? "PM" : "AM";
    display.setCursor(108, 0); 
    display.print(topSuffix);
  }
  
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  if (currentMode == MODE_PERMANENT_WEATHER || currentMode == MODE_TEMP_TEMPORARY) {
    // --- Temperature and Humidity Page (Using your custom coordinates) ---
    float h = dht.readHumidity() + HUMIDITY_OFFSET;
    float t = dht.readTemperature() + TEMP_OFFSET;

    // Header text "Temperature" at center (X = 28)
    display.setTextSize(1);
    display.setCursor(28, 16); 
    display.print("Temperature"); 

    if (isnan(h - HUMIDITY_OFFSET) || isnan(t - TEMP_OFFSET)) {
      display.setTextSize(1);
      display.setCursor(28, 38);
      display.print("Sensor Error!");
    } else {
      // Temperature value set at X = 26
      display.setTextSize(2);
      display.setCursor(26, 30);
      display.print(t, 1);
      display.print(" C");
      
      // Humidity value set at X = 27
      display.setTextSize(1);
      display.setCursor(27, 53);
      display.print("Humidity:");
      display.print((int)h);
      display.print("%");
    }
  } 
  else {
    // --- Clock Mode ---
    display.setTextSize(2);

    int displayHour = now.hour();

    if (is12HourFormat) {
      if (displayHour == 0) {
        displayHour = 12;
      } else if (displayHour > 12) {
        displayHour -= 12;
      }
      display.setCursor(34, 22); // 12-Hour center position
    } else {
      display.setCursor(16, 22); // 24-Hour center position
    }

    if (displayHour < 10) display.print("0");
    display.print(displayHour);
    display.print(":");
    
    if (now.minute() < 10) display.print("0");
    display.print(now.minute());
    
    if (!is12HourFormat) {
      display.print(":");
      if (now.second() < 10) display.print("0");
      display.print(now.second());
    }

    // Date (DD/MM/YYYY) center position
    display.setTextSize(1);
    display.setCursor(33, 50); 
    
    if (now.day() < 10) display.print("0");
    display.print(now.day());
    display.print("/");
    if (now.month() < 10) display.print("0");
    display.print(now.month());
    display.print("/");
    display.print(now.year());
  }

  // --- Wi-Fi Connected Icon at Bottom-Right Corner ---
  if (wifiConnectedFlag) {
    display.drawPixel(122, 60, SSD1306_WHITE);
    display.drawPixel(120, 58, SSD1306_WHITE);
    display.drawPixel(124, 58, SSD1306_WHITE);
    display.drawPixel(118, 56, SSD1306_WHITE);
    display.drawPixel(126, 56, SSD1306_WHITE);
  }

  display.display();
  delay(50);
}
