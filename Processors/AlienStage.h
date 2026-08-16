#pragma once
#include <JuceHeader.h>

class AlienStage {
public:
    AlienStage();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void setFrequency(float frequency);
    void setQ(float q);

    void reset();
    void process(juce::AudioBuffer<float>& buffer);
private:

    void updateCoefficients();

    std::vector<juce::dsp::IIR::Filter<float>> filters;

    double sampleRate = 44100.0;
    u_int32_t numChannels = 2;

    float frequency = 1000.0;
    float q = 1.0;
};