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

SequencerComponent::SequencerComponent(juce::AudioProcessorValueTreeState &parameters) : parameters(parameters)
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

SequencerComponent::~SequencerComponent()
{
    parameters.state.removeListener(this);
    parameters.removeParameterListener("randomRepeat", this);
}

void SequencerComponent::valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged,
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

void SequencerComponent::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "randomRepeat") {
        auto isRepeating = newValue > 0;
        for (int i = 0; i < 8; ++i)
            leds[i].setIsRecording(!isRepeating);
        triggerAsyncUpdate();
    }
}

void SequencerComponent::handleAsyncUpdate()
{
    for (int i = 0; i < 8; ++i) {
        leds[i].repaint();
    }
}

void SequencerComponent::resized()
{
    auto area = getLocalBounds();
    for (int i = 0; i < 8; ++i)
        leds[i].setBounds(area.removeFromLeft(20));
}
