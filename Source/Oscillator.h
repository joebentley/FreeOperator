/*
  ==============================================================================

    Oscillator.h
    Created: 22 Sep 2024 10:36:24pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Constants.h"

class Oscillator
{
public:
    Oscillator();
    
    void reset() { filter.reset(); }
    void setSampleRate(float sampleRate) { this->sampleRate = sampleRate; }
    // Use this one to set the noteon frequency
    void setFrequency(float frequency) { this->frequency = frequency; }
    // Use this one to set the fine offset while note is held down
    void setFineOffset(float fineOffset) { this->fineOffset = fineOffset; }
    void setSemitoneOffset(float semitoneOffset) { this->semitoneOffset = semitoneOffset; }
    // Same story
    void setAmplitude(float amplitude) { this->amplitude = amplitude; }
    void setAmplitudeOffset(float offset) { this->amplitudeOffset = offset; }
    // whether or not to use broken old phase accumulation
    void setOverdrivePhase(bool overdrivePhase) { this->overdrivePhase = overdrivePhase; }
    // FM modulation
    void setPhaseOffset(float phaseOffset);
    // FM modulation filter
    void setFilterCoefficients(const juce::IIRCoefficients &newCoefficients) { filter.setCoefficients(newCoefficients); }
    
    float getFrequency() { return frequency; }
    
    float processSample();
    
    void setWaveform(Waveform waveform);
    
private:
    juce::Array<float> lookupTable;
    double phase = 0.0;
    float phaseOffset = 0.0;
    float frequency = 440.0;
    float fineOffset = 0.0;
    float semitoneOffset = 0.0;
    float sampleRate = 0.0;
    float amplitude = 1.0;
    float amplitudeOffset = 0.0;
    bool overdrivePhase = false;
    
    juce::IIRFilter filter;
    
    JUCE_DECLARE_NON_COPYABLE(Oscillator)
};
