/*
  ==============================================================================

    ModulatorComponent.h
    Created: 28 Sep 2024 9:00:40pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ModulatorComponent  : public juce::Component
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
public:
    ModulatorComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber);
    ~ModulatorComponent() override;

    void resized() override;

private:
    static constexpr int rotaryBoxWidth = 70;
    static constexpr int rotaryBoxHeight = 20;
    static constexpr int rotaryWidth = 80;
    
    int oscNumber;
    
    juce::Slider coarseRandom, levelRandom;
    juce::Label coarseRandomLabel, levelRandomLabel, oscNumLabel;
    std::unique_ptr<SliderAttachment> coarseRandomAttachment, levelRandomAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulatorComponent)
};

class ModulatorComponents : public juce::Component
{
public:
    ModulatorComponents(juce::AudioProcessorValueTreeState &parameters)
    : mod1(parameters, 1), mod2(parameters, 2), mod3(parameters, 3), mod4(parameters, 4)
    {
        addAndMakeVisible(mod1);
        addAndMakeVisible(mod2);
        addAndMakeVisible(mod3);
        addAndMakeVisible(mod4);
    }

    void resized() override;
private:
    ModulatorComponent mod1;
    ModulatorComponent mod2;
    ModulatorComponent mod3;
    ModulatorComponent mod4;
};

