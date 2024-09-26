/*
  ==============================================================================

    AudioEngine.cpp
    Created: 19 Sep 2024 8:47:13pm
    Author:  Joe Bentley

  ==============================================================================
*/

#include "AudioEngine.h"
#include "Voice.h"

AudioEngine::AudioEngine(juce::AudioProcessorValueTreeState &parameters) : parameters(parameters)
{
    for (auto i = 0; i < maxNumVoices; ++i)
        addVoice(new Voice(parameters));
    
    setVoiceStealingEnabled(true);
}

void AudioEngine::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    setCurrentPlaybackSampleRate(spec.sampleRate);
    
    for (auto* v : voices)
        dynamic_cast<Voice*> (v)->prepare (spec);
}

void AudioEngine::renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples)
{
    MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);
    
//    auto block = juce::dsp::AudioBlock<float>(outputAudio);
//    auto blockToUse = block.getSubBlock((size_t) startSample, (size_t) numSamples);
//    auto contextToUse = juce::dsp::ProcessContextReplacing<float>(blockToUse);
//    fxChain.process(contextToUse);
}
