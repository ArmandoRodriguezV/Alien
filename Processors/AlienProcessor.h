#pragma once
#include <JuceHeader.h>
#include "AlienStage.h"

class AlienProcessor
{
public:
    AlienProcessor();
    ~AlienProcessor();

    void prepare(double sampleRate, int samplesPerBlock, int numChannel);
    void reset();
    void setActiveStages(int count);

    void setFrequency(float frequency);
    void setQ(float nq);
    void setSpread(float s);
    void setFlux(float f);
    void setStages(int e);

    void process(juce::AudioBuffer<float>& buffer);

private:
    void updateStages();

    std::vector<AlienStage> stages;
    double sampleRate = 44100.0;
    const float PI = juce::MathConstants<float>::pi;

    int maxStages = 128;
    int activeStages = 1;
    int targetStages = 1;

    juce::SmoothedValue<float> frequency;
    juce::SmoothedValue<float> q;
    juce::SmoothedValue<float> spread;
    juce::SmoothedValue<float> flux;
};