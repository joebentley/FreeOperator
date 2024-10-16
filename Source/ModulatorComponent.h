/*
  ==============================================================================

    ModulatorComponent.h
    Created: 28 Sep 2024 9:00:40pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SequencerComponent.h"

//================================ Tab 1 =======================================
class ModulatorComponent  : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
public:
    ModulatorComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber);
    ~ModulatorComponent() override;

    void resized() override;

private:
    void disableControlsForNoise();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    static constexpr int rotaryBoxWidth = 70;
    static constexpr int rotaryBoxHeight = 20;
    static constexpr int rotaryWidth = 80;
    
    int oscNumber;
    
    juce::Slider coarseRandom, fineRandom, levelRandom;
    juce::Label coarseRandomLabel, fineRandomLabel, levelRandomLabel, oscNumLabel;
    std::unique_ptr<SliderAttachment> coarseRandomAttachment, fineRandomAttachment, levelRandomAttachment;
    
    juce::AudioParameterBool* fixedParameter;
    juce::AudioProcessorValueTreeState &parameters;
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulatorComponents)
};

class ModulatorGlobal : public juce::Component
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
public:
    ModulatorGlobal(juce::AudioProcessorValueTreeState &parameters);
    ~ModulatorGlobal();
    
    void resized() override;
private:
    juce::AudioProcessorValueTreeState &parameters;
    
    static constexpr int rotaryBoxWidth = 70;
    static constexpr int rotaryBoxHeight = 20;
    static constexpr int rotaryWidth = 80;
    
    juce::Label globalLabel;
    juce::Slider timeRandom;
    std::unique_ptr<SliderAttachment> timeRandomAttachment;
    juce::Label timeRandomLabel;

    SequencerComponent sequencer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulatorGlobal)
};

class ModulatorTab1 : public juce::Component
{
public:
    ModulatorTab1(juce::AudioProcessorValueTreeState &parameters) : mods(parameters), globalMod(parameters)
    {
        addAndMakeVisible(mods);
        addAndMakeVisible(globalMod);
    }
    
    void resized() override;
private:
    ModulatorComponents mods;
    ModulatorGlobal globalMod;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulatorTab1)
};

//================================ Tab 2 =======================================
class ModulatorPitch : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
public:
    ModulatorPitch(juce::AudioProcessorValueTreeState &parameters);
    ~ModulatorPitch();
    
    void resized() override;
private:
    void disableControlsForNoise();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    juce::AudioProcessorValueTreeState &parameters;
    
    static constexpr int rotaryBoxWidth = 70;
    static constexpr int rotaryBoxHeight = 20;
    static constexpr int rotaryWidth = 80;
    
    juce::Slider offset, decay, amount;
    std::unique_ptr<SliderAttachment> offsetAttachment, decayAttachment, amountAttachment;
    juce::Label offsetLabel, decayLabel, amountLabel;
    
    juce::ToggleButton oscs[4];
    std::unique_ptr<ButtonAttachment> oscAttachments[4];
    juce::Label oscLabels[4];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulatorPitch)
};

class ModulatorTab2 : public juce::Component
{
public:
    ModulatorTab2(juce::AudioProcessorValueTreeState &parameters) : pitch(parameters)
    {
        addAndMakeVisible(pitch);
    }
    
    void resized() override;
private:
    ModulatorPitch pitch;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulatorTab2)
};
