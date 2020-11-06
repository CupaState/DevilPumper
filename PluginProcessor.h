/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Compressor.h"

#define ATTACK_ID "attack_id"
#define RELEASE_ID "release_id"
#define THRESHOLD_ID "threshold_id"
#define RATIO_ID "ratio_id"
#define KNEE_ID "knee_id"
#define GAIN_ID "gain_id"
#define OVERALL_GAIN_ID "overall_gain_id"

#define ATTACK_NAME "attack_name"
#define RELEASE_NAME "release_name"
#define THRESHOLD_NAME "threshold_name"
#define RATIO_NAME "ratio_name"
#define KNEE_NAME "knee_name"
#define GAIN_NAME "gain_name"
#define OVERALL_GAIN_NAME "overall_gain_name"

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

    void setOverallGain(float OverallGain) { *pOverallGain = OverallGain; }
    void setKneeWidth(float KneeWidth) { *pKneeWidth = KneeWidth; }
    void setGain(float Gain) { *pGain = Gain; }
    void setThreshold(float Threshold) { *pThreshold = Threshold; }
    void setRatio(float Ratio) { *pRatio = Ratio; }
    void setAttack(float Attack) { *pAttackTime = Attack; }
    void setRelease(float Release) { *pReleaseTime = Release; }

    void setCompressorState(int ON_OFF) { this->ON_OFF = ON_OFF; }

    //==============================================================================
    // Getter Functions for each parameter

    float getOverallGain() { return decibelsToGain(pOverallGain); }
    float getKneeWidth() { return *pKneeWidth; }
    float getGain() { return decibelsToGain(pGain); }
    float getThreshold() { return *pThreshold; }
    float getRatio() { return *pRatio; }
    float getAttack() { return *pAttackTime; }
    float getReleaseTime() { return *pReleaseTime; }

    float getCompressorState() { return ON_OFF; }

    float decibelsToGain(std::atomic<float>* decibels)
    {
        return std::pow(10.0, *decibels * 0.05);
    }

    juce::AudioProcessorValueTreeState parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameter();

private:

    ScopedPointer<Compressor> processorComp;

    std::atomic<float>* pAttackTime;
    std::atomic<float>* pReleaseTime;

    std::atomic<float>* pThreshold;
    std::atomic<float>* pRatio;
    std::atomic<float>* pKneeWidth;

    std::atomic<float>* pOverallGain;
    std::atomic<float>* pGain;

    int numChannels;

    //Compressor States
    int ON_OFF;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessor)
};
