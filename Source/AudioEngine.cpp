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
    parameters.addParameterListener("mono", this);
    
    auto mono = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("mono"))->get();
    
    if (mono) {
        addVoice(new Voice(parameters));
    } else {
        for (auto i = 0; i < maxNumVoices; ++i)
            addVoice(new Voice(parameters));
    }
    
    setVoiceStealingEnabled(true);
}

AudioEngine::~AudioEngine()
{
    parameters.removeParameterListener("mono", this);
}

void AudioEngine::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    this->spec = spec;
    setCurrentPlaybackSampleRate(spec.sampleRate);
    
    for (auto* v : voices)
        dynamic_cast<Voice*> (v)->prepare (spec);
}

void AudioEngine::parameterChanged (const juce::String& parameterID, float newValue)
{
    auto mono = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("mono"))->get();
    
    if (mono) {
        reduceNumVoices(1);
    } else {
        for (auto i = 0; i < maxNumVoices - 1; ++i) {
            auto *v = new Voice(parameters);
            v->prepare(spec);
            addVoice(v);
        }
    }
}

void AudioEngine::renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples)
{
    MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);
}
