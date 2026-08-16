#include "StyleV1.h"

void StyleV1::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&)
{
    auto radius = std::min(width, height) * 0.5f - 4.0f;
    auto centreX = x + width * 0.5f;
    auto centreY = y + height * 0.5f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Borde
    juce::Path border;
    border.addArc(15, 15, width - 30, height - 30, rotaryStartAngle ,rotaryEndAngle, true);
    g.setColour(juce::Colour(128, 128, 128));
    g.strokePath(border, juce::PathStrokeType(2.0f));

    border.addArc(20, 20, width - 40, height - 40, rotaryStartAngle ,rotaryEndAngle, true);
    g.setColour(juce::Colour(32, 32, 32));
    g.strokePath(border, juce::PathStrokeType(1.0f));

    // Points
    const int numPoints = (width / 3);
    const float pointRadius = 1.0f;
    float pointOrbit = radius - 1.0f;

    for (int i = 0; i <= numPoints; ++i)
    {
        float t = static_cast<float>(i) / (numPoints - 1);
        float pointAngle = juce::jmap(t, 0.0f, 1.0f, rotaryStartAngle, rotaryEndAngle);
        float px = centreX + std::cos(pointAngle - juce::MathConstants<float>::halfPi) * pointOrbit;
        float py = centreY + std::sin(pointAngle - juce::MathConstants<float>::halfPi) * pointOrbit;

        g.setColour(juce::Colour(120, 120, 120));
        g.fillEllipse(px - pointRadius, py - pointRadius, pointRadius * 2.0f, pointRadius * 2.0f);
    }

    // Thumb
    g.setColour(juce::Colour(179, 191, 84));
    float thumbRad = 5.0f;
    float thumbOrb = radius - thumbRad - 6.0f;
    float thumbX = centreX + std::cos(angle - juce::degreesToRadians(90.0f)) * thumbOrb;
    float thumbY = centreY + std::sin(angle - juce::degreesToRadians(90.0f)) * thumbOrb;
    g.fillEllipse(thumbX - thumbRad, thumbY - thumbRad, thumbRad * 2, thumbRad * 2);

    juce::Path borderThumb;
    float borderRadius = thumbRad * 1.75f;
    borderThumb.addEllipse(thumbX - borderRadius, thumbY - borderRadius, borderRadius * 2.0f, borderRadius * 2.0f);
    g.strokePath(borderThumb, juce::PathStrokeType(2.0f));

    // Track
    juce::Path track;
    track.addArc(15, 15, width - 30, height - 30, rotaryStartAngle, angle, true);
    g.strokePath(track, juce::PathStrokeType(2.5f));
}

void StyleV1::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider) {
    if (style == juce::Slider::LinearVertical)
    {
        constexpr float trackWidth = 2.0f;
        float centreX = x + width * 0.5f;

        // Track
        g.setColour(juce::Colour(100, 100, 100));
        g.drawLine(centreX, (float)y, centreX, (float)(y + height), trackWidth);

        // Track
        g.setColour(juce::Colour(179, 191, 84));
        g.drawLine(centreX, sliderPos, centreX, (y + height), trackWidth);

        // Thumb
        constexpr float thumbRadius = 5.0f;
        g.fillEllipse(centreX - thumbRadius, sliderPos - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);

        // thumb
        constexpr float borderRadius = 9.0f;
        g.drawEllipse(centreX - borderRadius, sliderPos - borderRadius, borderRadius * 2.0f, borderRadius * 2.0f, 2.0f);
    }
}















