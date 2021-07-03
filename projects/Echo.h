#pragma once
#include <vector>
#include <IPlugConstants.h>
using std::vector;
using iplug::sample;

class Echo {

public:
  Echo(int bufferSize, int delay);
  void push(sample s);
  sample read();
  void setDelay(int d);
  int getDelay();
  void setDecay(float d);
  float getDecay();

  int getBufferSizeInMs(int sampleRate);
private:
  vector<sample> samples;
  int wp;
  int delay;
  int bufferSize;
  float decay;
  bool filledOnce;
};