#include "PluginProcessor.h"
#include "PluginEditor.h"

AlienAudioProcessorEditor::AlienAudioProcessorEditor (AlienAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    mainEditor(p.apvts),
    volumeEditor(p.apvts)
{
    auto svg = juce::XmlDocument::parse(BinaryData::AlienLogo_svg);

    if (svg != nullptr)
        logo = juce::Drawable::createFromSVG(*svg);

    startTimerHz(60);
    addAndMakeVisible(mainEditor);
    addAndMakeVisible(volumeEditor);
    setSize(1200, 750);
}

AlienAudioProcessorEditor::~AlienAudioProcessorEditor()
{
}

//==============================================================================
void AlienAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(6, 8, 13));

    if (logo != nullptr)
    {
        constexpr float margin = 16.0f;
        constexpr float width  = 140.0f;
        constexpr float height = 140.0f;

        logo->drawWithin(
            g,
            { margin, margin, width, height },
            juce::RectanglePlacement::centred,
            1.0f);
    }

    g.setColour(juce::Colour(179, 191, 84));
    g.setFont(Font1::get(36.0f));
    g.drawText("Alien", 120, 10, 400, 50, juce::Justification::left);

    g.setColour(juce::Colour(128, 128, 128));
    g.setFont(Font1::get(24.0f));
    g.drawText("Disperser", 120, 40, 400, 50, juce::Justification::left);

    g.setFont(Font1::get(18.0f));
    g.drawText("Power by ARVA", getWidth() - 420, 10, 400, 30, juce::Justification::right);
}

void AlienAudioProcessorEditor::resized()
{
    const int blockX = getWidth() / 6;
    const int blockY = getHeight() / 8;

    mainEditor.setBounds(0, blockY, blockX * 5, blockY * 7);
    volumeEditor.setBounds(blockX * 5, blockY, blockX, blockY * 7);
}

void AlienAudioProcessorEditor::timerCallback() {
    volumeEditor.repaint();
    volumeEditor.setVolumeParameter(audioProcessor.getLVolume(), audioProcessor.getRVolume());
    mainEditor.updated();
}
