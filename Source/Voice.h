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
    void setSequenceAtCurrentIndex(const juce::String& parameterID, float valueToPush);
    float getCurrentFromSequence(const juce::String& parameterID);
    void incrementSequenceIndex();
    
    const float masterGain = juce::Decibels::decibelsToGain(-10.0);
    
    juce::AudioBuffer<float> tempBuffer;
    
    float noteVelocity = 0.0;
    int algorithm = 0;
    
    Oscillator osc[4];
    
    juce::AudioParameterFloat   *oscVolume[4];
    juce::AudioParameterInt     *oscCoarse[4];
    juce::AudioParameterFloat   *oscFine[4];
    juce::AudioParameterBool    *oscFixed[4];
    
    juce::AudioParameterInt     *oscCoarseRandom[4];
    juce::AudioParameterFloat   *oscFineRandom[4];
    juce::AudioParameterFloat   *oscLevelRandom[4];
    
    juce::AudioParameterFloat   *timeRandom;
    juce::AudioParameterBool    *overdrivePhase;
    juce::AudioParameterInt     *sequenceLength;
    
    juce::ADSR adsrOsc[4];
    juce::ADSR::Parameters adsrParameters[4];
    
    juce::AudioProcessorValueTreeState &parameters;
    
    juce::Random random;
    
    double sampleRate = 0.0;
};
