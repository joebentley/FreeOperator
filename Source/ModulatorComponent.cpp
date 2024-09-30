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
ModulatorComponent::ModulatorComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber) : oscNumber(oscNumber)
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    
    addAndMakeVisible(coarseRandom);
    addAndMakeVisible(levelRandom);
    coarseRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    levelRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    levelRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    
    coarseRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "CoarseRandom", coarseRandom);
    levelRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "LevelRandom", levelRandom);
    
    addAndMakeVisible(oscNumLabel);
    oscNumLabel.setText(juce::String(oscNumber), juce::dontSendNotification);
    oscNumLabel.setFont(juce::FontOptions(20));
    
    if (oscNumber == 4) {
        addAndMakeVisible(coarseRandomLabel);
        addAndMakeVisible(levelRandomLabel);
        coarseRandomLabel.setText("Rand. Coarse", juce::dontSendNotification);
        coarseRandomLabel.setJustificationType(juce::Justification::centredBottom);
        levelRandomLabel.setText("Rand. Level", juce::dontSendNotification);
        levelRandomLabel.setJustificationType(juce::Justification::centredBottom);
    }
}

ModulatorComponent::~ModulatorComponent()
{
}

void ModulatorComponent::resized()
{
    auto area = getLocalBounds();
    
    if (oscNumber == 4) {
        auto labelArea = area.removeFromTop(40);
        labelArea.removeFromLeft(50);
        coarseRandomLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        levelRandomLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
    }
    
    oscNumLabel.setBounds(area.removeFromLeft(50).withTrimmedLeft(18).withTrimmedBottom(30));
    coarseRandom.setBounds(area.removeFromLeft(rotaryWidth));
    levelRandom.setBounds(area.removeFromLeft(rotaryWidth));
}

void ModulatorComponents::resized()
{
    auto area = getLocalBounds();
    mod4.setBounds(area.removeFromTop(130));
    mod3.setBounds(area.removeFromTop(90));
    mod2.setBounds(area.removeFromTop(90));
    mod1.setBounds(area.removeFromTop(90));
}
