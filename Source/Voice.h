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
    ~Voice();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void noteStarted() override;
    void notePitchbendChanged() override {}
    void noteStopped(bool) override;
    void notePressureChanged() override {}
    void noteTimbreChanged()   override {}
    void noteKeyStateChanged() override {}
    
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
private:
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    float renderSampleForAlgorithm();
    
    const float masterGain = juce::Decibels::decibelsToGain(-10.0);
    
    juce::AudioBuffer<float> tempBuffer;
    
    float noteVelocity = 0.0;
    int algorithm = 0;
    
    Oscillator osc1;
    Oscillator osc2;
    Oscillator osc3;
    Oscillator osc4;
    
    juce::AudioParameterFloat *osc1Volume;
    juce::AudioParameterFloat *osc2Volume;
    juce::AudioParameterFloat *osc3Volume;
    juce::AudioParameterFloat *osc4Volume;
    juce::AudioParameterInt *osc1Coarse;
    juce::AudioParameterInt *osc2Coarse;
    juce::AudioParameterInt *osc3Coarse;
    juce::AudioParameterInt *osc4Coarse;
    juce::AudioParameterFloat *osc1Fine;
    juce::AudioParameterFloat *osc2Fine;
    juce::AudioParameterFloat *osc3Fine;
    juce::AudioParameterFloat *osc4Fine;
    juce::AudioParameterBool *osc1Fixed;
    juce::AudioParameterBool *osc2Fixed;
    juce::AudioParameterBool *osc3Fixed;
    juce::AudioParameterBool *osc4Fixed;
    
    juce::AudioParameterInt     *osc1CoarseRandom;
    juce::AudioParameterInt     *osc2CoarseRandom;
    juce::AudioParameterInt     *osc3CoarseRandom;
    juce::AudioParameterInt     *osc4CoarseRandom;
    juce::AudioParameterFloat   *osc1FineRandom;
    juce::AudioParameterFloat   *osc2FineRandom;
    juce::AudioParameterFloat   *osc3FineRandom;
    juce::AudioParameterFloat   *osc4FineRandom;
    juce::AudioParameterFloat   *osc1LevelRandom;
    juce::AudioParameterFloat   *osc2LevelRandom;
    juce::AudioParameterFloat   *osc3LevelRandom;
    juce::AudioParameterFloat   *osc4LevelRandom;
    
    juce::AudioParameterFloat   *timeRandom;
    
    juce::ADSR adsrOsc1;
    juce::ADSR adsrOsc2;
    juce::ADSR adsrOsc3;
    juce::ADSR adsrOsc4;
    juce::ADSR::Parameters adsrParameters1;
    juce::ADSR::Parameters adsrParameters2;
    juce::ADSR::Parameters adsrParameters3;
    juce::ADSR::Parameters adsrParameters4;
    
    juce::AudioProcessorValueTreeState &parameters;
    
    juce::Random random;
    
    double sampleRate;
};
