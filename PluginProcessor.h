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

    void setOverallGain(float OverallGain) { pOverallGain = Decibels::decibelsToGain(OverallGain); }
    void setKneeWidth(float KneeWidth) { pKneeWidth = KneeWidth; }
    void setGain(float Gain) { pGain = Decibels::decibelsToGain(Gain); }
    void setThreshold(float Threshold) { pThreshold = Threshold; }
    void setRatio(float Ratio) { pRatio = Ratio; }
    void setAttack(float Attack) { pAttackTime = Attack; }
    void setRelease(float Release) { pReleaseTime = Release; }

    void setCompressorState(int Analog) {
        if (Analog != 0)
        {
            (*processorComp).m_uTimeConstant = Compressor::TimeConstant::Analog;
        }
        else if(Analog == 0)
        {
            (*processorComp).m_uTimeConstant = Compressor::TimeConstant::Digital;
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

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessor)
};
