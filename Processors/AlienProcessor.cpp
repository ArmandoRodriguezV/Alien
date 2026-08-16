#include "AlienProcessor.h"

AlienProcessor::AlienProcessor() = default;
AlienProcessor::~AlienProcessor() = default;

void AlienProcessor::prepare(double sr, int samplesPerBlock, int numChannels)
{
    sampleRate = sr;

    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = sr;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(numChannels);

    stages.clear();
    stages.reserve(maxStages);

    frequency.reset(sampleRate, 0.005);
    q.reset(sampleRate, 0.005);
    spread.reset(sampleRate, 0.005);
    flux.reset(sampleRate, 0.005);

    frequency.setCurrentAndTargetValue(1000.0f);
    q.setCurrentAndTargetValue(1.0f);
    spread.setCurrentAndTargetValue(1.0f);
    flux.setCurrentAndTargetValue(1.0f);

    for (int i = 0; i < maxStages; ++i)
    {
        stages.emplace_back();
        stages.back().prepare(spec);
    }

    updateStages();
}

void AlienProcessor::reset()
{
    for (auto& stage : stages)
        stage.reset();
}

void AlienProcessor::setActiveStages(int count)
{
    activeStages = juce::jlimit(1, maxStages, count);
}

void AlienProcessor::setFrequency(float f)
{
    frequency.setTargetValue(f);
}

void AlienProcessor::setQ(float newQ)
{
    q.setTargetValue(newQ);
}

void AlienProcessor::setSpread(float s)
{
    spread.setTargetValue(s);
}

void AlienProcessor::setFlux(float f)
{
    flux.setTargetValue(f);
}

void AlienProcessor::setStages(int count)
{
    targetStages = juce::jlimit(1, maxStages, count);
}

void AlienProcessor::process(juce::AudioBuffer<float>& buffer)
{
    if (activeStages < targetStages)
    {
        stages[activeStages].reset();
        ++activeStages;
    }
    else if (activeStages > targetStages)
    {
        --activeStages;
    }


    updateStages();

    for (int i = 0; i < activeStages; ++i)
        stages[i].process(buffer);
}

void AlienProcessor::updateStages()
{
    const float currentFrequency = frequency.getNextValue();
    const float currentQ = q.getNextValue();
    const float currentSpread = spread.getNextValue();
    const float currentFlux = flux.getNextValue();

    for (int i = 0; i < activeStages; ++i)
    {
        const float t = static_cast<float>(i)
                      / static_cast<float>(juce::jmax(1, activeStages - 1));

        float x = t * 2.0f - 1.0f;

        x += currentFlux * std::sin(x * PI * 0.5f);

        const float freq =
            currentFrequency * std::pow(2.0f, x * currentSpread);

        const float stageQ =
            currentQ * (std::abs(x) * 4.0f);

        stages[i].setFrequency(freq);
        stages[i].setQ(stageQ);
    }
}