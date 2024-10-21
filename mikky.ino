#include <Adafruit_NeoPixel.h>

#include <Wire.h>
#define PIN_WS2812B  5   // Arduino pin that connects to WS2812B
#define PIN_STRIP2  4  // Second strip
#define NUM_PIXELS     13  // The number of LEDs (pixels) on WS2812B

Adafruit_NeoPixel WS2812B(NUM_PIXELS, PIN_WS2812B, NEO_WRGB + NEO_KHZ800);
Adafruit_NeoPixel strip2(11, PIN_STRIP2, NEO_WRGB + NEO_KHZ800);

const uint32_t red = 0x00ff0000;
const uint32_t green = 0xff000000;
const uint32_t blue = 0x0000ff00;

const uint32_t yellow = 0xffff0000;
const uint32_t cyan = 0x00ffff00;
const uint32_t magenta = 0xff00ff00;

const uint32_t white = 0xffffff00;
const uint32_t black = 0x00000000;

int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out = 0.0;
float Y_out = 0.0;
float Z_out = 0.0;
float total = 0.0;
int X_offset = 0, Y_offset = 0, Z_offset = 0; // Offset values

void setup() {
  Serial.begin(115200);
  Wire.begin();
  configureADXL345(); // Configure the sensor

  WS2812B.begin(); // INITIALIZE WS2812B strip object (REQUIRED)
  WS2812B.setBrightness(50);

  strip2.begin(); // INITIALIZE WS2812B strip object (REQUIRED)
  strip2.setBrightness(50);
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

  float dg = 0.2;
  float test_g = 0.9;
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) { 
    if(total > test_g) {
      if(pixel < 4) {
        WS2812B.setPixelColor(pixel, green);
        strip2.setPixelColor(pixel, green);
      }else if(pixel < 8) {
        WS2812B.setPixelColor(pixel, yellow);
        strip2.setPixelColor(pixel, yellow);
      } else {
        WS2812B.setPixelColor(pixel, red);
        strip2.setPixelColor(pixel, red);
      }
    } else {
      WS2812B.setPixelColor(pixel, black);
      strip2.setPixelColor(pixel, black);
    }
    test_g += dg;
  }
  /* Loop effect
  if(total > 2.0 && start_time==0) {
    start_time = millis();
  }
  
  int val = 100+total*50;
  const int total_t = 500;
  int t = millis() - start_time;
  int t_per_pixel = total_t / NUM_PIXELS;
  
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {           // for each pixel
    if(start_time != 0 && t < total_t) {
      if(pixel == t / t_per_pixel) {
        WS2812B.setPixelColor(pixel, WS2812B.Color(255, 0, 255));  // it only takes effect if pixels.show() is called
        strip2.setPixelColor(pixel, WS2812B.Color(0, 255, 255));  // it only takes effect if pixels.show() is called
      } else {
        WS2812B.setPixelColor(pixel, WS2812B.Color(255, 0, 0));  // it only takes effect if pixels.show() is called
        strip2.setPixelColor(pixel, WS2812B.Color(0, 255, 0));  // it only takes effect if pixels.show() is called
      }
    } else {
      WS2812B.setPixelColor(pixel, WS2812B.Color(255, 0, 0));  // it only takes effect if pixels.show() is called
      strip2.setPixelColor(pixel, WS2812B.Color(0, 255, 0));  // it only takes effect if pixels.show() is called
      start_time = 0;
    }
  }
  */
  
  WS2812B.setBrightness(128);
  WS2812B.show();  // send the updated pixel colors to the WS2812B hardware.

  strip2.setBrightness(128);
  strip2.show();  // send the updated pixel colors to the WS2812B hardware.
}
