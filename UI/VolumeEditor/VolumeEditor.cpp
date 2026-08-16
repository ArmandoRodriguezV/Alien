#include "VolumeEditor.h"
#include "../../Assets/Font1.h"

VolumeEditor::VolumeEditor(juce::AudioProcessorValueTreeState& state)
    : apvts(state)
{
    addAndMakeVisible(volumeSlider);
    volumeAttachment = std::make_unique<SliderAttachment>(apvts, "VOLUME_ID", volumeSlider);
}

VolumeEditor::~VolumeEditor() {
    volumeSlider.setLookAndFeel(nullptr);
}

void VolumeEditor::paint(juce::Graphics& g)
{
    const int centerX = getWidth() / 2;
    drawVolParameter(g, lVol, getWidth() - 40);
    drawVolParameter(g, rVol, getWidth() - 80);

    g.setColour(juce::Colours::white);
    g.drawLine(0, 0, 0, getHeight());

    g.drawLine(centerX, 20, centerX, getHeight() - 20);

    for (int i = 0; i <= 6; i++) {
        const int dy = 20 + (i * (getHeight() - 40)) / 6;
        const int db = 12 - (i * 12);
        g.drawLine(centerX - 5, dy, centerX, dy);
        g.setFont(Font1::get(15.0f));
        g.drawText(juce::String(db) + "Db", centerX - 70, dy - 10, 60, 20, juce::Justification::right);
    }
}

void VolumeEditor::resized() {
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeSlider.setBounds(10, 20, 40, getHeight() - 40);
    volumeSlider.setLookAndFeel(&style);
}

void VolumeEditor::drawVolParameter(juce::Graphics& g, float value, int x)
{
    const int top = 20;
    const int bottom = getHeight() - 20;
    const int parameterSize = bottom - top;

    const int numBins = 100;
    const float binSize = parameterSize / (float)numBins;

    float db = juce::Decibels::gainToDecibels(value, -60.0f);

    db = juce::jlimit(-60.0f, 12.0f, db);

    int activeBins = juce::roundToInt(
        juce::jmap(db, -60.0f, 12.0f, 0.0f, (float)numBins)
    );

    for (int i = 0; i < numBins; ++i)
    {
        float y = bottom - (i + 1) * binSize;

        g.setColour(i < activeBins
            ? juce::Colour(240, 255, 115)
            : juce::Colours::grey);

        g.fillRect((float)x, y, 30.0f, binSize - 2.0f);
    }
}

void VolumeEditor::setVolumeParameter(float lv, float rv) {
    lVol = lv;
    rVol = rv;
}