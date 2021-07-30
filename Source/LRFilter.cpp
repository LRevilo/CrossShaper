/*
  ==============================================================================

    LRFilter.cpp
    Created: 26 Jul 2021 4:42:27pm
    Author:  leola

  ==============================================================================
*/

#include "LRFilter.h"

LRFilter::LRFilter()
{
    update();
}

float LRFilter::tanpi(float x)
{
    //fast tan(pi*x) for 0 <= x <= 1/2

    const float a0 = 3.139557626f;
    const float a1 = -5.1164610208f;
    const float a2 = 2.23292204224f;

    float x2 = x * x;
    float u = x - 0.5f;
    float u2 = u * u;

    float q = -x * (a0 + x2 * (a1 + x2 * a2));
    float p = u * (a0 + u2 * (a1 + u2 * a2));

    return  q / p;
}

void LRFilter::setCutoffFrequency(float newCutoffFrequencyHz)
{

    cutoffFrequency = newCutoffFrequencyHz;
    update();
}

void LRFilter::reset()
{
    for (auto s : { &s1, &s2, &s3, &s4 })
        std::fill(s->begin(), s->end(), 0.f);
}

void LRFilter::update()
{
    g = tanpi(cutoffFrequency / sampleRate);
    h = (float)(1.0 / (1.0 + R2 * g + g * g));

}

void LRFilter::prepare(const int& inNumChannels, const int& inSampleRate)
{
    jassert(inSampleRate > 0);
    jassert(inNumChannels > 0);

    sampleRate = inSampleRate;
    update();

    s1.resize(inNumChannels);
    s2.resize(inNumChannels);
    s3.resize(inNumChannels);
    s4.resize(inNumChannels);

    reset();
}

void LRFilter::processSample(int channel, float inputValue, float& outputLow, float& outputHigh)
{
    auto yH = (inputValue - (R2 + g) * s1[(size_t)channel] - s2[(size_t)channel]) * h;

    auto yB = g * yH + s1[(size_t)channel];
    s1[(size_t)channel] = g * yH + yB;

    auto yL = g * yB + s2[(size_t)channel];
    s2[(size_t)channel] = g * yB + yL;

    auto yH2 = (yL - (R2 + g) * s3[(size_t)channel] - s4[(size_t)channel]) * h;

    auto yB2 = g * yH2 + s3[(size_t)channel];
    s3[(size_t)channel] = g * yH2 + yB2;

    auto yL2 = g * yB2 + s4[(size_t)channel];
    s4[(size_t)channel] = g * yB2 + yL2;

    outputLow = yL2;
    outputHigh = yL - R2 * yB + yH - yL2;
}

void LRFilter::processBuffer(std::atomic<float>* crossOverFreq, juce::AudioBuffer<float> inBuffer, juce::AudioBuffer<float>& outLowBuffer, juce::AudioBuffer<float>& outHighBuffer) noexcept
{
    const auto numChannels = inBuffer.getNumChannels();
    const auto numSamples = inBuffer.getNumSamples();

    jassert(inBuffer.getNumChannels() <= s1.size());
    jassert(inBuffer.getNumChannels() == numChannels);
    jassert(inBuffer.getNumSamples() == numSamples);


    for (size_t i = 0; i < numSamples; ++i)
    {

        float f = (1 - 1.f / 256) * prevCutoff + (1.f / 256) * *crossOverFreq;
        setCutoffFrequency(f);
        prevCutoff = f;
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples = inBuffer.getReadPointer(channel);
            auto* outputLowSamples = outLowBuffer.getWritePointer(channel);
            auto* outputHighSamples = outHighBuffer.getWritePointer(channel);
            processSample((int)channel, inputSamples[i], outputLowSamples[i], outputHighSamples[i]);
        }
    }

}