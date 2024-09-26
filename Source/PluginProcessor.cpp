/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using pid = juce::ParameterID;
using nrf = juce::NormalisableRange<float>;
using nri = juce::NormalisableRange<int>;

//==============================================================================
FMsynthAudioProcessor::FMsynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
parameters(*this, nullptr, juce::Identifier("FMSynth"), {
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Attack", 1),   "Osc 1 Attack",     nrf(0.0f, 50.0f, 0.0000001f, 0.4f), 0.01f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Decay", 1),    "Osc 1 Decay",      nrf(0.0f, 50.0f, 0.0000001f, 0.4f), 0.2f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Sustain", 1),  "Osc 1 Sustain",    nrf(0.0f, 1.0f, 0.0000001f), 0.6f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Release", 1),  "Osc 1 Release",    nrf(0.0f, 50.0f, 0.0000001f, 0.4f), 0.1f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Attack", 1),   "Osc 2 Attack",     nrf(0.0f, 50.0f, 0.0000001f, 0.4f), 0.01f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Decay", 1),    "Osc 2 Decay",      nrf(0.0f, 50.0f, 0.0000001f, 0.4f), 0.2f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Sustain", 1),  "Osc 2 Sustain",    nrf(0.0f, 1.0f, 0.0000001f), 1.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Release", 1),  "Osc 2 Release",    nrf(0.0f, 50.0f, 0.0000001f, 0.4f), 0.1f),
    std::make_unique<juce::AudioParameterInt>(  pid("osc1Coarse", 1),   "Osc 1 Coarse",     1, 10, 1),
    std::make_unique<juce::AudioParameterInt>(  pid("osc2Coarse", 1),   "Osc 2 Coarse",     1, 10, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("fmDepth", 1),      "FM Depth",         nrf(0.0f, 1.0f, 0.0000001f, 0.3f), 0.0f),
}),
audioEngine(parameters)
#endif
{
}

FMsynthAudioProcessor::~FMsynthAudioProcessor()
{
}

//==============================================================================
const juce::String FMsynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMsynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMsynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMsynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMsynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMsynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMsynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMsynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMsynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMsynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FMsynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    audioEngine.prepare({sampleRate, (juce::uint32) samplesPerBlock, 2});
    midiMessageCollector.reset(sampleRate);
}

void FMsynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMsynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FMsynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    midiMessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    audioEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool FMsynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMsynthAudioProcessor::createEditor()
{
    return new FMsynthAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void FMsynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FMsynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMsynthAudioProcessor();
}
