#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AlienAudioProcessor::AlienAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", parametersLayout())
#endif
{
}

AlienAudioProcessor::~AlienAudioProcessor()
{
}

//==============================================================================
const juce::String AlienAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AlienAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AlienAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AlienAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AlienAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AlienAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AlienAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AlienAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AlienAudioProcessor::getProgramName (int index)
{
    return {};
}

void AlienAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AlienAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    alienProcessor.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

void AlienAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AlienAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AlienAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        volumeProcessor[channel].process(channelData, channelData, buffer.getNumSamples());
        volumeProcessor[channel].setGain(*apvts.getRawParameterValue("VOLUME_ID"));

        lv = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
        if (buffer.getNumChannels() > 1)
            rv = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
    }

    alienProcessor.process(buffer);
    alienProcessor.setFrequency(*apvts.getRawParameterValue("FREQUENCY_ID"));
    alienProcessor.setStages(*apvts.getRawParameterValue("EXTENSION_ID"));
    alienProcessor.setSpread(*apvts.getRawParameterValue("SPREAD_ID"));
    alienProcessor.setFlux(*apvts.getRawParameterValue("FLUX_ID"));
    alienProcessor.setQ(*apvts.getRawParameterValue("PINCH_ID"));
}

juce::AudioProcessorValueTreeState::ParameterLayout AlienAudioProcessor::parametersLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "VOLUME_ID",
        "Volume",
        -60.0f,
        12.0f,
        0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "FREQUENCY_ID",
        "Frequency",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 0.001f),
        800.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "SPREAD_ID",
        "Spread",
        0.0f,
        4.0f,
        1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "PINCH_ID",
        "Pinch",
        0.1f,
        10.0f,
        0.1f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "FLUX_ID",
        "Flux",
        0.001f,
        1.0f,
        0.001f));

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "EXTENSION_ID",
        "Extension",
        0,
        128,
        0));

    return { params.begin(), params.end() };
}

//==============================================================================
bool AlienAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AlienAudioProcessor::createEditor()
{
    return new AlienAudioProcessorEditor (*this);
}

//==============================================================================
void AlienAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();

    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    copyXmlToBinary(*xml, destData);
}

void AlienAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr)
    {
        auto state = juce::ValueTree::fromXml(*xml);

        if (state.isValid())
            apvts.replaceState(state);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AlienAudioProcessor();
}
