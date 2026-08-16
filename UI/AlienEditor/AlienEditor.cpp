#include <JuceHeader.h>
#include "AlienEditor.h"

AlienEditor::AlienEditor(juce::AudioProcessorValueTreeState& state)
    :apvts(state)
{
    addAndMakeVisible(extension);
    extensionAttachment = std::make_unique<SliderAttachment>(apvts, "EXTENSION_ID", extension);

    addAndMakeVisible(freqSlider);
    freqAttachment = std::make_unique<SliderAttachment>(apvts, "FREQUENCY_ID", freqSlider);

    addAndMakeVisible(spreadSlider);
    spreadAttachment = std::make_unique<SliderAttachment>(apvts, "SPREAD_ID", spreadSlider);

    addAndMakeVisible(pinchSlider);
    pinchAttachment = std::make_unique<SliderAttachment>(apvts, "PINCH_ID", pinchSlider);

    addAndMakeVisible(fluxSlider);
    fluxAttachment = std::make_unique<SliderAttachment>(apvts, "FLUX_ID", fluxSlider);
}

AlienEditor::~AlienEditor() {
    extension.setLookAndFeel(nullptr);
    freqSlider.setLookAndFeel(nullptr);
    spreadSlider.setLookAndFeel(nullptr);
    pinchSlider.setLookAndFeel(nullptr);
    fluxSlider.setLookAndFeel(nullptr);
}

void AlienEditor::paint (juce::Graphics& g) {
    const float centerX = getWidth() / 2.0f;
    const float centerY = getHeight() / 2.0f;

    g.setColour(juce::Colours::white);
    g.drawLine(30, 0, static_cast<float>(getWidth()) - 60, 0);

    g.setColour(juce::Colours::white.withAlpha(0.4f));
    juce::Path curves;

    float baseY = 115.0f;

    float targetX = centerX;
    float targetY = centerY;

    const int blockX = getWidth() / 6;
    const int blockY = getHeight() / 4;


    curves.startNewSubPath(centerX - 250.0f, baseY);
    curves.lineTo(centerX - 125.0f, baseY);

    curves.quadraticTo(
        centerX - 80.0f, baseY,
        targetX - 20.0f, targetY
    );

    curves.startNewSubPath(centerX + 250.0f, baseY);
    curves.lineTo(centerX + 125.0f, baseY);

    curves.quadraticTo(
        centerX + 80.0f, baseY,
        targetX + 20.0f, targetY
    );

    curves.startNewSubPath(centerX - 250.0f, getHeight() - 115.0f);
    curves.lineTo(centerX - 125.0f, getHeight() - 115.0f);

    curves.quadraticTo(
        centerX - 80.0f, getHeight() - 115.0f,
        targetX - 20.0f, targetY + 20.0f
    );

    curves.startNewSubPath(centerX + 250.0f, getHeight() - 115.0f);
    curves.lineTo(centerX + 125.0f, getHeight() - 115.0f);

    curves.quadraticTo(
        centerX + 80.0f, getHeight() - 115.0f,
        targetX + 20.0f, targetY + 20.0f
    );

    g.strokePath(curves, juce::PathStrokeType(1.4f));

    drawExtension(g);
    drawFrequency(g);
    drawPinch(g);
    drawSpread(g);
    drawFlux(g);

    g.setColour(juce::Colour(179, 191, 84));
    g.setFont(Font1::get(28.0f));
    g.drawText("Frequency", centerX - 250, 80, 300, 30, juce::Justification::left);
    g.drawText("Pinch", centerX + 100, 80, 300, 30, juce::Justification::left);
    g.drawText("Spread", centerX - 250, getHeight() - 110, 300, 30, juce::Justification::left);
    g.drawText("Flux", centerX + 100, getHeight() - 110, 300, 30, juce::Justification::left);

    g.setColour(juce::Colours::white.withAlpha(0.65f));
    g.setFont(Font1::get(24.0f));

    g.drawText(
        juce::String(static_cast<int>(freqSlider.getValue())) + " Hz",
        blockX - (sliderSSize / 2), blockY - (sliderSSize / 2) + sliderSSize,
        sliderSSize, 30, juce::Justification::centred);

    g.drawText(
        juce::String(static_cast<int>(spreadSlider.getValue())) + " Oct",
        blockX - (sliderSSize / 2), blockY * 3 - (sliderSSize / 2) + sliderSSize,
        sliderSSize, 30, juce::Justification::centred);

    g.drawText(
        juce::String(pinchSlider.getValue()),
        blockX * 5 - (sliderSSize / 2), blockY - (sliderSSize / 2) + sliderSSize,
        sliderSSize, 30, juce::Justification::centred);

    g.drawText(
        juce::String(fluxSlider.getValue()),
        blockX * 5 - (sliderSSize / 2), blockY * 3 - (sliderSSize / 2) + sliderSSize,
        sliderSSize, 30, juce::Justification::centred);
}

void AlienEditor::drawFlux(juce::Graphics& g) const
{
    const float fluxValue = juce::jmap((float)fluxSlider.getValue(), 0.001f, 1.0f, 0.5f, 12.0f);

    const int blockX = getWidth() / 6 * 5;
    const int blockY = getHeight() / 4 * 3;

    const float centerX = blockX;
    const float centerY = blockY;

    const float width  = sliderSSize * 0.60f;
    const float height = sliderSSize * 0.20f;

    juce::Path wave;

    const int resolution = 120;

    for (int i = 0; i <= resolution; ++i)
    {
        float t = static_cast<float>(i) / resolution;
        float x = centerX - width * 0.5f + t * width;
        float frequency = fluxValue;
        float warpedT = t + std::sin(t * juce::MathConstants<float>::twoPi * 0.5f) * (fluxValue * 0.02f);
        float sine = std::sin(warpedT * juce::MathConstants<float>::twoPi * frequency);
        float amplitude = height * (0.6f + fluxValue * 0.05f);
        float y = centerY + sine * amplitude;

        if (i == 0)
            wave.startNewSubPath(x, y);
        else
            wave.lineTo(x, y);
    }

    g.setColour(juce::Colour(179, 191, 84));
    g.strokePath(wave, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved));
}
void AlienEditor::drawSpread(juce::Graphics& g) const
{
    // spread normalizado (0–1) pero con rango efectivo 0–4
    const float sNorm = spreadSlider.getValue();
    const float s = juce::jmap(sNorm, 0.0f, 1.0f, 0.2f, 4.0f);

    const int blockX = getWidth() / 6;
    const int blockY = getHeight() / 4 * 3;

    const float centerX = blockX;
    const float centerY = blockY;

    const float width  = sliderSSize * 0.65f;
    const float height = sliderSSize * 0.35f;

    constexpr int resolution = 120;

    const float epsilon = 0.001f;

    juce::Path leftCurve;
    juce::Path rightCurve;

    bool startedLeft = false;
    bool startedRight = false;

    for (int i = 0; i <= resolution; ++i)
    {
        float t = (float)i / resolution;

        float x = juce::jmap(t, -1.0f, 1.0f);

        float denom = (x * s);

        if (std::abs(denom) < epsilon)
            denom = epsilon;

        float y = 1.0f / denom;
        y = juce::jlimit(-2.0f, 2.0f, y);

        float px = centerX + x * width * 0.5f;
        float py = centerY - y * height * 0.15f;

        float pyMirror = centerY + (centerY - py);

        // LEFT
        if (!startedLeft)
        {
            leftCurve.startNewSubPath(px, py);
            startedLeft = true;
        }
        else
        {
            leftCurve.lineTo(px, py);
        }

        // RIGHT
        if (!startedRight)
        {
            rightCurve.startNewSubPath(px, pyMirror);
            startedRight = true;
        }
        else
        {
            rightCurve.lineTo(px, pyMirror);
        }
    }

    g.setColour(juce::Colour(179, 191, 84));

    g.strokePath(leftCurve, juce::PathStrokeType(2.0f));
    g.strokePath(rightCurve, juce::PathStrokeType(2.0f));
}
void AlienEditor::drawPinch(juce::Graphics& g) const
{
    const float norm = pinchSlider.getValue() / 100.0f;
    const float s = juce::jmap(norm, 1.0f, 100.0f);

    const int blockX = getWidth() / 6 * 5;
    const int blockY = getHeight() / 4;

    const float centerX = blockX;
    const float centerY = blockY;

    const float width  = sliderSSize * 0.60f;
    const float height = sliderSSize * 0.25f;

    juce::Path curve;
    curve.clear();

    constexpr int resolution = 120;

    bool started = false;

    for (int i = 0; i <= resolution; ++i)
    {
        const float t = (float)i / resolution;
        const float x = juce::jmap(t, -1.0f, 1.0f);
        const float y = std::pow(2.0f, -s * x * x);

        const float px = centerX - width * 0.5f + t * width;
        const float py = centerY + height * 0.5f - y * height;

        if (!started)
        {
            curve.startNewSubPath(px, py);
            started = true;
        }
        else
        {
            curve.lineTo(px, py);
        }
    }

    g.setColour(juce::Colour(179, 191, 84));
    g.strokePath(curve, juce::PathStrokeType(2.0f));
}
void AlienEditor::drawFrequency(juce::Graphics& g) const
{
    const float normalFreq = freqSlider.getValue() / 20000.0f;

    const int blockX = getWidth() / 6;
    const int blockY = getHeight() / 4;
    const float centerX = blockX;
    const float centerY = blockY;

    const float width  = sliderSSize * 0.60f;
    const float height = sliderSSize * 0.20f;

    const float cycles = juce::jmap(normalFreq, 0.0f, 1.0f, 1.0f, 4.0f);

    juce::Path wave;

    const int resolution = 60;
    for (int i = 0; i <= resolution; ++i)
    {
        float t = (float)i / resolution;
        float x = centerX - width * 0.5f + t * width;
        float y = centerY + std::sin(t * juce::MathConstants<float>::twoPi * cycles) * height;

        if (i == 0)
            wave.startNewSubPath(x, y);
        else
            wave.lineTo(x, y);
    }

    g.setColour(juce::Colour(179, 191, 84));
    g.strokePath(wave, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved));
}
void AlienEditor::drawExtension(juce::Graphics& g) const {
    const int centerX = getWidth() / 2;
    const int centerY = getHeight() / 2;

    const float extensionV = extension.getValue() / 128.0f;
    const int layers = 1 + static_cast<int>(extensionV * 15.0f);

    const int resolution = juce::jmap(static_cast<int>(extension.getValue()), 1, 128, 16, 128);

    const float baseRadius = 40.0f;
    const float spacing = 5.5f;
    const float maxRadius = baseRadius + 10.0f * spacing;

    for (int l = 0; l < layers; ++l)
    {
        juce::Path shape;
        const float radius = maxRadius - l * spacing;
        const float sharpness = juce::jmap(extensionV, 0.0f, 1.0f, 0.05f, 0.25f);
        const float phase = l * 0.15f;
        const float breathe = 1.0f + 0.03f * std::sin(time * 1.6f + phase);

        for (int i = 0; i <= resolution; ++i)
        {
            const float a = juce::MathConstants<float>::twoPi * i / resolution;

            float r = radius;

            r += radius * sharpness * std::sin(a * 3.0f);
            r += radius * sharpness * 0.5f * std::sin(a * 5.0f + 0.8f);
            r += radius * sharpness * 0.25f * std::sin(a * 9.0f + 1.7f);

            float turbulence =
                    (std::sin(time * 2.2f + phase + a * 4.0f))
                + 0.50f * (std::sin(time * 3.8f + phase + a * 7.0f))
                + 0.25f * (std::sin(time * 6.1f + phase + a * 13.0f));

            turbulence /= 1.75f ;
            r *= breathe;
            r += turbulence * radius * (0.04f + extensionV * 0.04f);

            const float x = centerX + std::cos(a) * r;
            const float y = centerY + std::sin(a) * r;

            if (i == 0)
                shape.startNewSubPath(x, y);
            else
                shape.lineTo(x, y);
        }

        shape.closeSubPath();
        g.setColour(juce::Colour(179, 191, 84).withAlpha(1.0f - (0.15f + 0.05f * l)));
        g.strokePath(shape, juce::PathStrokeType(2.5f));
    }

    g.setColour(juce::Colours::white);
    g.setFont(Font1::get(32.0f));
    g.drawText(juce::String(static_cast<int>(extensionV * 100)) + "%", centerX - 40, centerY + 120, 80, 40, juce::Justification::centred);
    g.setColour(juce::Colour(179, 191, 84));
    g.setFont(Font1::get(24.0f));
    g.drawText(juce::String("Disperser"), 0, centerY + 150, getWidth(), 40, juce::Justification::centred);
}
void AlienEditor::updated() {
    repaint();
    time += 0.01f * juce::jmap((float) extension.getValue(), 1.0f, 96.0f, 1.0f, 3.0f);
}

void AlienEditor::resized()
{
    const int centerX = getWidth() / 2;
    const int centerY = getHeight() / 2;
    const int blockX = getWidth() / 6;
    const int blockY = getHeight() / 4;

    extension.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    extension.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    extension.setBounds(centerX - (sliderXSize / 2), centerY - (sliderXSize / 2), sliderXSize, sliderXSize);
    extension.setLookAndFeel(&style);

    freqSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    freqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    freqSlider.setBounds(blockX - (sliderSSize / 2), blockY - (sliderSSize / 2), sliderSSize, sliderSSize);
    freqSlider.setLookAndFeel(&style);

    spreadSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    spreadSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    spreadSlider.setBounds(blockX - (sliderSSize / 2), blockY * 3 - (sliderSSize / 2), sliderSSize, sliderSSize);
    spreadSlider.setLookAndFeel(&style);

    pinchSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pinchSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    pinchSlider.setBounds(blockX * 5 - (sliderSSize / 2), blockY - (sliderSSize / 2), sliderSSize, sliderSSize);
    pinchSlider.setLookAndFeel(&style);

    fluxSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    fluxSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    fluxSlider.setBounds(blockX * 5 - (sliderSSize / 2), blockY * 3 - (sliderSSize / 2), sliderSSize, sliderSSize);
    fluxSlider.setLookAndFeel(&style);
}
