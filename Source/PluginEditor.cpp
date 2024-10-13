/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMsynthAudioProcessorEditor::FMsynthAudioProcessorEditor (FMsynthAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), oscs(vts), modTab(vts), parameters(vts)
{
    addAndMakeVisible(midiKeyboardComponent);
    midiKeyboardComponent.setMidiChannel(2);
    
    addAndMakeVisible(tabs);
    auto backgroundColour = getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
    tabs.addTab("Oscillators", backgroundColour, &oscs, false);
    tabs.addTab("Modulators", backgroundColour, &modTab, false);

    int width = 750;
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
    toneLabel.setText("AA Filter", juce::dontSendNotification);
    
    addAndMakeVisible(mono);
    monoAttachment = std::make_unique<ButtonAttachment>(parameters, "mono", mono);
    addAndMakeVisible(monoLabel);
    monoLabel.setText("Mono", juce::dontSendNotification);
    
    addAndMakeVisible(overdrivePhase);
    overdrivePhaseAttachment = std::make_unique<ButtonAttachment>(parameters, "overdrivePhase", overdrivePhase);
    addAndMakeVisible(overdrivePhaseLabel);
    overdrivePhaseLabel.setText("Overdrive Phase", juce::dontSendNotification);
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
}

void FMsynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    if (juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_Standalone)
        midiKeyboardComponent.setBounds(area.removeFromTop(80));
    
    tabs.setBounds(area.removeFromTop(480).reduced(5));
    
    auto bottomArea = area.removeFromTop(90).withTrimmedLeft(10);
    algorithmLabel.setBounds(bottomArea.removeFromLeft(90));
    algorithm.setBounds(bottomArea.removeFromLeft(90));
    
    toneLabel.setBounds(bottomArea.removeFromLeft(90).withTrimmedLeft(20));
    tone.setBounds(bottomArea.removeFromLeft(90));
    
    monoLabel.setBounds(bottomArea.removeFromLeft(90).withTrimmedLeft(40));
    mono.setBounds(bottomArea.removeFromLeft(40).withTrimmedLeft(8));
    
    overdrivePhaseLabel.setBounds(bottomArea.removeFromLeft(100).withTrimmedLeft(0));
    overdrivePhase.setBounds(bottomArea.removeFromLeft(30).withTrimmedLeft(8));
}
