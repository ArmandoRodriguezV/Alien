#pragma once
#include <JuceHeader.h>

class StyleV1: public juce::LookAndFeel_V4 {
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    void drawLinearSlider(juce::Graphics &, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider &) override;
};