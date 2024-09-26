/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 23 Sep 2024 4:51:06pm
    Author:  Joe Bentley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState &parameters, const juce::String &parameterPrefix)
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
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
}

void ADSRComponent::resized()
{
    auto adsrArea = getLocalBounds();
    attack.setBounds(adsrArea.removeFromLeft(100));
    decay.setBounds(adsrArea.removeFromLeft(100));
    sustain.setBounds(adsrArea.removeFromLeft(100));
    release.setBounds(adsrArea.removeFromLeft(100));
}
