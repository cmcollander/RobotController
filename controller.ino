#define J1X A0
#define J1Y A1
#define J1B 2
#define J2X A2
#define J2Y A3
#define J2B 3
#define SW1 11
#define SW2 12
#define BUT1 4
#define BUT2 5
#define BUT3 6
#define BUT4 7
#define BUT5 8
#define BUT6 9
#define KNOB1 A6
#define KNOB2 A7
#define XBEE Serial1

int i;

char msg[] = {'J','x','y','K','x','y','L','a','M','a','B','0','S','0'};

void setup() {
  pinMode(J1B, INPUT_PULLUP);
  pinMode(J2B, INPUT_PULLUP);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);
  pinMode(BUT4, INPUT_PULLUP);
  pinMode(BUT5, INPUT_PULLUP);
  pinMode(BUT6, INPUT_PULLUP);

  XBEE.begin(9600);
}

void loop() {
  // msg[0] = 'J';
  msg[1] = map(analogRead(J1X),0,1027,'z','a');
  msg[2] = map(analogRead(J1Y),0,1027,'z','a');
  // msg[3] = 'K';
  msg[4] = map(analogRead(J2X),0,1027,'z','a');
  msg[5] = map(analogRead(J2Y),0,1027,'z','a');
  // msg[6] = 'L';
  msg[7] = map(analogRead(KNOB1),0,1027,'z','a');
  // msg[8] = 'M';
  msg[9] = map(analogRead(KNOB2),0,1027,'z','a');
  // msg[10] = 'B';
  msg[11] = (digitalRead(BUT1)<<0) | (digitalRead(BUT2)<<1) | (digitalRead(BUT3)<<2) | (digitalRead(BUT4)<<3) | (digitalRead(BUT5)<<4) | (digitalRead(BUT6)<<5) | (digitalRead(J1B)<<6) | (digitalRead(J2B)<<7);
  // msg[12] = 'S';
  msg[13] = (digitalRead(SW1)<<0) | (digitalRead(SW2)<<1);
  XBEE.println(msg);
  delay(150);
}
