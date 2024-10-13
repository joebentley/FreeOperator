/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 13 Oct 2024 9:59:55am
    Author:  Joe Bentley

  ==============================================================================
*/

#include "MyLookAndFeel.h"

using namespace juce;

void MyLookAndFeel::drawComboBox (Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox& box)
{
    auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    Rectangle<int> boxBounds (0, 0, width, height);

    g.setColour (box.findColour (ComboBox::backgroundColourId));
    g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

    g.setColour (box.findColour (ComboBox::outlineColourId));
    g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

    Rectangle<int> arrowZone (width - 15, 8, 8, height - 13);
    Path path;
    path.startNewSubPath ((float) arrowZone.getX(), (float) arrowZone.getY());
    path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getBottom());
    path.lineTo ((float) arrowZone.getRight(), (float) arrowZone.getY());

    g.setColour (box.findColour (ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.fillPath (path);
}

void MyLookAndFeel::positionComboBoxText (ComboBox& box, Label& label)
{
    label.setBounds (1, 1, box.getWidth() - 8, box.getHeight() - 2);
    label.setFont (getComboBoxFont (box));
}

PopupMenu::Options MyLookAndFeel::getOptionsForComboBoxPopupMenu (ComboBox& box, Label& label)
{
    return PopupMenu::Options().withTargetComponent (&box)
                               .withItemThatMustBeVisible (box.getSelectedId())
                               .withInitiallySelectedItem (box.getSelectedId())
                               .withMinimumWidth (10)
                               .withMaximumNumColumns (1)
                               .withStandardItemHeight (label.getHeight())
                               .withTargetScreenArea(Rectangle<int> {box.getScreenX(), box.getScreenY(), 10, box.getHeight()});
}
