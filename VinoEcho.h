#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "projects/Echo.h"
const int kNumPresets = 1;

enum EParams
{
  kDelay = 0,
  kDecay = 1,
  kDelay2 = 2,
  kDecay2 = 3,
  kSingleMode = 4,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class VinoEcho final : public Plugin
{
private:
  Echo echo = Echo(44100 * 5, 5000);
  Echo echo2 = Echo(44100 * 5, 5000);
public:
  VinoEcho(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
#endif
};
