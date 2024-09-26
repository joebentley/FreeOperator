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
    OscComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    static constexpr int rotaryBoxWidth = 70;
    static constexpr int rotaryBoxHeight = 20;
    static constexpr int rotaryWidth = 80;
    
    int oscNumber;
    
    juce::Slider attack, decay, sustain, release, coarseOsc, volume;
    juce::Label oscNumLabel, attackLabel, decayLabel, sustainLabel, releaseLabel, coarseOscLabel, volumeLabel;
    
    std::unique_ptr<SliderAttachment> attackAttachment, decayAttachment, sustainAttachment,
        releaseAttachment, coarseOscAttachment, volumeAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
