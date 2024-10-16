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
    coarseRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    fineRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    fineRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    levelRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    levelRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    
    fixedParameter = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter(parameterPrefix + "Fixed"));
    coarseRandom.setEnabled(!fixedParameter->get());
    parameters.addParameterListener(parameterPrefix + "Fixed", this);
    
    coarseRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "CoarseRandom", coarseRandom);
    fineRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "FineRandom", fineRandom);
    levelRandomAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "LevelRandom", levelRandom);
    
    addAndMakeVisible(oscNumLabel);
    oscNumLabel.setText(juce::String(oscNumber), juce::dontSendNotification);
    oscNumLabel.setFont(juce::FontOptions(20));
    
    if (oscNumber == 4) {
        addAndMakeVisible(coarseRandomLabel);
        addAndMakeVisible(fineRandomLabel);
        addAndMakeVisible(levelRandomLabel);
        coarseRandomLabel.setText("Rand. Coarse", juce::dontSendNotification);
        coarseRandomLabel.setJustificationType(juce::Justification::centredBottom);
        fineRandomLabel.setText("Rand. Fine", juce::dontSendNotification);
        fineRandomLabel.setJustificationType(juce::Justification::centredBottom);
        levelRandomLabel.setText("Rand. Level", juce::dontSendNotification);
        levelRandomLabel.setJustificationType(juce::Justification::centredBottom);
    }
}

ModulatorComponent::~ModulatorComponent()
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    parameters.removeParameterListener(parameterPrefix + "Fixed", this);
}

void ModulatorComponent::parameterChanged (const juce::String& parameterID, float newValue)
{
    juce::Value monoParameter = parameters.getParameterAsValue("mono");
    
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
    }
    
    oscNumLabel.setBounds(area.removeFromLeft(50).withTrimmedLeft(18).withTrimmedBottom(30));
    coarseRandom.setBounds(area.removeFromLeft(rotaryWidth));
    fineRandom.setBounds(area.removeFromLeft(rotaryWidth));
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

// ===========================================================================================

ModulatorGlobal::ModulatorGlobal(juce::AudioProcessorValueTreeState &parameters) : parameters(parameters), sequencer(parameters)
{
    addAndMakeVisible(globalLabel);
    globalLabel.setText("Global", juce::dontSendNotification);
    globalLabel.setFont(juce::FontOptions(30));
    globalLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(timeRandom);
    timeRandom.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    timeRandom.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    timeRandomAttachment = std::make_unique<SliderAttachment>(parameters, "timeRandom", timeRandom);
    
    addAndMakeVisible(timeRandomLabel);
    timeRandomLabel.setText("Rand. Time", juce::dontSendNotification);
    
    addAndMakeVisible(sequencer);
}

ModulatorGlobal::~ModulatorGlobal()
{
}

void ModulatorGlobal::resized()
{
    auto area = getLocalBounds();
    globalLabel.setBounds(area.removeFromTop(80));
    
    auto controlsArea = area.removeFromTop(400);
    
    auto controlsRow1 = controlsArea.removeFromTop(110);
    auto controlsRow1Cell1 = controlsRow1.removeFromLeft(rotaryWidth);
    timeRandomLabel.setBounds(controlsRow1Cell1.removeFromTop(20));
    timeRandom.setBounds(controlsRow1Cell1);
    
    sequencer.setBounds(controlsArea.removeFromTop(300));
}

// ===========================================================================================

void ModulatorTab1::resized()
{
    auto area = getLocalBounds();
    mods.setBounds(area.removeFromLeft(300));
    area.removeFromLeft(50);
    globalMod.setBounds(area.removeFromLeft(300));
}

//================================ Tab 2 =======================================

ModulatorPitch::ModulatorPitch(juce::AudioProcessorValueTreeState &parameters) : parameters(parameters)
{
    addAndMakeVisible(offset);
    offset.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    offset.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    offsetAttachment = std::make_unique<SliderAttachment>(parameters, "pitchSemitone", offset);
    
    addAndMakeVisible(offsetLabel);
    offsetLabel.setText("Offset", juce::dontSendNotification);
    offsetLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(decay);
    decay.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decay.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    decayAttachment = std::make_unique<SliderAttachment>(parameters, "pitchDecay", decay);
    
    addAndMakeVisible(decayLabel);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(amount);
    amount.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    amount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    amountAttachment = std::make_unique<SliderAttachment>(parameters, "pitchAmount", amount);
    
    addAndMakeVisible(amountLabel);
    amountLabel.setText("Amount", juce::dontSendNotification);
    amountLabel.setJustificationType(juce::Justification::centred);
}

ModulatorPitch::~ModulatorPitch()
{
}

void ModulatorPitch::resized()
{
    auto area = getLocalBounds();
    
    auto controlsRow = area.removeFromTop(110);
    controlsRow.removeFromLeft(50);
    auto offsetArea = controlsRow.removeFromLeft(rotaryWidth);
    offsetLabel.setBounds(offsetArea.removeFromTop(20));
    offset.setBounds(offsetArea);
    
    auto decayArea = controlsRow.removeFromLeft(rotaryWidth);
    decayLabel.setBounds(decayArea.removeFromTop(20));
    decay.setBounds(decayArea);
    
    auto amountArea = controlsRow.removeFromLeft(rotaryWidth);
    amountLabel.setBounds(amountArea.removeFromTop(20));
    amount.setBounds(amountArea);
}

void ModulatorTab2::resized()
{
    auto area = getLocalBounds();
    pitch.setBounds(area.removeFromLeft(300));
}
