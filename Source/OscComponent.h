/*
  ==============================================================================

    ADSRComponent.h
    Created: 23 Sep 2024 4:51:06pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscComponent  : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
public:
    OscComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber);

    void resized() override;
    
private:
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    static constexpr int rotaryBoxWidth = 70;
    static constexpr int rotaryBoxHeight = 20;
    static constexpr int rotaryWidth = 80;
    
    int oscNumber;
    
    juce::Slider attack, decay, sustain, release, coarse, fine, volume;
    juce::ToggleButton fixed;
    juce::Label oscNumLabel, attackLabel, decayLabel, sustainLabel, releaseLabel, coarseLabel, fineLabel, fixedLabel, volumeLabel;
    
    std::unique_ptr<SliderAttachment> attackAttachment, decayAttachment, sustainAttachment,
        releaseAttachment, coarseAttachment, fineAttachment, volumeAttachment;
    std::unique_ptr<ButtonAttachment> fixedAttachment;
    
    juce::AudioParameterBool* fixedParameter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};

class OscComponents : public juce::Component
{
public:
    OscComponents(juce::AudioProcessorValueTreeState &parameters)
    : osc1(parameters, 1), osc2(parameters, 2), osc3(parameters, 3), osc4(parameters, 4)
    {
        addAndMakeVisible(osc1);
        addAndMakeVisible(osc2);
        addAndMakeVisible(osc3);
        addAndMakeVisible(osc4);
    }

    void resized() override;
private:
    OscComponent osc1;
    OscComponent osc2;
    OscComponent osc3;
    OscComponent osc4;
};
