/*
  ==============================================================================

    CustomRotary.cpp
    Created: 18 Jul 2021 4:18:02pm
    Author:  leola

  ==============================================================================
*/

#include "CustomRotary.h"


bool CustomRotary::hitTest(int x, int y)
{
    auto H = getHeight();
    auto W = getWidth();

    auto centre_x = x - W / 2;
    auto centre_y = y - H / 2;

    auto dist = sqrt(centre_x * centre_x + centre_y * centre_y);
    return (dist < juce::jmax(H,W)/2);
}
