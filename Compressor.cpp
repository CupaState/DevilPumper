/*
  ==============================================================================

    Compressor.cpp
    Created: 26 Jun 2020 6:37:59pm
    Author:  olegi

  ==============================================================================
*/

#include "Compressor.h"
//#include <stdio.h>

Compressor::Compressor()
{

}

Compressor::~Compressor()
{

}

void Compressor::processBlock(AudioSampleBuffer& buffer)
{
    int bufferSize = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    int MonoChannel = round(numChannels / 2); // number of stereo channels

    // create blank input buffer to add to
    AudioSampleBuffer inputBuffer(MonoChannel, bufferSize);
    inputBuffer.clear();


    for (int channel = 0; channel < MonoChannel; ++channel) //For each channel pair of channels
    {
        if (mThreshold < 0)
        {
            // Mix down left-right to analyse the input
            inputBuffer.addFrom(channel, 0, buffer, channel * 2.0, bufferSize, 0.5);
            inputBuffer.addFrom(channel, 0, buffer, channel * 2.0 + 1.0, bufferSize, 0.5);


            // compression : calculates the control voltage
            float alphaAttack = exp(-1 / (0.001 * mSampleRate * mAttackTime));
            float alphaRelease = exp(-1 / (0.001 * mSampleRate * mReleaseTime));

            for (int sample = 0; sample < bufferSize; ++sample)
            {
                //Level detection- estimate level using peak detector
                if (fabs(buffer.getWritePointer(channel)[sample]) < 0.000001)
                {
                    mInputGain = -120.0;
                }
                else
                {
                    mInputGain = 20 * log10(fabs(buffer.getWritePointer(channel)[sample]));
                }

                // Gain computer - apply input/output curve with kneewidth
                   // Incorporating the SOFT KNEE
                if (2 * (mInputGain - mThreshold) >= mKneeWidth)
                {
                    // above knee
                    mOutputGain = mThreshold + (mInputGain - mThreshold) / mRatio;
                }
                else if (2 * fabs(mInputGain - mThreshold) <= mKneeWidth)
                {
                    // in knee
                    mOutputGain = mInputGain + (1 / mRatio - 1) * pow(mInputGain - mThreshold + mKneeWidth / 2, 2) / (2 * mKneeWidth);
                }
                else // below knee
                {
                    mOutputGain = mInputGain;
                }
                mInputLevel = mInputGain - mOutputGain;

                //Ballistics- smoothing of the gain
                if (mInputLevel > mPreviousOutputLevel)
                {
                    mOutputLevel = alphaAttack * mPreviousOutputLevel + (1 - alphaAttack) * mInputLevel;
                }
                else
                {
                    mOutputLevel = alphaRelease * mPreviousOutputLevel + (1 - alphaRelease) * mInputLevel;
                }

                //find control voltage
                mControlVoltage = pow(10, (mMakeUpGain - mOutputLevel) / 20.0);
                mPreviousOutputLevel = mOutputLevel;

                // apply control voltage to both channels
                buffer.getWritePointer(2 * channel + 0)[sample] *= mControlVoltage;
                buffer.getWritePointer(2 * channel + 1)[sample] *= mControlVoltage;
            }
        }
        else // if threshold = 0, still apply make up gain.
        {
            buffer.applyGain(pow(10, (mMakeUpGain) / 20.0));
        }

    }
}

void Compressor::setParameters(float ratio, float threshold, float attack, float release, float makeUpGain, float kneeWidth)
{
    mRatio = ratio;
    mThreshold = threshold;
    mAttackTime = attack;
    mReleaseTime = release;
    mMakeUpGain = Decibels::gainToDecibels(makeUpGain);
    mKneeWidth = kneeWidth;
}

void Compressor::prepareToPlay(double samplerate, int samplesPerBlock, int numInputChannels)
{
    mSampleRate = samplerate;
}
