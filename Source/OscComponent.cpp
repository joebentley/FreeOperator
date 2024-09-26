/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 23 Sep 2024 4:51:06pm
    Author:  Joe Bentley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState &parameters, const juce::String &parameterPrefix)
{
    addAndMakeVisible(attack);
    addAndMakeVisible(decay);
    addAndMakeVisible(sustain);
    addAndMakeVisible(release);
    attack.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attack.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    decay.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decay.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    sustain.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    release.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    release.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    
    attackAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Attack", attack);
    decayAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Decay", decay);
    sustainAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Sustain", sustain);
    releaseAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Release", release);
    
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(sustainLabel);
    addAndMakeVisible(releaseLabel);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centredBottom);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centredBottom);
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centredBottom);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centredBottom);
    
    addAndMakeVisible(coarseOsc);
    coarseOsc.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseOsc.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    coarseOscAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Coarse", coarseOsc);
    
    addAndMakeVisible(coarseOscLabel);
    coarseOscLabel.setText("Coarse", juce::dontSendNotification);
    coarseOscLabel.setJustificationType(juce::Justification::centredBottom);
    
    addAndMakeVisible(volume);
    volume.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    volumeAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Volume", volume);
    
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Level", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centredBottom);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
}

void OscComponent::resized()
{
    auto area = getLocalBounds();
    auto labelArea = area.removeFromTop(40);
    attackLabel.setBounds(labelArea.removeFromLeft(100));
    decayLabel.setBounds(labelArea.removeFromLeft(100));
    sustainLabel.setBounds(labelArea.removeFromLeft(100));
    releaseLabel.setBounds(labelArea.removeFromLeft(100));
    coarseOscLabel.setBounds(labelArea.removeFromLeft(100));
    volumeLabel.setBounds(labelArea.removeFromLeft(100));
    
    attack.setBounds(area.removeFromLeft(100));
    decay.setBounds(area.removeFromLeft(100));
    sustain.setBounds(area.removeFromLeft(100));
    release.setBounds(area.removeFromLeft(100));
    coarseOsc.setBounds(area.removeFromLeft(100));
    volume.setBounds(area.removeFromLeft(100));
}
