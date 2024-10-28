Adafruit_NeoPixel strip1(NUM_PIXELS_1, PIN_STRIP1, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip2(NUM_PIXELS_2, PIN_STRIP2, NEO_RGB + NEO_KHZ800);

// ledmapping starts at the bottom
struct led_t {
  int strip;
  int led;
};

Adafruit_NeoPixel ledstrips[] = {strip1, strip2};
led_t led_mapping[NR_VERTICAL_LEDS][NR_LEDS_PER_POS];

void init_led_mapping() {
  // Setup ledstrips
  for(int i=0; i<NR_LEDS_PER_POS; i++) {
      ledstrips[i].begin();
      ledstrips[i].setBrightness(128);
  }
  
  for(int i=0; i<10; i++) {
    // Define onderste strips
    led_mapping[i][0].strip = 0;
    led_mapping[i][0].led = 9-i;

    led_mapping[i][1].strip = 0;
    led_mapping[i][1].led = 10 + i;
  }

  for(int i=10; i<NR_VERTICAL_LEDS; i++) {
      led_mapping[i][0].strip = 1;
      led_mapping[i][0].led = i-10;

      // Geen tweede led
      led_mapping[i][1].strip = -1;
      led_mapping[i][1].led = -1;
  }
}

void set_vertical_led(int lednr, uint8_t red, uint8_t green, uint8_t blue) {
  //uint32_t color =  (uint32_t)green<<16 & (uint32_t)blue<<8 & (uint32_t)red;
  //set_vertical_led(lednr, color); 
  for(int i=0; i<NR_LEDS_PER_POS; i++) {
    if(led_mapping[lednr][i].strip >= 0 && led_mapping[lednr][i].strip < NR_LEDS_PER_POS) {
      ledstrips[led_mapping[lednr][i].strip].setPixelColor(led_mapping[lednr][i].led, green, red, blue);
    }
  }
}

void set_vertical_led(int lednr, uint32_t color) {
  for(int i=0; i<NR_LEDS_PER_POS; i++) {
    if(led_mapping[lednr][i].strip >= 0 && led_mapping[lednr][i].strip < NR_LEDS_PER_POS) {
      ledstrips[led_mapping[lednr][i].strip].setPixelColor(led_mapping[lednr][i].led, color);
    }
  }
}

void show_led_strips(int brightness) {
  for(int i=0; i<NR_LEDS_PER_POS; i++) {
      ledstrips[i].setBrightness(brightness);
      ledstrips[i].show();  // send the updated pixel colors to the WS2812B hardware.
  }
}
