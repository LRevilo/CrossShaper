/*
  ==============================================================================

    LRFilter.h
    Created: 26 Jul 2021 4:42:27pm
    Author:  leola

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include <math.h>

class LRFilter
{
public:


    LRFilter();


    float tanpi(float x);

    /** Sets the cutoff frequency of the filter in Hz. */
    void setCutoffFrequency(float newCutoffFrequencyHz);

    //==============================================================================


    /** Returns the cutoff frequency of the filter. */
    float getCutoffFrequency() const noexcept { return cutoffFrequency; }

    //==============================================================================
    /** Initialises the filter. */
    void prepare(const int& inNumChannels, const int& inSampleRate);

    /** Resets the internal state variables of the filter. */
    void reset();

    void processSample(int channel, float inputValue, float& outputLow, float& outputHigh);
    void processBuffer(std::atomic<float>* crossOverFreq, juce::AudioBuffer<float> inBuffer, juce::AudioBuffer<float>& outLowBuffer, juce::AudioBuffer<float>& outHighBuffer) noexcept;

private:
    //==============================================================================
    void update();

    //==============================================================================
    float g;
    float h;
    float R2 = 1.414213562373f;
    std::vector<float> s1, s2, s3, s4;

    double sampleRate = 48000.0;
    float cutoffFrequency = 300.0;
    float prevCutoff = 300.f;
    //==============================================================================
    /** Processes the input and output samples supplied in the processing context. */
};