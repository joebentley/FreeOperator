/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMsynthAudioProcessorEditor::FMsynthAudioProcessorEditor (FMsynthAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), adsr1(vts, "osc1"), adsr2(vts, "osc2"), parameters(vts)
{
    addAndMakeVisible(midiKeyboardComponent);
    midiKeyboardComponent.setMidiChannel(2);
    
    addAndMakeVisible(adsr1);
    adsr1.setName("adsrOsc1");
    
    addAndMakeVisible(adsr2);
    adsr2.setName("adsrOsc2");
    
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
    
    setSize (400, 600);
    
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
    
    auto adsrArea = area.removeFromTop(150);
    adsr1.setBounds(adsrArea);
    
    auto adsrArea2 = area.removeFromTop(150);
    adsr2.setBounds(adsrArea2);
    
    auto fmDepthArea = area.removeFromTop(150);
    fmDepth.setBounds(fmDepthArea.removeFromLeft(100));
    coarseOsc1.setBounds(fmDepthArea.removeFromLeft(100));
    coarseOsc2.setBounds(fmDepthArea.removeFromLeft(100));
}
