#pragma once

#include <JuceHeader.h>

class Font1
{
public:
    static juce::Font get(float height)
    {
        static juce::Typeface::Ptr typeface =
            juce::Typeface::createSystemTypefaceFor(
                BinaryData::BrunoAceSCRegular_ttf,
                BinaryData::BrunoAceSCRegular_ttfSize);

        return juce::Font(
            juce::FontOptions()
                .withTypeface(typeface)
                .withHeight(height));
    }
private:
    Font1() = delete;
};