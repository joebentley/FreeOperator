/*
  ==============================================================================

    ModulatorComponent.cpp
    Created: 28 Sep 2024 9:00:40pm
    Author:  Joe Bentley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModulatorComponent.h"

//==============================================================================
ModulatorComponent::ModulatorComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber)
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    
    addAndMakeVisible(coarseRandomMin);
    addAndMakeVisible(coarseRandomMax);
    addAndMakeVisible(levelRandomMin);
    addAndMakeVisible(levelRandomMax);
    coarseRandomMin.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseRandomMin.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    coarseRandomMax.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseRandomMax.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    levelRandomMin.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    levelRandomMin.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    levelRandomMax.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    levelRandomMax.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    
    coarseRandomMinAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "CoarseRandomMin", coarseRandomMin);
    coarseRandomMaxAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "CoarseRandomMax", coarseRandomMax);
    levelRandomMinAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "LevelRandomMin", levelRandomMin);
    levelRandomMaxAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "LevelRandomMax", levelRandomMax);
}

ModulatorComponent::~ModulatorComponent()
{
}

void ModulatorComponent::resized()
{
    auto area = getLocalBounds();
    coarseRandomMin.setBounds(area.removeFromLeft(rotaryWidth));
    coarseRandomMax.setBounds(area.removeFromLeft(rotaryWidth));
    levelRandomMin.setBounds(area.removeFromLeft(rotaryWidth));
    levelRandomMax.setBounds(area.removeFromLeft(rotaryWidth));
}

void ModulatorComponents::resized()
{
    auto area = getLocalBounds();
    mod4.setBounds(area.removeFromTop(130));
    mod3.setBounds(area.removeFromTop(90));
    mod2.setBounds(area.removeFromTop(90));
    mod1.setBounds(area.removeFromTop(90));
}
