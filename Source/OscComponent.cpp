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
OscComponent::OscComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber) : oscNumber(oscNumber)
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    
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
    
    addAndMakeVisible(coarseOsc);
    coarseOsc.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseOsc.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    coarseOscAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Coarse", coarseOsc);
    
    addAndMakeVisible(volume);
    volume.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    volumeAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Volume", volume);
    
    addAndMakeVisible(oscNumLabel);
    oscNumLabel.setText(juce::String(oscNumber), juce::dontSendNotification);
//    oscNumLabel.setJustificationType(juce::Justification::centred);
    oscNumLabel.setFont(juce::FontOptions(20));
    
    if (oscNumber == 4) {
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
        
        addAndMakeVisible(coarseOscLabel);
        coarseOscLabel.setText("Coarse", juce::dontSendNotification);
        coarseOscLabel.setJustificationType(juce::Justification::centredBottom);
        
        addAndMakeVisible(volumeLabel);
        volumeLabel.setText("Level", juce::dontSendNotification);
        volumeLabel.setJustificationType(juce::Justification::centredBottom);
    }
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
    
    if (oscNumber == 4) {
        auto labelArea = area.removeFromTop(40);
        labelArea.removeFromLeft(50);
        attackLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        decayLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        sustainLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        releaseLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        coarseOscLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        volumeLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
    }
    
    oscNumLabel.setBounds(area.removeFromLeft(50).withTrimmedLeft(18).withTrimmedBottom(30));
    attack.setBounds(area.removeFromLeft(rotaryWidth));
    decay.setBounds(area.removeFromLeft(rotaryWidth));
    sustain.setBounds(area.removeFromLeft(rotaryWidth));
    release.setBounds(area.removeFromLeft(rotaryWidth));
    coarseOsc.setBounds(area.removeFromLeft(rotaryWidth));
    volume.setBounds(area.removeFromLeft(rotaryWidth));
}
