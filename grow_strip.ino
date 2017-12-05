#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

#define DEBUG 0

#define NUM_LEDS 50
#define DATA_PIN 6
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define POT_PIN 1
#define POT_LOW 333
#define POT_HIGH 667

// Use floats so we can take ratios
#define POT_MIN 120.0
#define POT_MAX 1020.0

#define INTENSITY 255

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  Serial.begin(9600);
}

void loop() {
  const int pot = analogRead(POT_PIN);

  CRGB color = getColor(pot);

  setLeds(color);

  FastLED.show();

  delay(30);
}

void setLeds(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

CRGB getColor(int rawPot) {
  // This drops the 1s digit.
  int roundedPot = 10 * (rawPot / 10);

  if (roundedPot < POT_MIN) {
    return CRGB::Black; // Off
  }

  float pot = roundedPot - POT_MIN;
  float bluePercent = pot / (POT_MAX - POT_MIN);
  float redPercent = 1 - bluePercent;

  #if DEBUG
  Serial.print(bluePercent);
  Serial.print("\t");
  Serial.println(redPercent);
  #endif

  int blue = bluePercent * INTENSITY;
  int red = redPercent * INTENSITY;

  return CRGB(/* red, green, blue */ red, 0, blue);
}

