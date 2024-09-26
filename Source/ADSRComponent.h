/*
  ==============================================================================

    ADSRComponent.h
    Created: 23 Sep 2024 4:51:06pm
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
public:
    ADSRComponent(juce::AudioProcessorValueTreeState &parameters, const juce::String &parameterPrefix);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    float a() { return attack.getValue(); }
    float d() { return decay.getValue(); }
    float s() { return sustain.getValue(); }
    float r() { return release.getValue(); }
    
    void a(float a) { attack.setValue(a); }
    void d(float d) { decay.setValue(d); }
    void s(float s) { sustain.setValue(s); }
    void r(float r) { release.setValue(r); }
    void adsr(const juce::ADSR::Parameters& params) { a(params.attack); d(params.decay); s(params.sustain); r(params.release); }
    
private:
    static constexpr int rotaryBoxWidth = 80;
    static constexpr int rotaryBoxHeight = 20;
    
    juce::Slider attack, decay, sustain, release;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
