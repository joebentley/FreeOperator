/*
  ==============================================================================

    SequencerComponent.cpp
    Created: 10 Oct 2024 3:34:12pm
    Author:  Joe Bentley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SequencerComponent.h"

LEDComponent::LEDComponent()
{
}

LEDComponent::~LEDComponent()
{
}

void LEDComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    if (isEnabled)
        g.setColour (juce::Colours::whitesmoke);
    else
        g.setColour (juce::Colours::grey);
    g.drawEllipse(5, 5, 10, 10, 2);
    
    if (isOn) {
        if (isRecording)
            g.setColour(juce::Colours::red);
        else
            g.setColour(juce::Colours::whitesmoke);
        g.fillEllipse(6, 6, 8, 8);
    }
}

void LEDComponent::resized()
{
}

//================================================================================================

SequencerLEDsComponent::SequencerLEDsComponent(juce::AudioProcessorValueTreeState &parameters) : parameters(parameters)
{
    bool isRepeating = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("randomRepeat"))->get();
    for (int i = 0; i < MAX_SEQUENCE_LENGTH; ++i) {
        addAndMakeVisible(leds[i]);
        leds[i].setIsRecording(!isRepeating);
    }

    parameters.state.addListener(this);
    parameters.addParameterListener("randomRepeat", this);
    parameters.addParameterListener("sequenceLength", this);
    
    leds[static_cast<int>(parameters.state.getProperty("sequenceIndex"))].setIsOn(true);
    updateSequenceLength();
}

SequencerLEDsComponent::~SequencerLEDsComponent()
{
    parameters.state.removeListener(this);
    parameters.removeParameterListener("randomRepeat", this);
    parameters.removeParameterListener("sequenceLength", this);
}

void SequencerLEDsComponent::valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged,
                                                   const juce::Identifier& property)
{
    if (property == juce::Identifier("sequenceIndex")) {
        int index = parameters.state.getProperty("sequenceIndex");
        for (int i = 0; i < MAX_SEQUENCE_LENGTH; ++i) {
            leds[i].setIsOn(i == index);
        }
        triggerAsyncUpdate();
    }
}

void SequencerLEDsComponent::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "randomRepeat") {
        auto isRepeating = newValue > 0;
        for (int i = 0; i < MAX_SEQUENCE_LENGTH; ++i)
            leds[i].setIsRecording(!isRepeating);
        triggerAsyncUpdate();
    } else if (parameterID == "sequenceLength") {
        updateSequenceLength();
    }
}

void SequencerLEDsComponent::updateSequenceLength()
{
    auto length = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("sequenceLength"))->get();
    for (int i = 0; i < MAX_SEQUENCE_LENGTH; ++i) {
        leds[i].setIsEnabled(i < length);
    }
    triggerAsyncUpdate();
}

void SequencerLEDsComponent::handleAsyncUpdate()
{
    for (int i = 0; i < MAX_SEQUENCE_LENGTH; ++i) {
        leds[i].repaint();
    }
}

void SequencerLEDsComponent::resized()
{
    auto area = getLocalBounds();
    // TODO: make this react to MAX_SEQUENCE_LENGTH
    auto row1 = area.removeFromTop(20);
    for (int i = 0; i < 8; ++i)
        leds[i].setBounds(row1.removeFromLeft(20));
    auto row2 = area.removeFromTop(20);
    for (int i = 0; i < 8; ++i)
        leds[i+8].setBounds(row2.removeFromLeft(20));
}

SequencerComponent::SequencerComponent(juce::AudioProcessorValueTreeState &parameters) : parameters(parameters), sequencerLEDs(parameters)
{
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
    
    addAndMakeVisible(sequenceLength);
    sequenceLengthAttachment = std::make_unique<SliderAttachment>(parameters, "sequenceLength", sequenceLength);
    sequenceLength.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    
    addAndMakeVisible(sequenceLengthLabel);
    sequenceLengthLabel.setText("Sequence Length", juce::dontSendNotification);
    
    addAndMakeVisible(sequencerLEDs);
}

SequencerComponent::~SequencerComponent()
{
    parameters.removeParameterListener("randomRepeat", this);
}

void SequencerComponent::parameterChanged (const juce::String& parameterID, float newValue)
{
    randomRepeatHold.setEnabled(dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("randomRepeat"))->get());
}

void SequencerComponent::resized()
{
    auto area = getLocalBounds();
    auto controlsRow1 = area.removeFromTop(50);
    randomRepeatLabel.setBounds(controlsRow1.removeFromLeft(80));
    randomRepeat.setBounds(controlsRow1.removeFromLeft(40).withTrimmedLeft(8));
    
    controlsRow1.removeFromLeft(20);
    randomRepeatHold.setBounds(controlsRow1.removeFromLeft(80).withSizeKeepingCentre(80, 30));
    
    auto controlsRow2 = area.removeFromTop(50);
    sequenceLengthLabel.setBounds(controlsRow2.removeFromLeft(120));
    sequenceLength.setBounds(controlsRow2.removeFromLeft(120));
    
    sequencerLEDs.setBounds(area.removeFromTop(100).withTrimmedLeft(10));
}
