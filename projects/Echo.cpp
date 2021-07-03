#include "echo.h"
#include <IPlugConstants.h>
using iplug::sample;

Echo::Echo(int bufferSize, int delay) {
  this->bufferSize = bufferSize;
  this->delay = delay;
  filledOnce = false;
  wp = 0;
  decay = 0.5f;
}

void Echo::push(sample s) {
  sample ns = s * decay;
  if (!filledOnce) {
    samples.push_back(ns);
  }
  else {
    samples[wp] = ns;
  }
  wp++;
  if (wp >= bufferSize) {
    wp = 0;
    filledOnce = true;
  }
}

sample Echo::read() {
  if (wp < delay && !filledOnce || delay == 0) {
    return 0;
  }
  else {
    if (wp - delay >= 0) {
      return samples[wp - delay];
    }
    else {
      return samples[samples.size() - 1 + (wp - delay)];
    }
  }
}

void Echo::setDelay(int d) {
  delay = d;
}

int Echo::getDelay() {
  return delay;
}

void Echo::setDecay(float d) {
  decay = d;
}

float Echo::getDecay() {
  return decay;
}

int Echo::getBufferSizeInMs(int sampleRate) {
  return 1000 * (float)bufferSize / (float)sampleRate;
}