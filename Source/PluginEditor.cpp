/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMsynthAudioProcessorEditor::FMsynthAudioProcessorEditor (FMsynthAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), osc1(vts, 1), osc2(vts, 2), osc3(vts, 3), osc4(vts, 4), parameters(vts)
{
    addAndMakeVisible(midiKeyboardComponent);
    midiKeyboardComponent.setMidiChannel(2);
    
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(osc3);
    addAndMakeVisible(osc4);
    
    int width = 550;
    int height = 500;
    if (juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_Standalone)
        height += 80;
    setSize (width, height);
    
    midiKeyboardState.addListener(&audioProcessor.getMidiMessageCollector());
    
    addAndMakeVisible(algorithm);
    algorithm.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    algorithm.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    algorithmAttachment = std::make_unique<SliderAttachment>(parameters, "algorithm", algorithm);
    
//    addAndMakeVisible(algorithmLabel);
//    algorithmLabel.setText("Algorithm", juce::dontSendNotification);
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
    
    osc4.setBounds(area.removeFromTop(130));
    osc3.setBounds(area.removeFromTop(90));
    osc2.setBounds(area.removeFromTop(90));
    osc1.setBounds(area.removeFromTop(90));
    
    algorithm.setBounds(area.removeFromTop(90));
}
