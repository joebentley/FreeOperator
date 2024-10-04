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
ModulatorComponent::ModulatorComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber) : oscNumber(oscNumber), parameters(parameters)
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    
    addAndMakeVisible(coarseRandom);
    addAndMakeVisible(fineRandom);
    addAndMakeVisible(levelRandom);
    addAndMakeVisible(decayRandom);
    coarseRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    fineRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    fineRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    levelRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    levelRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    decayRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decayRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    
    fixedParameter = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter(parameterPrefix + "Fixed"));
    coarseRandom.setEnabled(!fixedParameter->get());
    parameters.addParameterListener(parameterPrefix + "Fixed", this);
    
    coarseRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "CoarseRandom", coarseRandom);
    fineRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "FineRandom", fineRandom);
    levelRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "LevelRandom", levelRandom);
    decayRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "DecayRandom", decayRandom);
    
    addAndMakeVisible(oscNumLabel);
    oscNumLabel.setText(juce::String(oscNumber), juce::dontSendNotification);
    oscNumLabel.setFont(juce::FontOptions(20));
    
    if (oscNumber == 4) {
        addAndMakeVisible(coarseRandomLabel);
        addAndMakeVisible(fineRandomLabel);
        addAndMakeVisible(levelRandomLabel);
        addAndMakeVisible(decayRandomLabel);
        coarseRandomLabel.setText("Rand. Coarse", juce::dontSendNotification);
        coarseRandomLabel.setJustificationType(juce::Justification::centredBottom);
        fineRandomLabel.setText("Rand. Fine", juce::dontSendNotification);
        fineRandomLabel.setJustificationType(juce::Justification::centredBottom);
        levelRandomLabel.setText("Rand. Level", juce::dontSendNotification);
        levelRandomLabel.setJustificationType(juce::Justification::centredBottom);
        decayRandomLabel.setText("Rand. Decay", juce::dontSendNotification);
        decayRandomLabel.setJustificationType(juce::Justification::centredBottom);
    }
}

ModulatorComponent::~ModulatorComponent()
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    parameters.removeParameterListener(parameterPrefix + "Fixed", this);
}

void ModulatorComponent::parameterChanged (const juce::String& parameterID, float newValue)
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    if (parameterID == parameterPrefix + "Fixed")
        coarseRandom.setEnabled(!fixedParameter->get());
}

void ModulatorComponent::resized()
{
    auto area = getLocalBounds();
    
    if (oscNumber == 4) {
        auto labelArea = area.removeFromTop(40);
        labelArea.removeFromLeft(50);
        coarseRandomLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        fineRandomLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        levelRandomLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        decayRandomLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
    }
    
    oscNumLabel.setBounds(area.removeFromLeft(50).withTrimmedLeft(18).withTrimmedBottom(30));
    coarseRandom.setBounds(area.removeFromLeft(rotaryWidth));
    fineRandom.setBounds(area.removeFromLeft(rotaryWidth));
    levelRandom.setBounds(area.removeFromLeft(rotaryWidth));
    decayRandom.setBounds(area.removeFromLeft(rotaryWidth));
}

void ModulatorComponents::resized()
{
    auto area = getLocalBounds();
    mod4.setBounds(area.removeFromTop(130));
    mod3.setBounds(area.removeFromTop(90));
    mod2.setBounds(area.removeFromTop(90));
    mod1.setBounds(area.removeFromTop(90));
}
