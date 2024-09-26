/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMsynthAudioProcessorEditor::FMsynthAudioProcessorEditor (FMsynthAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), osc1(vts, "osc1"), osc2(vts, "osc2"), parameters(vts)
{
    addAndMakeVisible(midiKeyboardComponent);
    midiKeyboardComponent.setMidiChannel(2);
    
    addAndMakeVisible(osc1);
    
    addAndMakeVisible(osc2);
    
    setSize (600, 600);
    
    midiKeyboardState.addListener(&audioProcessor.getMidiMessageCollector());
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
    if (juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_Standalone) {
        midiKeyboardComponent.setBounds(area.removeFromTop(80));
    }
    
    osc1.setBounds(area.removeFromTop(150));
    
    osc2.setBounds(area.removeFromTop(150));
}
