#include "AlienStage.h"

AlienStage::AlienStage() = default;

void AlienStage::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    numChannels = spec.numChannels;

    filters.clear();
    filters.resize(numChannels);

    juce::dsp::ProcessSpec monoSpec = spec;
    monoSpec.numChannels = 1;

    for (auto& filter : filters)
    {
        filter.prepare(monoSpec);
        filter.reset();
    }

    updateCoefficients();
}

void AlienStage::setFrequency(float f) {
    frequency = juce::jlimit(20.0f, 20000.0f, f);
    updateCoefficients();
}

void AlienStage::setQ(float nq) {
    q = juce::jlimit(0.1f, 50.0f, nq);
    updateCoefficients();
}

void AlienStage::reset() {
    for (int i = 0; i < numChannels; i++) {
        filters[i].reset();
    }
}

void AlienStage::process(juce::AudioBuffer<float>& buffer)
{
    auto block = juce::dsp::AudioBlock<float>(buffer);
    for (uint32_t ch = 0; ch < numChannels; ++ch)
    {
        auto channelBlock = block.getSingleChannelBlock(ch);
        juce::dsp::ProcessContextReplacing<float> context(channelBlock);
        filters[ch].process(context);
    }
}

void AlienStage::updateCoefficients()
{
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, frequency, q);

    for (auto& filter : filters)
        *filter.coefficients = *coeffs;
}