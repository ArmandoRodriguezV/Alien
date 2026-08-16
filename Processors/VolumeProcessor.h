#pragma once
#include <JuceHeader.h>

class VolumeProcessor {
public:
    void process(float* inAudio, float* outAudio, int numSamples);
    void setGain(float g);
private:
    float gain = 1.0f;
};