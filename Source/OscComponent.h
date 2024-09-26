/*
  ==============================================================================

    ADSRComponent.h
    Created: 23 Sep 2024 4:51:06pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
public:
    OscComponent(juce::AudioProcessorValueTreeState &parameters, const juce::String &parameterPrefix);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    static constexpr int rotaryBoxWidth = 80;
    static constexpr int rotaryBoxHeight = 20;
    
    juce::Slider attack, decay, sustain, release;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
