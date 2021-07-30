/*
  ==============================================================================

    CustomRotary.h
    Created: 18 Jul 2021 4:18:02pm
    Author:  leola

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomRotary : public juce::Slider

{
public:

    bool hitTest(int x, int y) override;
    juce::String labelName;
};