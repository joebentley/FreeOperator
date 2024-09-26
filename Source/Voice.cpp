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
    adsrParameters1.attack  = parameters.getParameterAsValue("osc1Attack").getValue();
    adsrParameters1.decay   = parameters.getParameterAsValue("osc1Decay").getValue();
    adsrParameters1.sustain = parameters.getParameterAsValue("osc1Sustain").getValue();
    adsrParameters1.release = parameters.getParameterAsValue("osc1Release").getValue();
    adsrParameters2.attack  = parameters.getParameterAsValue("osc2Attack").getValue();
    adsrParameters2.decay   = parameters.getParameterAsValue("osc2Decay").getValue();
    adsrParameters2.sustain = parameters.getParameterAsValue("osc2Sustain").getValue();
    adsrParameters2.release = parameters.getParameterAsValue("osc2Release").getValue();
    adsrParameters3.attack  = parameters.getParameterAsValue("osc3Attack").getValue();
    adsrParameters3.decay   = parameters.getParameterAsValue("osc3Decay").getValue();
    adsrParameters3.sustain = parameters.getParameterAsValue("osc3Sustain").getValue();
    adsrParameters3.release = parameters.getParameterAsValue("osc3Release").getValue();
    adsrParameters4.attack  = parameters.getParameterAsValue("osc4Attack").getValue();
    adsrParameters4.decay   = parameters.getParameterAsValue("osc4Decay").getValue();
    adsrParameters4.sustain = parameters.getParameterAsValue("osc4Sustain").getValue();
    adsrParameters4.release = parameters.getParameterAsValue("osc4Release").getValue();
    
    parameters.addParameterListener("osc1Attack", this);
    parameters.addParameterListener("osc1Decay", this);
    parameters.addParameterListener("osc1Sustain", this);
    parameters.addParameterListener("osc1Release", this);
    parameters.addParameterListener("osc2Attack", this);
    parameters.addParameterListener("osc2Decay", this);
    parameters.addParameterListener("osc2Sustain", this);
    parameters.addParameterListener("osc2Release", this);
    parameters.addParameterListener("osc3Attack", this);
    parameters.addParameterListener("osc3Decay", this);
    parameters.addParameterListener("osc3Sustain", this);
    parameters.addParameterListener("osc3Release", this);
    parameters.addParameterListener("osc4Attack", this);
    parameters.addParameterListener("osc4Decay", this);
    parameters.addParameterListener("osc4Sustain", this);
    parameters.addParameterListener("osc4Release", this);
    
    osc1Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc1Volume"));
    osc2Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc2Volume"));
    osc3Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc3Volume"));
    osc4Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc4Volume"));
    osc1Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc1Coarse"));
    osc2Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc2Coarse"));
    osc3Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc3Coarse"));
    osc4Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc4Coarse"));
    
    
    parameters.addParameterListener("algorithm", this);
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
    adsrOsc1.setSampleRate(spec.sampleRate);
    adsrOsc2.setSampleRate(spec.sampleRate);
    adsrOsc3.setSampleRate(spec.sampleRate);
    adsrOsc4.setSampleRate(spec.sampleRate);

    osc1.setSampleRate((float)spec.sampleRate);
    osc2.setSampleRate((float)spec.sampleRate);
    osc3.setSampleRate((float)spec.sampleRate);
    osc4.setSampleRate((float)spec.sampleRate);
    
    tempBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
}

void Voice::noteStarted()
{
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    
    osc1.setFrequency(osc1Coarse->get() * freqHz);
    osc2.setFrequency(osc2Coarse->get() * freqHz);
    osc3.setFrequency(osc3Coarse->get() * freqHz);
    osc4.setFrequency(osc4Coarse->get() * freqHz);
    
    noteVelocity = velocity;
    
    adsrOsc1.setParameters(adsrParameters1);
    adsrOsc1.noteOn();
    adsrOsc2.setParameters(adsrParameters2);
    adsrOsc2.noteOn();
    adsrOsc3.setParameters(adsrParameters3);
    adsrOsc3.noteOn();
    adsrOsc4.setParameters(adsrParameters4);
    adsrOsc4.noteOn();
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
    else if (parameterID == "osc3Attack")
        adsrParameters3.attack = newValue;
    else if (parameterID == "osc3Decay")
        adsrParameters3.decay = newValue;
    else if (parameterID == "osc3Sustain")
        adsrParameters3.sustain = newValue;
    else if (parameterID == "osc3Release")
        adsrParameters3.release = newValue;
    else if (parameterID == "osc4Attack")
        adsrParameters4.attack = newValue;
    else if (parameterID == "osc4Decay")
        adsrParameters4.decay = newValue;
    else if (parameterID == "osc4Sustain")
        adsrParameters4.sustain = newValue;
    else if (parameterID == "osc4Release")
        adsrParameters4.release = newValue;
    else if (parameterID == "algorithm")
        algorithm = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("algorithm"))->get();
}

void Voice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    if (isActive() && adsrOsc1.isActive() == false) {
//        DBG("Main ADSR end");
        adsrOsc1.setParameters(adsrParameters1);
        adsrOsc2.setParameters(adsrParameters2);
        adsrOsc3.setParameters(adsrParameters3);
        adsrOsc4.setParameters(adsrParameters4);
        clearCurrentNote();
        return;
    }
    
    osc1.setAmplitude(osc1Volume->get());
    osc2.setAmplitude(osc2Volume->get());
    osc3.setAmplitude(osc3Volume->get());
    osc4.setAmplitude(osc4Volume->get());
    
    auto audioBlock = juce::dsp::AudioBlock<float>(tempBuffer).getSubBlock(startSample, numSamples);
    audioBlock.clear();
    
    for (int s = 0; s < numSamples; ++s) {
        auto sample = renderSampleForAlgorithm();
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

float Voice::renderSampleForAlgorithm()
{
    jassert(algorithm > 0);
    jassert(algorithm < 11);
    
    float sample = 0.0;
    
    // Ableton Operator algos
    switch(algorithm) {
        case 1: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            osc3.setPhaseOffset(osc4Sample);
            auto osc3Sample = osc3.processSample();
            osc3Sample *= adsrOsc3.getNextSample();
            osc2.setPhaseOffset(osc3Sample);
            auto osc2Sample = osc2.processSample();
            osc2Sample *= adsrOsc2.getNextSample();
            osc1.setPhaseOffset(osc2Sample);
            
            sample = osc1.processSample();
            
            sample *= noteVelocity * masterGain;
            break;
        }
    }
    
    return sample;
}
