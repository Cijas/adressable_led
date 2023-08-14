//version 1.0.0 - sunlight_experience - this project uses the SunPosition library available on https://github.com/GyverLibs/SunPosition
//also uses the FastLED library to control the rgb phase on the LED strip, RTClib to get the real time

#include <FastLED.h>
#include <SunPosition.h>
#include <Wire.h>

// DS3231RTC - Version: Latest
#include <RTClib.h>

RTC_DS3231 rtc;

//Pin 6 as the output pin to the LED strip
#define LED_PIN   6
#define NUM_LEDS  72

//configure the rgb by the number
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  Wire.begin();
  rtc.begin();
}

void loop() {
  DateTime now = rtc.now();

  //São Paulo latitude and longitude
  float latitude = -23.5;
  float longitude = -46.6;

  //instance of SunPosition library
  SunPosition sunPosition(latitude, longitude, now.unixtime());

  //get the sun position values
  float sunAltitude = sunPosition.altitude();
  float sunAzimuth = sunPosition.azimuth();

  //define color phases
  CRGB dawn_color(255, 150, 0); //light orange
  CRGB day_color(255, 255, 255);
  CRGB dusk_color(255, 50, 0); //dark orange
  CRGB night_color(0, 0, 50); //dark blue

  //define color sequence
  CRGB colors[] = {dawn_color, day_color, dusk_color, night_color};

  //color sequence index
  int color_index = map(sunAltitude, -90, 90, 0, 3);
  color_index = constrain(color_index, 0, 3);

  //current color based in sun phase
  CRGB current_color = colors[color_index];

  //define the brightness based in angle elevation
  int brightness = map(sunAltitude, -90, 90, 0, 255);
  brightness = constrain(brightness, 0, 255);

  //set the color and brightness to each LED on the strip
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = current_color;
    leds[i].fadeLightBy(brightness);
  }

  //Refresh the LED strip

  //Wait a short time to loop again
  delay(1000);
}



//____________________________________________ OBSOLETE UNFUNCTIONAL CODE ________________________________________________________
/*  //divide o dia em 4 partes
  int interval = 86400 / 4;

  //indice da sequencia
  int color_index = (seconds / interval) % 4;

  CRGB current_color = colors[color_index];

  int brightness = map(sunCoords.declination(), 0, 90, 0, 255);

                   //cores na fita de led
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = current_color;
    leds[i].fadeLightBy(brightness);
  }

  FastLED.show();

  delay(1000);
}

  /*double spLatitude = -23.5489;
  double spLogitude = -46.6388;
  double timeZone = 60 * (-3);

  //SUN POSITION (latitude (degrees), longitude (degrees), unix time (seconds), time zone in hours or minutes. The calculation will be done
  SunPosition sunCoords(spLatitude, spLogitude, now.second(), timeZone);
  //SunPosition sunCoords(now.year(), now.month(), now.day() now.hour(), now.minute(), now.second(), 45.0, 25.0);

  //sun phase calculate
  int sunPhase = getSunPhase(sunCoords);*/
  //datetime in seconds
  //int seconds = sunCoords.hour * 3600 + sunCoords.minutes * 60 + sunCoords.seconds;