#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_midi.h>
#include <tables/sin8192_int8.h>

//---analog pins-------------
#define aRatePin 0
#define bRatePin 1
#define bDepthPin 2
#define cRatePin 3
#define cDepthPin 4
#define balancePin 5

#define CONTROL_RATE 128
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> operatorA(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> operatorB(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> operatorC(SIN8192_DATA);

int aRate;
int bRate;
int bDepth;
int cRate;
int cDepth;
int balanceA;
int balanceB;

void setup() {
//  Serial.begin(9600);
  startMozzi(CONTROL_RATE);
}

void loop() {
  audioHook();
}

void updateControl() {
  int aRateValue = mozziAnalogRead(aRatePin);
  int bRateValue = mozziAnalogRead(bRatePin);
  int bDepthValue = mozziAnalogRead(bDepthPin);
  int cRateValue = mozziAnalogRead(cRatePin);
  int cDepthValue = mozziAnalogRead(cDepthPin);
  int balanceValue = mozziAnalogRead(balancePin);

  aRate = mtof(26 + (aRateValue >> 4));
  bRate = bRateValue >> 2;
  bDepth = bDepthValue >> 3;
  cRate = cRateValue >> 2;
  cDepth = cDepthValue >> 4;
  balanceA = balanceValue >> 8;
  balanceB = 3 - balanceA;
  //  Serial.println(balanceA);

  operatorA.setFreq(aRate);
  operatorB.setFreq(bRate);
  operatorC.setFreq(cRate);
}

int updateAudio() {
  operatorB.setFreq (bRate + (operatorC.next() * cDepth * balanceB));
  operatorA.setFreq(aRate + (operatorB.next() * bDepth) + (operatorC.next() * cDepth * balanceA) >> 1);
  long output = (long)
                operatorA.next();
  output <<= 1;
  return (int) output;
}
