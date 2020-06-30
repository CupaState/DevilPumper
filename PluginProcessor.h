/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Compressor.h"


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

    float pAttackTime;
    float pReleaseTime;

    float pThreshold;
    float pRatio;
    float pKneeWidth;

    float pMakeUpGain;
    float pGain;

    int numChannels;
};
