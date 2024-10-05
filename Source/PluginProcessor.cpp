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
#endif
parameters(*this, nullptr, juce::Identifier("FMSynth"), {
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Attack", 1),   "Osc 1 Attack",     nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.01f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Decay", 1),    "Osc 1 Decay",      nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.2f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Sustain", 1),  "Osc 1 Sustain",    nrf(0.0f, 1.0f, 0.00001f), 0.6f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Release", 1),  "Osc 1 Release",    nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.1f),
    std::make_unique<juce::AudioParameterInt>(  pid("osc1Coarse", 1),   "Osc 1 Coarse",     1, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Fine", 1),     "Osc 1 Fine",       nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterBool>(pid("osc1Fixed", 1),     "Osc 1 Fixed", false),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1Volume", 1),   "Osc 1 Volume",     nrf(0.0f, 1.0f, 0.00001f, 0.3f), 1.0f),
    std::make_unique<juce::AudioParameterInt>(pid("osc1CoarseRandom", 1), "Osc 1 Coarse Random", 0, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1FineRandom", 1), "Osc 1 Fine Random", nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc1LevelRandom", 1), "Osc 1 Level Random", nrf(0.0f, 1.0f, 0.00001f, 0.3f), 0.0f),
    
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Attack", 1),   "Osc 2 Attack",     nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.01f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Decay", 1),    "Osc 2 Decay",      nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.2f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Sustain", 1),  "Osc 2 Sustain",    nrf(0.0f, 1.0f, 0.00001f), 1.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Release", 1),  "Osc 2 Release",    nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.1f),
    std::make_unique<juce::AudioParameterInt>(  pid("osc2Coarse", 1),   "Osc 2 Coarse",     1, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Fine", 1),     "Osc 2 Fine",       nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterBool>(pid("osc2Fixed", 1),     "Osc 2 Fixed", false),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2Volume", 1),   "Osc 2 Volume",     nrf(0.0f, 1.0f, 0.00001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterInt>(pid("osc2CoarseRandom", 1), "Osc 2 Coarse Random", 0, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2FineRandom", 1), "Osc 2 Fine Random", nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc2LevelRandom", 1), "Osc 2 Level Random", nrf(0.0f, 1.0f, 0.00001f, 0.3f), 0.0f),
    
    std::make_unique<juce::AudioParameterFloat>(pid("osc3Attack", 1),   "Osc 3 Attack",     nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.01f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc3Decay", 1),    "Osc 3 Decay",      nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.2f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc3Sustain", 1),  "Osc 3 Sustain",    nrf(0.0f, 1.0f, 0.00001f), 1.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc3Release", 1),  "Osc 3 Release",    nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.1f),
    std::make_unique<juce::AudioParameterInt>(  pid("osc3Coarse", 1),   "Osc 3 Coarse",     1, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc3Fine", 1),     "Osc 3 Fine",       nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterBool>(pid("osc3Fixed", 1),     "Osc 3 Fixed", false),
    std::make_unique<juce::AudioParameterFloat>(pid("osc3Volume", 1),   "Osc 3 Volume",     nrf(0.0f, 1.0f, 0.00001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterInt>(pid("osc3CoarseRandom", 1), "Osc 3 Coarse Random", 0, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc3FineRandom", 1), "Osc 3 Fine Random", nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc3LevelRandom", 1), "Osc 3 Level Random", nrf(0.0f, 1.0f, 0.00001f, 0.3f), 0.0f),
    
    std::make_unique<juce::AudioParameterFloat>(pid("osc4Attack", 1),   "Osc 4 Attack",     nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.01f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc4Decay", 1),    "Osc 4 Decay",      nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.2f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc4Sustain", 1),  "Osc 4 Sustain",    nrf(0.0f, 1.0f, 0.00001f), 1.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc4Release", 1),  "Osc 4 Release",    nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.1f),
    std::make_unique<juce::AudioParameterInt>(  pid("osc4Coarse", 1),   "Osc 4 Coarse",     1, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc4Fine", 1),     "Osc 4 Fine",       nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterBool>(pid("osc4Fixed", 1),     "Osc 4 Fixed", false),
    std::make_unique<juce::AudioParameterFloat>(pid("osc4Volume", 1),   "Osc 4 Volume",     nrf(0.0f, 1.0f, 0.00001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterInt>(pid("osc4CoarseRandom", 1), "Osc 4 Coarse Random", 0, 30, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("osc4FineRandom", 1), "Osc 4 Fine Random", nrf(0.0f, 22000.0f, 0.001f, 0.3f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>(pid("osc4LevelRandom", 1), "Osc 4 Level Random", nrf(0.0f, 1.0f, 0.00001f, 0.3f), 0.0f),
    
    std::make_unique<juce::AudioParameterFloat>(pid("timeRandom", 1), "Time Random", nrf(0.0f, 50.0f, 0.00001f, 0.27f), 0.0f),
    std::make_unique<juce::AudioParameterInt>(  pid("algorithm", 1),    "Algorithm",        1, 11, 1),
    std::make_unique<juce::AudioParameterFloat>(pid("tone", 1),    "Tone",        nrf(1000.0f, 10000.0f, 0.01f, 0.27f), 5000.0f)
}),
audioEngine(parameters)
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
