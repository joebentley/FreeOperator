/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 13 Oct 2024 9:59:55am
    Author:  Joe Bentley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MyLookAndFeel : public juce::LookAndFeel_V4
{
    void drawComboBox (juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox& box) override;
    void positionComboBoxText (juce::ComboBox& box, juce::Label& label) override;
    juce::PopupMenu::Options getOptionsForComboBoxPopupMenu (juce::ComboBox& box, juce::Label& label) override;
};
