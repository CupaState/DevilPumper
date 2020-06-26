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
    float mRatio{ 0.0 };
    float mKneeWidth{ 5.0 };

    float mInputGain{ 0.0 };
    float mOutputGain{ 0.0 };
    float mMakeUpGain{ 0.0 };
    float mInputLevel{ 0.0 };
    float mOutputLevel{ 0.0 };
    float mPreviousOutputLevel{ 0.0 };

    float mControlVoltage{ 0.0 };
    float mSampleRate{ 44100 };
};

#endif//