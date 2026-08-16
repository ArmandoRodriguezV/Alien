#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AlienEditor/AlienEditor.h"
#include "UI/VolumeEditor/VolumeEditor.h"

class AlienAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    AlienAudioProcessorEditor (AlienAudioProcessor&);
    ~AlienAudioProcessorEditor() override;

    void timerCallback() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AlienAudioProcessor& audioProcessor;

    std::unique_ptr<juce::Drawable> logo;

    AlienEditor mainEditor;
    VolumeEditor volumeEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlienAudioProcessorEditor)
};
