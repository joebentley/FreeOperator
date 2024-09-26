/*
  ==============================================================================

    Voice.cpp
    Created: 19 Sep 2024 9:15:20pm
    Author:  Joe Bentley

  ==============================================================================
*/

#include "Voice.h"

Voice::Voice(juce::AudioProcessorValueTreeState &parametersToUse) : parameters(parametersToUse)
{
    adsrParameters1.attack = parameters.getParameterAsValue("osc1Attack").getValue();
    adsrParameters1.decay = parameters.getParameterAsValue("osc1Decay").getValue();
    adsrParameters1.sustain = parameters.getParameterAsValue("osc1Sustain").getValue();
    adsrParameters1.release = parameters.getParameterAsValue("osc1Release").getValue();
    adsrParameters2.attack = parameters.getParameterAsValue("osc2Attack").getValue();
    adsrParameters2.decay = parameters.getParameterAsValue("osc2Decay").getValue();
    adsrParameters2.sustain = parameters.getParameterAsValue("osc2Sustain").getValue();
    adsrParameters2.release = parameters.getParameterAsValue("osc2Release").getValue();
    
    parameters.addParameterListener("osc1Attack", this);
    parameters.addParameterListener("osc1Decay", this);
    parameters.addParameterListener("osc1Sustain", this);
    parameters.addParameterListener("osc1Release", this);
    parameters.addParameterListener("osc2Attack", this);
    parameters.addParameterListener("osc2Decay", this);
    parameters.addParameterListener("osc2Sustain", this);
    parameters.addParameterListener("osc2Release", this);
    
    fmDepth = static_cast<juce::AudioParameterFloat*>(parameters.getParameter("fmDepth"));
    coarseDetuningOsc1 = static_cast<juce::AudioParameterInt*>(parameters.getParameter("osc1Coarse"));
    coarseDetuningOsc2 = static_cast<juce::AudioParameterInt*>(parameters.getParameter("osc2Coarse"));
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
    adsrOsc1.setSampleRate(spec.sampleRate);
    adsrOsc2.setSampleRate(spec.sampleRate);
    
    tempBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
    
    carrier.setSampleRate((float)spec.sampleRate);
    modulator.setSampleRate((float)spec.sampleRate);
//    processorChain.prepare(spec);
}

void Voice::noteStarted()
{
//    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    
    carrier.setFrequency(coarseDetuningOsc1->get() * freqHz);
    modulator.setFrequency(coarseDetuningOsc2->get() * freqHz);
    
//    processorChain.get<oscIndex>().setFrequency (freqHz, true);
//    processorChain.get<oscIndex>().setLevel (velocity);
    
    adsrOsc1.setParameters(adsrParameters1);
    adsrOsc1.noteOn();
    adsrOsc2.setParameters(adsrParameters2);
    adsrOsc2.noteOn();
}

void Voice::noteStopped(bool)
{
    adsrOsc1.noteOff();
    adsrOsc2.noteOff();
}

void Voice::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "osc1Attack")
        adsrParameters1.attack = newValue;
    else if (parameterID == "osc1Decay")
        adsrParameters1.decay = newValue;
    else if (parameterID == "osc1Sustain")
        adsrParameters1.sustain = newValue;
    else if (parameterID == "osc1Release")
        adsrParameters1.release = newValue;
    else if (parameterID == "osc2Attack")
        adsrParameters2.attack = newValue;
    else if (parameterID == "osc2Decay")
        adsrParameters2.decay = newValue;
    else if (parameterID == "osc2Sustain")
        adsrParameters2.sustain = newValue;
    else if (parameterID == "osc2Release")
        adsrParameters2.release = newValue;
}

void Voice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    if (isActive() && adsrOsc1.isActive() == false) {
        DBG("Main ADSR end");
        adsrOsc1.setParameters(adsrParameters1);
        adsrOsc2.setParameters(adsrParameters2);
        clearCurrentNote();
        return;
    }
    
    auto fmDepthValue = fmDepth->get();
    
    auto audioBlock = juce::dsp::AudioBlock<float>(tempBuffer).getSubBlock(startSample, numSamples);
    audioBlock.clear();
    
    for (int s = 0; s < numSamples; ++s) {
        auto modulation = modulator.processSample();
        
        modulation *= adsrOsc2.getNextSample();

        carrier.setPhaseOffset(fmDepthValue * modulation);
        
        auto sample = carrier.processSample();
        
        sample *= masterGain;
        
        audioBlock.setSample(0, s, sample);
        audioBlock.setSample(1, s, sample);
    }
    
//    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
//    processorChain.process(context);
    
    adsrOsc1.applyEnvelopeToBuffer(tempBuffer, startSample, numSamples);
    
    juce::dsp::AudioBlock<float>(outputBuffer)
        .getSubBlock((size_t)startSample, (size_t)numSamples)
        .add(audioBlock);
}

