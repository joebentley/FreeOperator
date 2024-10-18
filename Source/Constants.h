/*
  ==============================================================================

    Constants.h
    Created: 10 Oct 2024 7:13:14pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

static constexpr int MAX_SEQUENCE_LENGTH = 16;
static constexpr int WAVETABLE_SIZE = 256;

enum class Waveform {
    Sine,
    Saw,
    Noise
};

constexpr std::initializer_list<const char*> WaveformStrings { "Sine", "Saw", "Noise" };

inline Waveform waveformFromString(const juce::String &string)
{
    if (string == "Sine")
        return Waveform::Sine;
    if (string == "Saw")
        return Waveform::Saw;
    if (string == "Noise")
        return Waveform::Noise;
    
    throw;
}
