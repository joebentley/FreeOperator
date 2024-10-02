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
    parameters.addParameterListener("tone", this);
    
    osc1Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc1Volume"));
    osc2Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc2Volume"));
    osc3Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc3Volume"));
    osc4Volume = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc4Volume"));
    osc1Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc1Coarse"));
    osc2Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc2Coarse"));
    osc3Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc3Coarse"));
    osc4Coarse = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc4Coarse"));
    osc1Fine = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc1Fine"));
    osc2Fine = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc2Fine"));
    osc3Fine = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc3Fine"));
    osc4Fine = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc4Fine"));
    
    osc1CoarseRandom = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc1CoarseRandom"));
    osc1LevelRandom = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc1LevelRandom"));
    osc2CoarseRandom = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc2CoarseRandom"));
    osc2LevelRandom = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc2LevelRandom"));
    osc3CoarseRandom = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc3CoarseRandom"));
    osc3LevelRandom = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc3LevelRandom"));
    osc4CoarseRandom = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc4CoarseRandom"));
    osc4LevelRandom = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc4LevelRandom"));
    
    parameters.addParameterListener("algorithm", this);
    algorithm = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("algorithm"))->get();
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
    
    auto filterCoefficients = juce::IIRCoefficients::makeLowPass(spec.sampleRate, dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("tone"))->get());
    osc1.setFilterCoefficients(filterCoefficients);
    osc2.setFilterCoefficients(filterCoefficients);
    osc3.setFilterCoefficients(filterCoefficients);
    osc4.setFilterCoefficients(filterCoefficients);
    
    osc1.reset();
    osc2.reset();
    osc3.reset();
    osc4.reset();
    
    tempBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
    
    sampleRate = spec.sampleRate;
}

void Voice::noteStarted()
{
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    
    auto coarseRange = osc1CoarseRandom->get();
    if (coarseRange > 0)
        osc1.setFrequency((osc1Coarse->get() + random.nextInt(coarseRange)) * freqHz + osc1Fine->get());
    else
        osc1.setFrequency(osc1Coarse->get() * freqHz + osc1Fine->get());
    
    coarseRange = osc2CoarseRandom->get();
    if (coarseRange > 0)
        osc2.setFrequency((osc2Coarse->get() + random.nextInt(coarseRange)) * freqHz + osc2Fine->get());
    else
        osc2.setFrequency(osc2Coarse->get() * freqHz + osc2Fine->get());
    
    coarseRange = osc3CoarseRandom->get();
    if (coarseRange > 0)
        osc3.setFrequency((osc3Coarse->get() + random.nextInt(coarseRange)) * freqHz + osc3Fine->get());
    else
        osc3.setFrequency(osc3Coarse->get() * freqHz + osc3Fine->get());
    
    coarseRange = osc4CoarseRandom->get();
    if (coarseRange > 0)
        osc4.setFrequency((osc4Coarse->get() + random.nextInt(coarseRange)) * freqHz + osc4Fine->get());
    else
        osc4.setFrequency(osc4Coarse->get() * freqHz + osc4Fine->get());
    
    osc1.setAmplitudeOffset(random.nextFloat() * osc1LevelRandom->get());
    osc2.setAmplitudeOffset(random.nextFloat() * osc2LevelRandom->get());
    osc3.setAmplitudeOffset(random.nextFloat() * osc3LevelRandom->get());
    osc4.setAmplitudeOffset(random.nextFloat() * osc4LevelRandom->get());
    
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
    adsrOsc3.noteOff();
    adsrOsc4.noteOff();
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
    else if (parameterID == "tone") {
        auto filterCoefficients = juce::IIRCoefficients::makeLowPass(sampleRate, dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("tone"))->get());
        osc1.setFilterCoefficients(filterCoefficients);
        osc2.setFilterCoefficients(filterCoefficients);
        osc3.setFilterCoefficients(filterCoefficients);
        osc4.setFilterCoefficients(filterCoefficients);
    }
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
    
    juce::dsp::AudioBlock<float>(outputBuffer)
        .getSubBlock((size_t)startSample, (size_t)numSamples)
        .add(audioBlock);
}

float Voice::renderSampleForAlgorithm()
{
    jassert(algorithm > 0);
    jassert(algorithm < 12);
    
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
            sample *= adsrOsc1.getNextSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 2: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(0);
            auto osc3Sample = osc3.processSample();
            osc3Sample *= adsrOsc3.getNextSample();
            
            osc2.setPhaseOffset(osc4Sample + osc3Sample);
            auto osc2Sample = osc2.processSample();
            osc2Sample *= adsrOsc2.getNextSample();
            osc1.setPhaseOffset(osc2Sample);
            
            sample = osc1.processSample();
            sample *= adsrOsc1.getNextSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 3: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(0);
            auto osc3Sample = osc3.processSample();
            osc3Sample *= adsrOsc3.getNextSample();
            
            osc2.setPhaseOffset(osc3Sample);
            auto osc2Sample = osc2.processSample();
            osc2Sample *= adsrOsc2.getNextSample();
            
            osc1.setPhaseOffset(osc2Sample + osc4Sample);
            
            sample = osc1.processSample();
            sample *= adsrOsc1.getNextSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 4: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(osc4Sample);
            auto osc3Sample = osc3.processSample();
            osc3Sample *= adsrOsc3.getNextSample();
            
            osc2.setPhaseOffset(osc4Sample);
            auto osc2Sample = osc2.processSample();
            osc2Sample *= adsrOsc2.getNextSample();
            
            osc1.setPhaseOffset(osc3Sample + osc2Sample);
            
            sample = osc1.processSample();
            sample *= adsrOsc1.getNextSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 5: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(osc4Sample);
            auto osc3Sample = osc3.processSample();
            osc3Sample *= adsrOsc3.getNextSample();
            
            osc1.setPhaseOffset(osc3Sample);
            osc2.setPhaseOffset(osc3Sample);
            
            sample = adsrOsc1.getNextSample() * osc1.processSample() + adsrOsc2.getNextSample() * osc2.processSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 6: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(osc4Sample);
            auto osc3Sample = osc3.processSample();
            osc3Sample *= adsrOsc3.getNextSample();
            
            osc2.setPhaseOffset(osc3Sample);
            osc1.setPhaseOffset(0);
            sample = adsrOsc1.getNextSample() * osc1.processSample() + adsrOsc2.getNextSample() * osc2.processSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 7: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(0);
            auto osc3Sample = osc3.processSample();
            osc3Sample *= adsrOsc3.getNextSample();
            
            osc2.setPhaseOffset(0);
            auto osc2Sample = osc2.processSample();
            osc2Sample *= adsrOsc2.getNextSample();
            
            osc1.setPhaseOffset(osc2Sample + osc3Sample + osc4Sample);
            sample = adsrOsc1.getNextSample() * osc1.processSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 8: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(osc4Sample);
            
            osc2.setPhaseOffset(0);
            auto osc2Sample = osc2.processSample();
            osc2Sample *= adsrOsc2.getNextSample();
            
            osc1.setPhaseOffset(osc2Sample);
            sample = adsrOsc1.getNextSample() * osc1.processSample() + adsrOsc3.getNextSample() * osc3.processSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 9: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(osc4Sample);
            osc2.setPhaseOffset(osc4Sample);
            osc1.setPhaseOffset(osc4Sample);
            
            sample = adsrOsc1.getNextSample() * osc1.processSample() + adsrOsc2.getNextSample() * osc2.processSample() + adsrOsc3.getNextSample() * osc3.processSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 10: {
            auto osc4Sample = osc4.processSample();
            osc4Sample *= adsrOsc4.getNextSample();
            
            osc3.setPhaseOffset(osc4Sample);
            osc2.setPhaseOffset(0);
            osc1.setPhaseOffset(0);
            
            sample = adsrOsc1.getNextSample() * osc1.processSample() + adsrOsc2.getNextSample() * osc2.processSample() + adsrOsc3.getNextSample() * osc3.processSample();
            sample *= noteVelocity * masterGain;
            break;
        }
        case 11: {
            osc3.setPhaseOffset(0);
            osc2.setPhaseOffset(0);
            osc1.setPhaseOffset(0);
            
            sample = adsrOsc1.getNextSample() * osc1.processSample() + adsrOsc2.getNextSample() * osc2.processSample() + adsrOsc3.getNextSample() * osc3.processSample() + adsrOsc4.getNextSample() * osc4.processSample();
            sample *= noteVelocity * masterGain;
            break;
        }
    }
    
    return sample;
}
