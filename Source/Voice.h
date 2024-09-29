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
    
    juce::AudioParameterInt *osc1CoarseRandomMin;
    juce::AudioParameterInt *osc1CoarseRandomMax;
    juce::AudioParameterFloat *osc1LevelRandomMin;
    juce::AudioParameterFloat *osc1LevelRandomMax;
    juce::AudioParameterInt *osc2CoarseRandomMin;
    juce::AudioParameterInt *osc2CoarseRandomMax;
    juce::AudioParameterFloat *osc2LevelRandomMin;
    juce::AudioParameterFloat *osc2LevelRandomMax;
    juce::AudioParameterInt *osc3CoarseRandomMin;
    juce::AudioParameterInt *osc3CoarseRandomMax;
    juce::AudioParameterFloat *osc3LevelRandomMin;
    juce::AudioParameterFloat *osc3LevelRandomMax;
    juce::AudioParameterInt *osc4CoarseRandomMin;
    juce::AudioParameterInt *osc4CoarseRandomMax;
    juce::AudioParameterFloat *osc4LevelRandomMin;
    juce::AudioParameterFloat *osc4LevelRandomMax;
    
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
};
