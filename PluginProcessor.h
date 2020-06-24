/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessor)

public:

    AudioProcessorValueTreeState treeState;
    AudioProcessorValueTreeState::ParameterLayout createParameter();

    double mAttackTime{ 5.0 };
    double mReleaseTime{ 25.0 };

    double mThreshold{ 0.0 };
    double mRatio{ 0.0 };

    double mOutputGain{ 0 };
    double mSampleRate{ 44100 };
};
