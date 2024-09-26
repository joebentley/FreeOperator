/*
  ==============================================================================

    AudioEngine.h
    Created: 19 Sep 2024 8:47:13pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioEngine : public juce::MPESynthesiser
{
public:
    static constexpr auto maxNumVoices = 4;
    
    AudioEngine(juce::AudioProcessorValueTreeState &parameters);
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    
private:
    void renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override;
    
    juce::AudioProcessorValueTreeState &parameters;
};
