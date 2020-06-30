/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Compressor.h"

#define GAIN_ID "Gain_ID"
#define GAIN_NAME "GAIN_NAME"
#define ATTACK_ID "Attack_ID"
#define ATTACK_NAME "Attack_NAME"
#define RELEASE_ID "Release_ID"
#define RELEASE_NAME "Release_NAME"
#define THRESHOLD_ID "Threshold_ID"
#define THRESHOLD_NAME "Threshold_NAME"
#define RATIO_ID "Ratio_ID"
#define RATIO_NAME "Ratio_NAME"
#define KNEE_ID "Knee_ID"
#define KNEE_NAME "Knee_NAME"
#define MAKEUP_ID "MakeUp_ID"
#define MAKEUP_NAME "MakeUp_NAME"
#define PROCESSOR_ID "Pocessor_ID"
#define PROCESSOR_NAME "Processor_NAME"


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

    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

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

    void setMakeUpGain(float makeUpGain) { pMakeUpGain = Decibels::decibelsToGain(makeUpGain); }
    void setKneeWidth(float KneeWidth) { pKneeWidth = KneeWidth; }
    void setGain(float Gain) { pGain = Decibels::decibelsToGain(Gain); }
    void setThreshold(float Threshold) { pThreshold = Threshold; }
    void setRatio(float Ratio) { pRatio = Ratio; }
    void setAttack(float Attack) { pAttackTime = Attack; }
    void setRelease(float Release) { pReleaseTime = Release; }


    //==============================================================================
    // Getter Functions for each parameter

    float getMakeUpGain() { return Decibels::decibelsToGain(pMakeUpGain); }
    float getKneeWidth() { return pKneeWidth; }
    float getGain() { return Decibels::decibelsToGain(pGain); }
    float getThreshold() { return pThreshold; }
    float getRatio() { return pRatio; }
    float getAttack() { return pAttackTime; }
    float getReleaseTime() { return pReleaseTime; }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessor)

public:
    ScopedPointer <Compressor> processorComp;
    AudioProcessorValueTreeState treeState;
    AudioProcessorValueTreeState::ParameterLayout createParameter();

    float pAttackTime;
    float pReleaseTime;

    float pThreshold;
    float pRatio;
    float pKneeWidth;

    float pMakeUpGain;
    float pGain;

    int numChannels;
};
