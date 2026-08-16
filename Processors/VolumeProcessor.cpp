#include "VolumeProcessor.h"

void VolumeProcessor::process(float *inAudio, float *outAudio, int numSamples) {
    for (int i = 0; i < numSamples; i++) {
        outAudio[i] = inAudio[i] * gain;
    }
}

void VolumeProcessor::setGain(float g) {
    gain = juce::Decibels::decibelsToGain(g, -60.0f);
}
