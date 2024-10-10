/*
  ==============================================================================

    SequencerComponent.h
    Created: 10 Oct 2024 3:34:12pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/

class LEDComponent  : public juce::Component
{
public:
    LEDComponent();
    ~LEDComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setIsOn(bool isOn) { this->isOn = isOn; }
    void setIsRecording(bool isRecording) { this->isRecording = isRecording; }
    
private:
    bool isOn = false;
    bool isRecording = true;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LEDComponent)
};


class SequencerComponent  : public juce::Component, public juce::ValueTree::Listener,
public juce::AudioProcessorValueTreeState::Listener, public juce::AsyncUpdater
{
public:
    SequencerComponent(juce::AudioProcessorValueTreeState &parameters);
    ~SequencerComponent() override;
    
    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged,
                                   const juce::Identifier& property) override;
    
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    void resized() override;

private:
    void handleAsyncUpdate() override;
    
    LEDComponent leds[8];
    
    juce::AudioProcessorValueTreeState &parameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerComponent)
};
