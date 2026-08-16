#pragma once
#include <JuceHeader.h>
#include "../../Assets/Font1.h"
#include "../Styles/StyleV1.h"

class AlienEditor  : public juce::Component {
public:
    AlienEditor(juce::AudioProcessorValueTreeState& state);
    ~AlienEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void updated();

private:
    void drawExtension(juce::Graphics&) const;
    void drawFrequency(juce::Graphics&) const;
    void drawPinch(juce::Graphics&) const;
    void drawSpread(juce::Graphics&) const;
    void drawFlux(juce::Graphics&) const;

    juce::Random random;

    StyleV1 style;

    juce::AudioProcessorValueTreeState& apvts;
    juce::Slider extension, freqSlider, spreadSlider, pinchSlider, fluxSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> extensionAttachment, freqAttachment, spreadAttachment, pinchAttachment, fluxAttachment;

    const int sliderXSize = 350;
    const int sliderSSize = 175;
    float time = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlienEditor)
};
