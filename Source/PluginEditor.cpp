/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMsynthAudioProcessorEditor::FMsynthAudioProcessorEditor (FMsynthAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), oscs(vts), mods(vts), parameters(vts)
{
    addAndMakeVisible(midiKeyboardComponent);
    midiKeyboardComponent.setMidiChannel(2);
    
    addAndMakeVisible(tabs);
    auto backgroundColour = getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
    tabs.addTab("Oscillators", backgroundColour, &oscs, false);
    tabs.addTab("Modulators", backgroundColour, &mods, false);
//    addAndMakeVisible(oscs);
    
    int width = 550;
    int height = 580;
    if (juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_Standalone)
        height += 80;
    setSize (width, height);
    
    midiKeyboardState.addListener(&audioProcessor.getMidiMessageCollector());
    
    addAndMakeVisible(algorithm);
    algorithm.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    algorithm.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    algorithmAttachment = std::make_unique<SliderAttachment>(parameters, "algorithm", algorithm);
    
    addAndMakeVisible(algorithmLabel);
    algorithmLabel.setText("Algorithm", juce::dontSendNotification);
    
    addAndMakeVisible(tone);
    tone.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    tone.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    toneAttachment = std::make_unique<SliderAttachment>(parameters, "tone", tone);
    
    addAndMakeVisible(toneLabel);
    toneLabel.setText("Tone", juce::dontSendNotification);
}

FMsynthAudioProcessorEditor::~FMsynthAudioProcessorEditor()
{
    midiKeyboardState.removeListener(&audioProcessor.getMidiMessageCollector());
}

//==============================================================================
void FMsynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setColour (juce::Colours::white);
//    g.setFont (juce::FontOptions (15.0f));
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FMsynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    if (juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_Standalone)
        midiKeyboardComponent.setBounds(area.removeFromTop(80));
    
    tabs.setBounds(area.removeFromTop(480).reduced(5));
    
    auto algorithmArea = area.removeFromTop(90).withTrimmedLeft(150);
    algorithmLabel.setBounds(algorithmArea.removeFromLeft(90));
    algorithm.setBounds(algorithmArea.removeFromLeft(90));
    
    toneLabel.setBounds(algorithmArea.removeFromLeft(90));
    tone.setBounds(algorithmArea.removeFromLeft(90));
}
