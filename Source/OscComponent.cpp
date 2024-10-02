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
OscComponent::OscComponent(juce::AudioProcessorValueTreeState &parameters, int oscNumber) : oscNumber(oscNumber), parameters(parameters)
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
    
    addAndMakeVisible(coarse);
    coarse.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarse.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    coarseAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Coarse", coarse);
    
    fixedParameter = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter(parameterPrefix + "Fixed"));
    coarse.setEnabled(!fixedParameter->get());
    parameters.addParameterListener(parameterPrefix + "Fixed", this);
    
    addAndMakeVisible(fine);
    fine.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    fine.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    fineAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Fine", fine);
    
    addAndMakeVisible(fixed);
    fixedAttachment = std::make_unique<ButtonAttachment>(parameters, parameterPrefix + "Fixed", fixed);
    
    addAndMakeVisible(volume);
    volume.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    volumeAttachment = std::make_unique<SliderAttachment>(parameters, parameterPrefix + "Volume", volume);
    
    addAndMakeVisible(oscNumLabel);
    oscNumLabel.setText(juce::String(oscNumber), juce::dontSendNotification);
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
        
        addAndMakeVisible(coarseLabel);
        coarseLabel.setText("Coarse", juce::dontSendNotification);
        coarseLabel.setJustificationType(juce::Justification::centredBottom);
        
        addAndMakeVisible(fineLabel);
        fineLabel.setText("Fine", juce::dontSendNotification);
        fineLabel.setJustificationType(juce::Justification::centredBottom);
        
        addAndMakeVisible(fixedLabel);
        fixedLabel.setText("Fixed", juce::dontSendNotification);
        fixedLabel.setJustificationType(juce::Justification::centredBottom);
        
        addAndMakeVisible(volumeLabel);
        volumeLabel.setText("Level", juce::dontSendNotification);
        volumeLabel.setJustificationType(juce::Justification::centredBottom);
    }
}

OscComponent::~OscComponent()
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    parameters.removeParameterListener(parameterPrefix + "Fixed", this);
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
        coarseLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        fineLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
        fixedLabel.setBounds(labelArea.removeFromLeft(40));
        volumeLabel.setBounds(labelArea.removeFromLeft(rotaryWidth));
    }
    
    oscNumLabel.setBounds(area.removeFromLeft(50).withTrimmedLeft(18).withTrimmedBottom(30));
    attack.setBounds(area.removeFromLeft(rotaryWidth));
    decay.setBounds(area.removeFromLeft(rotaryWidth));
    sustain.setBounds(area.removeFromLeft(rotaryWidth));
    release.setBounds(area.removeFromLeft(rotaryWidth));
    coarse.setBounds(area.removeFromLeft(rotaryWidth));
    fine.setBounds(area.removeFromLeft(rotaryWidth));
    fixed.setBounds(area.removeFromLeft(40).withTrimmedLeft(8));
    volume.setBounds(area.removeFromLeft(rotaryWidth));
}

void OscComponent::parameterChanged (const juce::String& parameterID, float newValue)
{
    juce::String parameterPrefix = "osc" + juce::String(oscNumber);
    if (parameterID == parameterPrefix + "Fixed")
        coarse.setEnabled(!fixedParameter->get());
}

void OscComponents::resized()
{
    auto area = getLocalBounds();
    osc4.setBounds(area.removeFromTop(130));
    osc3.setBounds(area.removeFromTop(90));
    osc2.setBounds(area.removeFromTop(90));
    osc1.setBounds(area.removeFromTop(90));
}
