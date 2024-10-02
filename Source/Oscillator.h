/*
  ==============================================================================

    Oscillator.h
    Created: 22 Sep 2024 10:36:24pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Oscillator
{
public:
    Oscillator();
    
    void reset() { filter.reset(); }
    void setSampleRate(float sampleRate) { this->sampleRate = sampleRate; }
    void setFrequency(float frequency) { this->frequency = frequency; }
    void setPhaseOffset(float phaseOffset);
    void setAmplitude(float amplitude) { this->amplitude = amplitude; }
    void setAmplitudeOffset(float offset) { this->amplitudeOffset = offset; }
    void setFilterCoefficients(const juce::IIRCoefficients &newCoefficients) { filter.setCoefficients(newCoefficients); }
    
    float getFrequency() { return frequency; }
    
    float processSample();
    
private:
    juce::Array<float> lookupTable;
    float phase = 0.0;
    float phaseOffset = 0.0;
    float frequency = 440.0;
    float sampleRate = 0.0;
    float amplitude = 1.0;
    float amplitudeOffset = 0.0;
    
    juce::IIRFilter filter;
    
    JUCE_DECLARE_NON_COPYABLE(Oscillator)
};
