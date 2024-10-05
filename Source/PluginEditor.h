/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscComponent.h"
#include "ModulatorComponent.h"

//==============================================================================
/**
*/
class FMsynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
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

    juce::TabbedComponent tabs { juce::TabbedButtonBar::Orientation::TabsAtTop };
    OscComponents oscs;
    ModulatorTab modTab;
    
    juce::Slider algorithm;
    std::unique_ptr<SliderAttachment> algorithmAttachment;
    juce::Label algorithmLabel;
    
    juce::Slider tone;
    std::unique_ptr<SliderAttachment> toneAttachment;
    juce::Label toneLabel;
    
    juce::ToggleButton mono;
    std::unique_ptr<ButtonAttachment> monoAttachment;
    juce::Label monoLabel;
    
    juce::AudioProcessorValueTreeState &parameters;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMsynthAudioProcessorEditor)
};
