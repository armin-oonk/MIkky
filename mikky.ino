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

float total = 0.0;
int X_offset = 0, Y_offset = 0, Z_offset = 0; // Offset values

const int t_duration = 300;
const int dt = t_duration / NR_VERTICAL_LEDS;

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
unsigned wait_animation = 0;
unsigned long wait_animation_time = 0;
long wait_animation_delay = 100;


void loop() {
  total = get_total_g();

  // Loop effect
  if(total > 2.0 && start_time==0) {
    start_time = millis();
  }
  
  int t = millis() - start_time;

  // Reset alles naar rood
  for(int x=0; x<NR_VERTICAL_LEDS; x++) {
    //set_vertical_led(x, red);
    int flicker = (2*(x + wait_animation)) % NR_VERTICAL_LEDS;
    set_vertical_led(x, 255, 2 * flicker, 0);
  }

  if(millis() - wait_animation_time > wait_animation_delay) {
    wait_animation++;
    wait_animation_time = millis();
  }

  // Start animation
  if(start_time != 0 && t < t_duration) {
    int pixel = t / dt;
    set_vertical_led(pixel - 1, white);
    set_vertical_led(pixel, white);
    set_vertical_led(pixel + 1, white);
  } else {
    start_time = 0;
  }
  
  show_led_strips(128);
}
