#pragma once
#include <JuceHeader.h>
#include "../Styles/StyleV1.h"

class VolumeEditor: public juce::Component
{
public:
    VolumeEditor(juce::AudioProcessorValueTreeState& state);
    ~VolumeEditor();

    void paint(juce::Graphics &g) override;
    void resized() override;

    void setVolumeParameter(float lv, float yv);

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::Slider volumeSlider;

    StyleV1 style;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> volumeAttachment;

    void drawVolParameter(juce::Graphics& g, float value, int x);
    float lVol = 0.0f;
    float rVol = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VolumeEditor)
};
