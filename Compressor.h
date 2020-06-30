/*
  ==============================================================================

    Compressor.h
    Created: 26 Jun 2020 6:37:59pm
    Author:  olegi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#ifndef Compressor_h
#define Compressor_h
class Compressor
{
public:
    Compressor();
    ~Compressor();

    void prepareToPlay(double samplerate, int samplesPerBlock, int numInputChannels);
    void processBlock(AudioSampleBuffer& buffer);
    void setParameters(float ratio, float threshold, float attack, float release, float makeUpGain, float kneeWidth);

private:
    float mAttackTime{ 5.0 };
    float mReleaseTime{ 25.0 };

    float mThreshold{ -3.0 };
    float mRatio{ 1.0 };
    float mKneeWidth{ 5.0 };

    float mInputGain;
    float mOutputGain;
    float mMakeUpGain;
    float mInputLevel;
    float mOutputLevel;
    float mPreviousOutputLevel;

    float mControlVoltage;
    float mSampleRate;
};

#endif//