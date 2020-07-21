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
#include "pluginconstants.h"

class Compressor
{
public:
    Compressor();
    ~Compressor();

    void prepareToPlay(double samplerate);
    void processBlock(AudioSampleBuffer& buffer);
    void setParameters(float ratio, float threshold, float attack, float release, float makeUpGain, float kneeWidth, float outputGain);

    enum TimeConstant
    {
        Digital,
        Analog,
    };

    TimeConstant m_uTimeConstant;

    //void processAudioFrame(float* inputBuffer, float* outputBuffer, int uNumInputChannels, int uNumOutputChannels);

private:

    float mAttackTime;
    float mReleaseTime;
    float mThreshold;
    float mRatio;
    float mKneeWidth;
    float mSampleRate;
    float mMakeUpGain;

    float mInputGain;
    float mOutputGain;
    float mInputLevel;
    float mOutputLevel;

    float mPreviousOutputLevel;
    float mControlVoltage;
};

#endif//Compressor_h 