/*
  ==============================================================================

    Compressor.h
    Created: 26 Jun 2020 6:37:59pm
    Author:  olegi

  ==============================================================================
*/

#pragma once

const float DIGITAL_TC = -2.0; // log(1%)
const float ANALOG_TC = -0.43533393574791066201247090699309; // (log(36.7%)

#ifndef Compressor_h
#define Compressor_h

#include <JuceHeader.h>


class Compressor
{
public:
    Compressor();
    ~Compressor();

    void prepareToPlay(double samplerate, int samplesPerBlock, int numInputChannels);
    void processBlock(AudioSampleBuffer& buffer);
    void setParameters(float ratio, float threshold, float attack, float release, float makeUpGain, float kneeWidth);

    enum class TimeConstant
    {
        Analog,
        Digital,
    };

    TimeConstant mTimeConstant;

private:
    float mAttackTime{ 5.0f };
    float mReleaseTime{ 25.0f };
    float mThreshold{ -3.0f };
    float mRatio{ 1.0f };
    float mKneeWidth{ 5.0f };
    float mSampleRate{ 44100.0f };
    float mMakeUpGain{ 0 };

    // Compressor ON-OFF state
    int compressorState = 1;

    float mInputGain{ 0 };
    float mOutputGain{ 0 };
    float mInputLevel{ 0 };
    float mOutputLevel{ 0 };

    float mPreviousOutputLevel{ 0 };
    float mControlVoltage{ 0 };

};

#endif//Compressor_h 