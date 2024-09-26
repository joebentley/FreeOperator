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
    osc1.setName("adsrOsc1");
    
    addAndMakeVisible(osc2);
    osc2.setName("adsrOsc2");
    
    addAndMakeVisible(fmDepth);
    fmDepth.setName("fmDepth");
    fmDepth.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    fmDepth.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    fmDepthAttachment = std::make_unique<SliderAttachment>(parameters, "fmDepth", fmDepth);
    
    addAndMakeVisible(coarseOsc1);
    coarseOsc1.setName("coarseOsc1");
    coarseOsc1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseOsc1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    coarseOsc1Attachment = std::make_unique<SliderAttachment>(parameters, "osc1Coarse", coarseOsc1);
    
    addAndMakeVisible(coarseOsc2);
    coarseOsc2.setName("coarseOsc2");
    coarseOsc2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    coarseOsc2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, rotaryBoxWidth, rotaryBoxHeight);
    coarseOsc2Attachment = std::make_unique<SliderAttachment>(parameters, "osc2Coarse", coarseOsc2);
    
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
    
    auto fmDepthArea = area.removeFromTop(150);
    fmDepth.setBounds(fmDepthArea.removeFromLeft(100));
    coarseOsc1.setBounds(fmDepthArea.removeFromLeft(100));
    coarseOsc2.setBounds(fmDepthArea.removeFromLeft(100));
}
