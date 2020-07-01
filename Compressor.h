/*
  ==============================================================================

    Compressor.h
    Created: 26 Jun 2020 6:37:59pm
    Author:  olegi

  ==============================================================================
*/

#pragma once



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

private:
    float mAttackTime;
    float mReleaseTime;
    float mThreshold;
    float mRatio;
    float mKneeWidth;
    float mSampleRate;
    float mMakeUpGain;

    // Compressor ON-OFF state
    int compressorState = 1;

    float mInputGain;
    float mOutputGain;
    float mInputLevel;
    float mOutputLevel;

    float mPreviousOutputLevel;
    float mControlVoltage;

};

#endif//Compressor_h 