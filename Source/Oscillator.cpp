/*
  ==============================================================================

    Oscillator.cpp
    Created: 22 Sep 2024 10:36:24pm
    Author:  Joe Bentley

  ==============================================================================
*/

#include "Oscillator.h"

Oscillator::Oscillator()
{
    lookupTable.clear();
    for (int i = 0; i < WAVETABLE_SIZE; ++i) {
        float phase = (float)i / (float)WAVETABLE_SIZE * juce::MathConstants<float>::twoPi;
        lookupTable.add(std::sin(phase));
    }
}

void Oscillator::setPhaseOffset(float phaseOffset)
{
    if (!overdrivePhase)
        phaseOffset *= 10.0;
    
    // simple lpf
    this->phaseOffset = filter.processSingleSampleRaw(phaseOffset);
}

float Oscillator::processSample()
{
    if (overdrivePhase)
        phase += (frequency + fineOffset) * (1.f / sampleRate) + phaseOffset;
    else
        phase += (frequency + fineOffset) * (1.f / sampleRate);
    
    if (phase < 0.0)
        phase += 1.0;
    if (phase > 1.0)
        phase -= 1.0;
    
    auto tempPhase = phase + phaseOffset;
        
    while (tempPhase < 0.0)
        tempPhase += 1.0;
    while (tempPhase > 1.0)
        tempPhase -= 1.0;
    
    int lowerIndex = std::floor(tempPhase * (float)WAVETABLE_SIZE);
    int upperIndex = lowerIndex + 1;
    if (upperIndex == WAVETABLE_SIZE)
        upperIndex = 0;
    
    auto progressThrough = tempPhase * (float)WAVETABLE_SIZE - std::floor(tempPhase * (float)WAVETABLE_SIZE);
    auto sample = lookupTable[lowerIndex] + progressThrough * (lookupTable[upperIndex] - lookupTable[lowerIndex]);
    
//    auto sample = std::sin(6.28318530718f * (phase + phaseOffset));
    
    return juce::jmin(1.0f, amplitudeOffset + amplitude) * sample;
}

