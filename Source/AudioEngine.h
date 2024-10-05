/*
  ==============================================================================

    AudioEngine.h
    Created: 19 Sep 2024 8:47:13pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioEngine : public juce::MPESynthesiser, public juce::AudioProcessorValueTreeState::Listener
{
public:
    static constexpr auto maxNumVoices = 8;
    
    AudioEngine(juce::AudioProcessorValueTreeState &parameters);
    ~AudioEngine();
    
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    
private:
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    void renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override;
    
    juce::AudioProcessorValueTreeState &parameters;
    
    juce::dsp::ProcessSpec spec;
};
