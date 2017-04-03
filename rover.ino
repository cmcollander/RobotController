#include <Adafruit_NeoPixel.h>

#define W1D 7
#define W2D 8
#define W3D 9
#define W4D 10

#define W1D_MULT 1
#define W2D_MULT 0
#define W3D_MULT 0
#define W4D_MULT 0

#define W1P 3
#define W2P 4
#define W3P 5
#define W4P 6

#define LED_PIN 23

#define LED_WHITE leds.Color(255,255,255)
#define LED_BLACK leds.Color(0,0,0)
#define LED_RED leds.Color(255,0,0)
#define LED_YELLOW leds.Color(255,255,0)
#define LED_BLUE leds.Color(0,0,255)
#define LED_GREEN leds.Color(0,255,0)

Adafruit_NeoPixel leds = Adafruit_NeoPixel(3, LED_PIN, NEO_GRB + NEO_KHZ800);


char in1,in2,in3;

int j1x = 0;
int j1y = 50;
int j1b = 0;

int j2x = 0;
int j2y = 50;
int j2b = 0;

int b1 = 1;
int b2 = 1;
int b3 = 1;
int b4 = 1;
int b5 = 1;
int b6 = 1;

int sw1 = 0;
int sw2 = 0;

int knob1 = 0;
int knob2 = 0;

int left_wheel = 0;
int right_wheel = 0;

void setWheel(int d, int p, int m,  int val) {
  if(val<0) {
    digitalWrite(d,1^m);
    analogWrite(p,val*-1);
  }
  else {
    digitalWrite(d,0^m);
    analogWrite(p,val);
  }
}

void joystick2motors(int joyx, int joyy) {
  int nJoyX = map(joyx,0,100,-128,128);
  int nJoyY = map(joyy,0,100,-128,128);
  double fPivYLimit = 32.0; // Threshold, (0..+127)
  int nMotMixL;
  int nMotMixR;
  
  // Temp variables
  double nMotPremixL;
  double nMotPremixR;
  int nPivSpeed;
  double fPivScale;
  
  // Calculate Drive Turn output due to Joystick X input
  if (nJoyY >= 0) {
    // Forward
    nMotPremixL = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
    nMotPremixR = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
  } else {
    nMotPremixL = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
    nMotPremixR = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
  }
  
  // Scale Drive output due to Joystick Y input (throttle)
  nMotPremixL = nMotPremixL * nJoyY/128.0;
  nMotPremixR = nMotPremixR * nJoyY/128.0;
  
  // Now calculate pivot amount
  nPivSpeed = nJoyX;
  fPivScale = (abs(nJoyY)>fPivYLimit)? 0.0 : (1.0 - abs(nJoyY)/fPivYLimit);
  
  // Calculate final mix of Drive and Pivot
  nMotMixL = (1.0-fPivScale)*nMotPremixL + fPivScale*(nPivSpeed);
  nMotMixR = (1.0-fPivScale)*nMotPremixR + fPivScale*(-nPivSpeed);
  
  left_wheel = map(nMotMixL,-128,128,-255,255);
  right_wheel = map(nMotMixR,-128,128,-255,255);
  
}

void moveRover(int left, int right) {
  if(left>-50 && left<50) // Any values close to zero will be set to zero, so tiny joystick action will result in no movement
    left = 0;
  if(right>-50 && right<50)
    right = 0;
  setWheel(W1D,W1P,W1D_MULT, left);
  setWheel(W2D,W2P,W2D_MULT, left);
  setWheel(W3D,W3P,W3D_MULT, right);
  setWheel(W4D,W4P,W4D_MULT, right);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<leds.numPixels(); i++) {
    leds.setPixelColor(i, c);
    leds.show();
    delay(wait);
  }
}

void setup() {
  pinMode(W1D, OUTPUT);
  pinMode(W2D, OUTPUT);
  pinMode(W3D, OUTPUT);
  pinMode(W4D, OUTPUT);
  pinMode(W1P, OUTPUT);
  pinMode(W2P, OUTPUT);
  pinMode(W3P, OUTPUT);
  pinMode(W4P, OUTPUT);

  Serial1.begin(9600);
  
  moveRover(0,0);
  
  leds.begin();
  leds.show(); // Initialize all LEDs to 'off'
  colorWipe(LED_BLACK,0);
}

void loop() {
  if(Serial1.available()) {
    in1 = Serial1.read(); // Pull in our command
    switch(in1) {
      case 'J': // First Joystick
        while(!Serial1.available()); // Wait for our next byte
        in2 = Serial1.read();
        while(!Serial1.available()); // Wait for our second byte
        in3 = Serial1.read();
        if(in2<'a' || in3<'a' || in2>'z' || in3>'z') break; // If either value is outside of our range, we grabbed a wrong value so break from the switch
        j1x = map(in2,'a','z',100,0);
        j1y = map(in3,'a','z',0,100);
        break;
      case 'K': // Second Joystick
        while(!Serial1.available()); // Wait for our next byte
        in2 = Serial1.read();
        while(!Serial1.available()); // Wait for our second byte
        in3 = Serial1.read();
        if(in2<'a' || in3<'a' || in2>'z' || in3>'z') break; // If either value is outside of our range, we grabbed a wrong value so break from the switch
        j2x = map(in2,'a','z',0,100);
        j2y = map(in3,'a','z',0,100);
        break;
      case 'L': // First Knob
        while(!Serial1.available()); // Wait for our next byte
        in2 = Serial1.read();
        if(in2<'a' || in2>'z') break; // If our value is outside of our range, break from switch
        knob1 = map(in2,'a','a',0,100);
        break;
      case 'M': // Second Knob
        while(!Serial1.available()); // Wait for our next byte
        in2 = Serial1.read();
        if(in2<'a' || in2>'z') break; // If our value is outside of our range, break from switch
        knob2 = map(in2,'a','a',0,100);
        break;

      case 'B': // Our Buttons
        while(!Serial1.available()); // Wait for our next byte
        in2 = Serial1.read(); // Note that since we use every possible integer for our buttons, there is no error checking capability here.
        b1 = in2&(1<<0); // We use bitmasking to pull each button value from the corresponding bit of the byte
        b2 = in2&(1<<1);
        b3 = in2&(1<<2);
        b4 = in2&(1<<3);
        b5 = in2&(1<<4);
        b6 = in2&(1<<5);
        j1b = in2&(1<<6);
        j2b = in2&(1<<7);
        break;
      case 'S': // Our Switches
        while(!Serial1.available()); // Wait for our next byte
        in2 = Serial1.read();
        sw1 = in2&(1<<0);
        sw2 = in2&(1<<1);
        break;
    }
    joystick2motors(j1x,j1y);
    moveRover(left_wheel,right_wheel);
  }
  

  // Change LED Color based on button pressed
  if(!b6)
    colorWipe(LED_WHITE,0);
  else if(!b5)
    colorWipe(LED_BLACK,0);
  else if(!b4)
    colorWipe(LED_RED,0);
  else if(!b3)
    colorWipe(LED_YELLOW,0);
  else if(!b2)
    colorWipe(LED_BLUE,0);
  else if(!b1)
    colorWipe(LED_GREEN,0);
}
