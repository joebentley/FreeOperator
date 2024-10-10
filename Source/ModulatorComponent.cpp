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
    
    addAndMakeVisible(randomRepeat);
    randomRepeatAttachment = std::make_unique<ButtonAttachment>(parameters, "randomRepeat", randomRepeat);
    
    addAndMakeVisible(randomRepeatLabel);
    randomRepeatLabel.setText("Repeat Sequence", juce::dontSendNotification);
    
    addAndMakeVisible(randomRepeatHold);
    randomRepeatHold.setButtonText("Overwrite");
    
    randomRepeatHold.onStateChange = [&] {
        auto repeat = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("randomRepeat"));
        auto isOver = randomRepeatHold.isOver();
        auto isDown = randomRepeatHold.isDown();
        
        if (isDown && repeat->get()) {
            // Prevent infinite loop
            parameters.removeParameterListener("randomRepeat", this);
            repeat->beginChangeGesture();
            repeat->setValueNotifyingHost(0.0);
            repeat->endChangeGesture();
            parameters.addParameterListener("randomRepeat", this);
        } else if (isOver && !isDown && !repeat->get()) {
            parameters.removeParameterListener("randomRepeat", this);
            repeat->beginChangeGesture();
            repeat->setValueNotifyingHost(1.0);
            repeat->endChangeGesture();
            parameters.addParameterListener("randomRepeat", this);
        }
    };
    
    parameters.addParameterListener("randomRepeat", this);
    randomRepeatHold.setEnabled(dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("randomRepeat"))->get());
    
    addAndMakeVisible(sequencer);
}

ModulatorGlobal::~ModulatorGlobal()
{
    parameters.removeParameterListener("randomRepeat", this);
}

void ModulatorGlobal::parameterChanged (const juce::String& parameterID, float newValue)
{
    randomRepeatHold.setEnabled(dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("randomRepeat"))->get());
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
    
    auto controlsRow2 = controlsArea.removeFromTop(110);
    randomRepeatLabel.setBounds(controlsRow2.removeFromLeft(80));
    randomRepeat.setBounds(controlsRow2.removeFromLeft(40).withTrimmedLeft(8));
    
    controlsRow2.removeFromLeft(20);
    randomRepeatHold.setBounds(controlsRow2.removeFromLeft(80).withSizeKeepingCentre(80, 30));
    
    sequencer.setBounds(controlsArea.removeFromTop(20));
}

// ===========================================================================================

void ModulatorTab::resized()
{
    auto area = getLocalBounds();
    mods.setBounds(area.removeFromLeft(300));
    area.removeFromLeft(50);
    globalMod.setBounds(area.removeFromLeft(300));
}
