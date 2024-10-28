#include <Adafruit_NeoPixel.h>

#include <Wire.h>
// Onderste strip
#define PIN_STRIP1  5   // Arduino pin that connects to WS2813
#define NUM_PIXELS_1     20  // The number of LEDs (pixels) on WS2813

// bovenste st
#define PIN_STRIP2  4  
#define NUM_PIXELS_2     15  // The number of LEDs (pixels) on WS2813

// For each vertical led upto 2 leds are possible
#define NR_VERTICAL_LEDS 25
#define NR_LEDS_PER_POS 2

const uint32_t red = 0x00ff00;
const uint32_t green = 0xff0000;
const uint32_t blue = 0x0000ff;

const uint32_t yellow = 0xffff00;
const uint32_t cyan = 0x00ffff;
const uint32_t magenta = 0xff00ff;

const uint32_t white = 0xffffff;
const uint32_t black = 0x000000;

int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out = 0.0;
float Y_out = 0.0;
float Z_out = 0.0;
float total = 0.0;
int X_offset = 0, Y_offset = 0, Z_offset = 0; // Offset values

const int t_duration = 300;
const int dt = t_duration / (NUM_PIXELS_1 + NUM_PIXELS_2);

void setup() {
  Serial.begin(115200);

  init_led_mapping();
  
  Wire.begin();
  configureADXL345(); // Configure the sensor
}

void configureADXL345() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8); // Enable measurement (D3 bit high)
  Wire.endTransmission();
  delay(10);
}
unsigned prev_time = 0;
unsigned start_time = 0;

void loop() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);
  X_out = (Wire.read() | Wire.read() << 8);
  X_out = X_out / 256;
  Y_out = (Wire.read() | Wire.read() << 8);
  Y_out = Y_out / 256;
  Z_out = (Wire.read() | Wire.read() << 8);
  Z_out = Z_out / 256;

  total = sqrt(X_out*X_out + Y_out*Y_out + Z_out*Z_out);

  // Loop effect
  if(total > 2.0 && start_time==0) {
    start_time = millis();
  }
  
  //int val = 100+total*50;
  //const int total_t = 500;
  int t = millis() - start_time;
  //int t_per_pixel = total_t / NUM_PIXELS;

  for(int x=0; x<NR_VERTICAL_LEDS; x++) {
    int lednr = (millis() / 1000)%NR_VERTICAL_LEDS;
    if(x == lednr) {
      set_vertical_led(x, red);
    } else {
      set_vertical_led(x, black);
    }
  }
  // Reset alles naar rood
  /*for (int pixel = 0; pixel < NUM_PIXELS_1; pixel++) {
    strip1.setPixelColor(pixel, red);
  }

  for (int pixel = 0; pixel < NUM_PIXELS_2; pixel++) {
    strip2.setPixelColor(pixel, red);
  }

  if(start_time != 0 && t < t_duration) {
    int pixel = t / dt;

    if(pixel <= NUM_PIXELS_2) {
      strip2.setPixelColor(NUM_PIXELS_2 - pixel - 1, white);
      strip2.setPixelColor(NUM_PIXELS_2 - pixel, white);
      strip2.setPixelColor(NUM_PIXELS_2 - pixel + 1, white);
    } else {
      strip1.setPixelColor(pixel - NUM_PIXELS_2 - 1 - 1, white);
      strip1.setPixelColor(pixel - NUM_PIXELS_2 - 1, white);
      strip1.setPixelColor(pixel - NUM_PIXELS_2 - 1 + 1, white);
    }
  } else {
    start_time = 0;
  }*/
  
  show_led_strips(128);
}
