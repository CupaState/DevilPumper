/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Compressor.h"


#define ATTACK_ID "attack_ID"
#define ATTACK_NAME "attack_NAME"
#define RELEASE_ID "release_ID"
#define RELEASE_NAME "release_NAME"
#define THRESHOLD_ID "threshold_ID"
#define THRESHOLD_NAME "threshold_NAME"
#define RATIO_ID "ratio_ID"
#define RATIO_NAME "ratio_NAME"
#define KNEE_ID "knee_ID"
#define KNEE_NAME "knee_NAME"
#define OVERALLGAIN_ID "overallgain_ID"
#define OVERALLGAIN_NAME "overallgain_NAME"
#define GAIN_ID "gain_ID"
#define GAIN_NAME "gain_NAME"

//==============================================================================
/**
*/
class DevilPumperInfinityAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    DevilPumperInfinityAudioProcessor();
    ~DevilPumperInfinityAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Setter Functions for each parameter

    void setOverallGain(float OverallGain) { this->pOverallGain = Decibels::decibelsToGain(OverallGain); }
    void setKneeWidth(float KneeWidth) { this->pKneeWidth = KneeWidth; }
    void setGain(float Gain) { this->pGain = Decibels::decibelsToGain(Gain); }
    void setThreshold(float Threshold) { this->pThreshold = Threshold; }
    void setRatio(float Ratio) { this->pRatio = Ratio; }
    void setAttack(float Attack) { this->pAttackTime = Attack; }
    void setRelease(float Release) { this->pReleaseTime = Release; }

    void setCompressorState(bool analog)
    {
        if (analog)
        {
            (*processorComp).mTimeConstant = Compressor::TimeConstant::Analog;
        }
        else
        {
            (*processorComp).mTimeConstant = Compressor::TimeConstant::Digital;
        }
    }

    //==============================================================================
    // Getter Functions for each parameter

    float getOverallGain() { return Decibels::decibelsToGain(pOverallGain); }
    float getKneeWidth() { return pKneeWidth; }
    float getGain() { return Decibels::decibelsToGain(pGain); }
    float getThreshold() { return pThreshold; }
    float getRatio() { return pRatio; }
    float getAttack() { return pAttackTime; }
    float getReleaseTime() { return pReleaseTime; }

private:

    ScopedPointer <Compressor> processorComp;

    float pAttackTime;
    float pReleaseTime;

    float pThreshold;
    float pRatio;
    float pKneeWidth;

    float pOverallGain;
    float pGain;

    int numChannels;

    //Compressor States
    int ON_OFF;

public:
    AudioProcessorValueTreeState parameter;
    AudioProcessorValueTreeState::ParameterLayout createParameter();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessor)
};