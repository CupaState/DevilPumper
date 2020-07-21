/*
  ==============================================================================

    Compressor.cpp
    Created: 26 Jun 2020 6:37:59pm
    Author:  olegi

  ==============================================================================
*/

#include "Compressor.h"
#include <stdio.h>

Compressor::Compressor()
{
}

Compressor::~Compressor()
{

}

void Compressor::processBlock(AudioSampleBuffer& buffer)
{
    int bufferSize = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels(); // number of channels
    int M = round(numChannels / 2); // number of stereo channels

    // create blank input buffer to add to
    AudioSampleBuffer inputBuffer(M, bufferSize);
    inputBuffer.clear();


    for (int m = 0; m < M; ++m) //For each channel pair of channels
    {
        if (mThreshold < 0)
        {
            // Mix down left-right to analyse the input
            inputBuffer.addFrom(m, 0, buffer, m * 2, 0, bufferSize, 0.5);
            inputBuffer.addFrom(m, 0, buffer, m * 2 + 1, 0, bufferSize, 0.5);

            float alphaAttack;
            float alphaRelease;

            // compression : calculates the control voltage
            if (m_uTimeConstant == Analog)
            {
                alphaAttack = exp(ANALOG_TC / (0.001 * mSampleRate * mAttackTime));
                alphaRelease = exp(ANALOG_TC / (0.001 * mSampleRate * mReleaseTime));
            }
            else
            {
                alphaAttack = exp(DIGITAL_TC / (0.001 * mSampleRate * mAttackTime));
                alphaRelease = exp(DIGITAL_TC / (0.001 * mSampleRate * mReleaseTime));
            }


            for (int i = 0; i < bufferSize; ++i)
            {
                //Level detection- estimate level using peak detector
                if (fabs(buffer.getWritePointer(m)[i]) < 0.000001)
                {
                    mInputGain = -120.0;
                }
                else
                {
                    mInputGain = 20 * log10(fabs(buffer.getWritePointer(m)[i]));
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
                mControlVoltage = pow(10, (mMakeUpGain - mOutputLevel) / 20);
                mPreviousOutputLevel = mOutputLevel;

                // apply control voltage to both channels
                buffer.getWritePointer(2 * m + 0)[i] *= mControlVoltage;
                buffer.getWritePointer(2 * m + 1)[i] *= mControlVoltage;
            }
        }
        else // if threshold = 0, still apply make up gain.
        {
            buffer.applyGain(pow(10, (mMakeUpGain) / 20));
        }
    }
}

void Compressor::setParameters(float ratio, float threshold, float attack, float release, float makeUpGain, float kneeWidth, float outputGain)
{
    this->mRatio = ratio;
    this->mThreshold = threshold;
    this->mAttackTime = attack;
    this->mReleaseTime = release;
    this->mMakeUpGain = Decibels::gainToDecibels(makeUpGain);
    this->mKneeWidth = kneeWidth;
    this->mOutputGain = outputGain;
}

void Compressor::prepareToPlay(double samplerate)
{
    mSampleRate = samplerate;

    mPreviousOutputLevel = 0;
}

