/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscComponent.h"

//==============================================================================
/**
*/
class FMsynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
public:
    FMsynthAudioProcessorEditor (FMsynthAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~FMsynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    static constexpr int rotaryBoxWidth = 80;
    static constexpr int rotaryBoxHeight = 20;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMsynthAudioProcessor& audioProcessor;
    juce::MidiKeyboardState midiKeyboardState;
    juce::MidiKeyboardComponent midiKeyboardComponent { midiKeyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };

    OscComponent osc1;
    OscComponent osc2;
    juce::Slider fmDepth;
    juce::Slider coarseOsc1;
    juce::Slider coarseOsc2;
    
    juce::AudioProcessorValueTreeState &parameters;
    
    std::unique_ptr<SliderAttachment> fmDepthAttachment;
    std::unique_ptr<SliderAttachment> coarseOsc1Attachment;
    std::unique_ptr<SliderAttachment> coarseOsc2Attachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMsynthAudioProcessorEditor)
};
