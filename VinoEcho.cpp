#include "VinoEcho.h"

#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

// Utility Function for placing a label under a given knob.
IRECT labelRect(IRECT knobRect) {
  return IRECT(knobRect.L - 12, knobRect.T + 55, knobRect.L + 64, knobRect.B + 20);
}

IRECT rect(int x, int y, int w, int h) {
  return IRECT(x, y, x + w, y + h);
}

VinoEcho::VinoEcho(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kDelay)->InitInt("Delay", 0, 0, 5000, "ms");
  GetParam(kDelay2)->InitInt("Delay 2", 0, 0, 5000, "ms");

  GetParam(kDecay)->InitInt("Decay", 0, 0, 100, "%");
  GetParam(kDecay2)->InitInt("Decay 2", 0, 0, 100, "%");

  GetParam(kSingleMode)->InitBool("Single Echo Mode", false);

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    // Load resources and setup background
    IBitmap knobImg = pGraphics->LoadBitmap(KNOBIMG_FN, 1);
    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachBackground(BACKIMG_FN);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    // Channel 1
    IBKnobRotaterControl* knobDelay = new IBKnobRotaterControl(150, 100, knobImg, kDelay);
    IBKnobRotaterControl* knobDecay = new IBKnobRotaterControl(100, 200, knobImg, kDecay);

    // Channel 2
    IBKnobRotaterControl* knobDelay2 = new IBKnobRotaterControl(200, 325, knobImg, kDelay2);
    IBKnobRotaterControl* knobDecay2 = new IBKnobRotaterControl(325, 350, knobImg, kDecay2);

    // Attach all knobs
    pGraphics->AttachControl(knobDelay);
    pGraphics->AttachControl(knobDecay);
    pGraphics->AttachControl(knobDelay2);
    pGraphics->AttachControl(knobDecay2);

    // Attach all parameter caption
    const IColor lblColor = IColor(100, 250, 250, 250);
    pGraphics->AttachControl(new ICaptionControl(labelRect(knobDelay->GetRECT()), kDelay, IText(18), lblColor, true));
    pGraphics->AttachControl(new ICaptionControl(labelRect(knobDecay->GetRECT()), kDecay, IText(18), lblColor, true));
    pGraphics->AttachControl(new ICaptionControl(labelRect(knobDelay2->GetRECT()), kDelay2, IText(18), lblColor, true));
    pGraphics->AttachControl(new ICaptionControl(labelRect(knobDecay2->GetRECT()), kDecay2, IText(18), lblColor, true));

    pGraphics->AttachControl(new IVToggleControl(IRECT(350, 15, 475, 65), kSingleMode, "", DEFAULT_STYLE, "Disabled", "Enabled"));

    // Attach all labels
    pGraphics->AttachControl(new IVLabelControl(rect(125, 80, 100, 20), "Delay"));
    pGraphics->AttachControl(new IVLabelControl(rect(75, 180, 100, 20), "Decay"));

    pGraphics->AttachControl(new IVLabelControl(rect(175, 305, 100, 20), "Delay 2"));
    pGraphics->AttachControl(new IVLabelControl(rect(300, 330, 100, 20), "Decay 2"));
  };
#endif
}



#if IPLUG_DSP
void VinoEcho::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int delayP = (float)GetParam(kDelay)->Value();
  const int convertedP = delayP > 0 ? GetSampleRate() * (delayP / 1000.0) : 0;
  const float decayP = (float)GetParam(kDecay)->Value() / 100.0;
  echo.setDelay(convertedP);
  echo.setDecay(decayP);

  const int delayP2 = (float)GetParam(kDelay2)->Value();
  const int convertedP2 = delayP2 > 0 ? GetSampleRate() * (delayP2 / 1000.0) : 0;
  const float decayP2 = (float)GetParam(kDecay2)->Value() / 100.0;
  echo2.setDelay(convertedP2);
  echo2.setDecay(decayP2);

  const bool singleMode = GetParam(kSingleMode)->Bool();

  const int nChans = NOutChansConnected();
  
  for (int s = 0; s < nFrames; s++) {
    echo.push(outputs[0][s]);
    sample samp = echo.read();

    echo2.push(outputs[1][s]);
    sample samp2 = echo2.read();

    outputs[0][s] = inputs[0][s] + samp;
    outputs[1][s] = inputs[1][s] + (singleMode ? samp : samp2);
    /*for (int c = 0; c < nChans; c++) {
      outputs[c][s] = inputs[c][s] + samp;
    }*/
  }
}
#endif
