/*
  ==============================================================================

    Voice.h
    Created: 19 Sep 2024 9:15:20pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"

class Voice : public juce::MPESynthesiserVoice, public juce::AudioProcessorValueTreeState::Listener
{
public:
    Voice(juce::AudioProcessorValueTreeState &parameters);
    void prepare(const juce::dsp::ProcessSpec& spec);
    void noteStarted() override;
    void notePitchbendChanged() override {}
    void noteStopped(bool) override;
    void notePressureChanged() override {}
    void noteTimbreChanged()   override {}
    void noteKeyStateChanged() override {}
    
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
private:
    const float masterGain = juce::Decibels::decibelsToGain(-15.0);
    
    juce::AudioBuffer<float> tempBuffer;
    
    enum {
        oscIndex,
        masterGainIndex
    };
    
//    juce::dsp::ProcessorChain<CustomOscillator<float>, juce::dsp::Gain<float>> processorChain;
//    juce::dsp::Oscillator<float> carrier;
//    juce::dsp::Oscillator<float> modulator;
    Oscillator carrier;
    Oscillator modulator;
    float carrierFrequency;
    juce::RangedAudioParameter *fmDepth;
    
    juce::RangedAudioParameter *coarseDetuningOsc1;
    juce::RangedAudioParameter *coarseDetuningOsc2;
    
    juce::ADSR adsrOsc1;
    juce::ADSR adsrOsc2;
    juce::ADSR::Parameters adsrParameters1;
    juce::ADSR::Parameters adsrParameters2;
    
    juce::AudioProcessorValueTreeState &parameters;
};
