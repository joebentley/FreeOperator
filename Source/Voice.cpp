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
    for (int i = 0; i < 4; ++i) {
        adsrParameters[i].attack    = parameters.getParameterAsValue("osc" + juce::String(i+1) + "Attack").getValue();
        adsrParameters[i].decay     = parameters.getParameterAsValue("osc" + juce::String(i+1) + "Decay").getValue();
        adsrParameters[i].sustain   = parameters.getParameterAsValue("osc" + juce::String(i+1) + "Sustain").getValue();
        adsrParameters[i].release   = parameters.getParameterAsValue("osc" + juce::String(i+1) + "Release").getValue();
    }
    
    for (int i = 0; i < 4; ++i) {
        parameters.addParameterListener("osc" + juce::String(i+1) + "Attack", this);
        parameters.addParameterListener("osc" + juce::String(i+1) + "Decay", this);
        parameters.addParameterListener("osc" + juce::String(i+1) + "Sustain", this);
        parameters.addParameterListener("osc" + juce::String(i+1) + "Release", this);
    }
    
    parameters.addParameterListener("tone", this);
    
    for (int i = 0; i < 4; i++) {
        oscVolume[i] = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc" + juce::String(i+1) + "Volume"));
        oscCoarse[i] = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc" + juce::String(i+1) + "Coarse"));
        oscFine[i] = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc" + juce::String(i+1) + "Fine"));
        oscFixed[i] = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("osc" + juce::String(i+1) + "Fixed"));
        oscCoarseRandom[i] = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("osc" + juce::String(i+1) + "CoarseRandom"));
        oscFineRandom[i] = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc" + juce::String(i+1) + "FineRandom"));
        oscLevelRandom[i] = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("osc" + juce::String(i+1) + "LevelRandom"));
    }
    
    timeRandom = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("timeRandom"));
    
    parameters.addParameterListener("algorithm", this);
    algorithm = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("algorithm"))->get();
}

Voice::~Voice()
{
    for (int i = 0; i < 4; ++i) {
        parameters.addParameterListener("osc" + juce::String(i+1) + "Attack", this);
        parameters.addParameterListener("osc" + juce::String(i+1) + "Decay", this);
        parameters.addParameterListener("osc" + juce::String(i+1) + "Sustain", this);
        parameters.addParameterListener("osc" + juce::String(i+1) + "Release", this);
    }
    
    parameters.removeParameterListener("tone", this);
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
    for (int i = 0; i < 4; ++i) {
        adsrOsc[i].setSampleRate(spec.sampleRate);
        osc[i].setSampleRate((float)spec.sampleRate);
        
        auto filterCoefficients = juce::IIRCoefficients::makeLowPass(spec.sampleRate, dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("tone"))->get());
        osc[i].setFilterCoefficients(filterCoefficients);
        osc[i].reset();
    }
    
    tempBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
    
    sampleRate = spec.sampleRate;
}

void Voice::noteStarted()
{
    bool useSequence = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("randomRepeat"))->get();
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    
    for (int i = 0; i < 4; ++i) {
        osc[i].setFrequency(0);
        
        int coarseRange;
        juce::String sequenceParameterID = "sequenceOsc" + juce::String(i+1) + "Coarse";
        
        if (!oscFixed[i]->get()) {
            if (useSequence) {
                auto coarseOffset = getCurrentFromSequenceAndIncrement(sequenceParameterID);
                osc[i].setFrequency((oscCoarse[i]->get() + coarseOffset) * freqHz);
            } else {
                coarseRange = oscCoarseRandom[i]->get();
                if (coarseRange > 0) {
                    auto generated = random.nextInt(coarseRange);
                    pushOntoSequence(sequenceParameterID, generated);
                    osc[i].setFrequency((oscCoarse[i]->get() + random.nextInt(coarseRange)) * freqHz);
                } else
                    osc[i].setFrequency(oscCoarse[i]->get() * freqHz + oscFine[i]->get());
            }
        }
        osc[i].setFrequency(osc[i].getFrequency() + oscFine[i]->get());
        
        auto fineRange = oscFineRandom[i]->get();
        if (fineRange > 0.00001)
            osc[i].setFrequency(osc[i].getFrequency() + random.nextFloat() * fineRange);
        
        osc[i].setAmplitudeOffset(random.nextFloat() * oscLevelRandom[i]->get());
    }
    
    noteVelocity = velocity;
    
    juce::ADSR::Parameters adsrParametersCopy[4];
    
    for (int i = 0; i < 4; ++i)
        adsrParametersCopy[i] = adsrParameters[i];
    
    if (timeRandom->get() > 0.000001f) {
        auto decayOffset = random.nextFloat() * timeRandom->get();
        for (int i = 0; i < 4; ++i)
            adsrParametersCopy[i].decay += decayOffset;
    }
    
    for (int i = 0; i < 4; ++i)
        adsrOsc[i].setParameters(adsrParametersCopy[i]);
    
    for (int i = 0; i < 4; ++i)
        adsrOsc[i].noteOn();
}

void Voice::noteStopped(bool)
{
    for (int i = 0; i < 4; ++i)
        adsrOsc[i].noteOff();
}

void Voice::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "osc1Attack")
        adsrParameters[0].attack = newValue;
    else if (parameterID == "osc1Decay")
        adsrParameters[0].decay = newValue;
    else if (parameterID == "osc1Sustain")
        adsrParameters[0].sustain = newValue;
    else if (parameterID == "osc1Release")
        adsrParameters[0].release = newValue;
    else if (parameterID == "osc2Attack")
        adsrParameters[1].attack = newValue;
    else if (parameterID == "osc2Decay")
        adsrParameters[1].decay = newValue;
    else if (parameterID == "osc2Sustain")
        adsrParameters[1].sustain = newValue;
    else if (parameterID == "osc2Release")
        adsrParameters[1].release = newValue;
    else if (parameterID == "osc3Attack")
        adsrParameters[2].attack = newValue;
    else if (parameterID == "osc3Decay")
        adsrParameters[2].decay = newValue;
    else if (parameterID == "osc3Sustain")
        adsrParameters[2].sustain = newValue;
    else if (parameterID == "osc3Release")
        adsrParameters[2].release = newValue;
    else if (parameterID == "osc4Attack")
        adsrParameters[3].attack = newValue;
    else if (parameterID == "osc4Decay")
        adsrParameters[3].decay = newValue;
    else if (parameterID == "osc4Sustain")
        adsrParameters[3].sustain = newValue;
    else if (parameterID == "osc4Release")
        adsrParameters[3].release = newValue;
    else if (parameterID == "algorithm")
        algorithm = dynamic_cast<juce::AudioParameterInt*>(parameters.getParameter("algorithm"))->get();
    else if (parameterID == "tone") {
        auto filterCoefficients = juce::IIRCoefficients::makeLowPass(sampleRate, dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("tone"))->get());
        for (int i = 0; i < 4; ++i)
            osc[i].setFilterCoefficients(filterCoefficients);
    }
}

void Voice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    if (isActive() && adsrOsc[0].isActive() == false) {
//        DBG("Main ADSR end");
        for (int i = 0; i < 4; ++i)
            adsrOsc[i].setParameters(adsrParameters[i]);
        clearCurrentNote();
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        osc[i].setAmplitude(oscVolume[i]->get());
    }
    
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
    
    auto &osc1 = osc[0];
    auto &osc2 = osc[1];
    auto &osc3 = osc[2];
    auto &osc4 = osc[3];
    auto &adsrOsc1 = adsrOsc[0];
    auto &adsrOsc2 = adsrOsc[1];
    auto &adsrOsc3 = adsrOsc[2];
    auto &adsrOsc4 = adsrOsc[3];
    
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

void Voice::pushOntoSequence(const juce::String& parameterID, float valueToPush)
{
    auto var = parameters.state.getProperty(parameterID);
    juce::Array<juce::var> *asArray = var.getArray();
    asArray->remove(0);
    asArray->add(valueToPush);
//    parameters.state.setProperty(parameterID, *asArray, nullptr);
}

float Voice::getCurrentFromSequenceAndIncrement(const juce::String& parameterID)
{
    auto var = parameters.state.getProperty(parameterID);
    juce::Array<juce::var> *asArray = var.getArray();
    int index = parameters.state.getProperty(parameterID + "Index");
    parameters.state.setProperty(parameterID + "Index", (index + 1) % asArray->size(), nullptr);
    return asArray->operator[](index);
}
