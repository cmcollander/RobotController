#define W1D 7
#define W2D 8
#define W3D 9
#define W4D 10

#define W1D_MULT 0
#define W2D_MULT 1
#define W3D_MULT 1
#define W4D_MULT 1

#define W1P 3
#define W2P 4
#define W3P 5
#define W4P 6

char in1,in2,in3;

int j1x = 0;
int j1y = 0;
int j1b = 0;

int j2x = 0;
int j2y = 0;
int j2b = 0;

int b1 = 0;
int b2 = 0;
int b3 = 0;
int b4 = 0;
int b5 = 0;
int b6 = 0;

int sw1 = 0;
int sw2 = 0;

int knob1 = 0;
int knob2 = 0;

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
        j1x = map(in2,'a','z',0,100);
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
      case 'L': // Second Knob
        while(!Serial1.available()); // Wait for our next byte
        in2 = Serial1.read();
        if(in2<'a' || in2>'z') break; // If our value is outside of our range, break from switch
        knob2 = map(in2,'a','a',0,100);
        break;
      case 'B': // Our Buttons
        while(!Serial.available()); // Wait for our next byte
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
  }
  moveRover(map(j1y,0,100,-255,255),map(j2y,0,100,-255,255));
}
