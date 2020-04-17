#include "EspMQTTClient.h"

//define where your pins are
int latchPin = 0; //grys
int dataPin = 3; //swart
int clockPin = 2; //wit

byte switchVar1 = 0;
byte prevVar1 = 0;

unsigned long previousMillis = 0;
const long interval = 10000;

EspMQTTClient client("Buffalo4", "*********", "192.168.3.30", "", "", "buffalo4Alarm", 1883);

void setup()
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);
}

void onConnectionEstablished() {}

void loop()
{
  digitalWrite(latchPin, 1);
  delayMicroseconds(20);
  digitalWrite(latchPin, 0);
  switchVar1 = shiftIn(dataPin, clockPin);

  unsigned long currentMillis = millis();

  if ((currentMillis - previousMillis >= interval) || (switchVar1 != prevVar1)) {
    previousMillis = currentMillis;
    for (int i = 0, mask = 1; i < 8; i++, mask = mask << 1)
    {
      String state = (!(switchVar1 & mask)) ? "1" : "0";
      String topic = "buffalo4/alarm/zone_" + String(i);
      client.publish(topic, state);
    }
    prevVar1 = switchVar1;
  }
  client.loop();
}

byte shiftIn(int myDataPin, int myClockPin) {
  int i;
  int temp = 0;
  byte myDataIn = 0;
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(myDataPin);
    if (temp) {
      myDataIn = myDataIn | (1 << i);
    }
    digitalWrite(myClockPin, 1);
  }
  return myDataIn;
}
