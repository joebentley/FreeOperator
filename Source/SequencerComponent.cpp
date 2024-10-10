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

    g.setColour (juce::Colours::whitesmoke);
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
    for (int i = 0; i < 8; ++i) {
        addAndMakeVisible(leds[i]);
        leds[i].setIsRecording(!isRepeating);
    }

    parameters.state.addListener(this);
    parameters.addParameterListener("randomRepeat", this);
    
    leds[static_cast<int>(parameters.state.getProperty("sequenceIndex"))].setIsOn(true);
}

SequencerLEDsComponent::~SequencerLEDsComponent()
{
    parameters.state.removeListener(this);
    parameters.removeParameterListener("randomRepeat", this);
}

void SequencerLEDsComponent::valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged,
                                                   const juce::Identifier& property)
{
    if (property == juce::Identifier("sequenceIndex")) {
        int index = parameters.state.getProperty("sequenceIndex");
        for (int i = 0; i < 8; ++i) {
            leds[i].setIsOn(i == index);
        }
        triggerAsyncUpdate();
    }
}

void SequencerLEDsComponent::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "randomRepeat") {
        auto isRepeating = newValue > 0;
        for (int i = 0; i < 8; ++i)
            leds[i].setIsRecording(!isRepeating);
        triggerAsyncUpdate();
    }
}

void SequencerLEDsComponent::handleAsyncUpdate()
{
    for (int i = 0; i < 8; ++i) {
        leds[i].repaint();
    }
}

void SequencerLEDsComponent::resized()
{
    auto area = getLocalBounds();
    for (int i = 0; i < 8; ++i)
        leds[i].setBounds(area.removeFromLeft(20));
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
    auto controlsRow1 = area.removeFromTop(110);
    randomRepeatLabel.setBounds(controlsRow1.removeFromLeft(80));
    randomRepeat.setBounds(controlsRow1.removeFromLeft(40).withTrimmedLeft(8));
    
    controlsRow1.removeFromLeft(20);
    randomRepeatHold.setBounds(controlsRow1.removeFromLeft(80).withSizeKeepingCentre(80, 30));
    
    sequencerLEDs.setBounds(area.removeFromTop(100));
}
