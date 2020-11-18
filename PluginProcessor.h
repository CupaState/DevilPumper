/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>

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

    void compressorMath(AudioSampleBuffer& buffer);
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
    //void getCurrentProgramStateInformation(MemoryBlock& destData) override;
    //void setCurrentProgramStateInformation(const void* data, int sizeInBytes) override;

    // Setter Functions for each parameter

    void setOverallGain(float OverallGain) { pOverallGain = OverallGain; }
    void setKneeWidth(float KneeWidth) { pKneeWidth = KneeWidth; }
    void setGain(float Gain) { pGain = Gain; }
    void setThreshold(float Threshold) { *pThreshold = Threshold; }
    void setRatio(float Ratio) { pRatio = Ratio; }
    void setAttack(float Attack) { pAttackTime = Attack; }
    void setRelease(float Release) { pReleaseTime = Release; }

    //==============================================================================
    // Getter Functions for each parameter

    float getOverallGain() { return Decibels::decibelsToGain(pOverallGain); }
    float getKneeWidth() { return pKneeWidth; }
    float getGain() { return Decibels::decibelsToGain(pGain); }
    float getThreshold() { return *pThreshold; }
    float getRatio() { return pRatio; }
    float getAttack() { return pAttackTime; }
    float getReleaseTime() { return pReleaseTime; }

    juce::AudioProcessorValueTreeState parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameter();

    void saveToTxt(
        float bpm, 
        float threshold, 
        float sampleRate, 
        float inputGain, 
        float outputGain, 
        float inputLevel, 
        float previousOutputLevel,
        float outputLevel,
        float controlVoltage,
        float gainInDecibels,
        float knee,
        float gain,
        float overallGain
    )
    {
        std::string strBPM = std::to_string(bpm);
        std::string strThreshold = std::to_string(threshold);
        std::string strSampleRate = std::to_string(sampleRate);
        std::string strInputGain = std::to_string(inputGain);
        std::string strOutputGain = std::to_string(outputGain);
        std::string strInputLevel = std::to_string(inputLevel);
        std::string strPreviousOutputLevel = std::to_string(previousOutputLevel);
        std::string strOutputLevel = std::to_string(outputLevel);
        std::string strControlVoltage = std::to_string(controlVoltage);
        std::string strGainInDecibels = std::to_string(gainInDecibels);
        std::string strKnee = std::to_string(knee);
        std::string strGain = std::to_string(gain);
        std::string strOverallGain = std::to_string(overallGain);

        std::ofstream file;
        file.open("D:\\2.txt");
        file <<
            "bpm = " << strBPM << "\n" << 
            "threshold = " << strThreshold << "\n" <<
            "SampleRate = " << strSampleRate << "\n" <<
            "Input Gain = " << strInputGain << "\n" <<
            "Output Gain = " << strOutputGain << "\n" <<
            "Input Level = " << strInputLevel << "\n" <<
            "Previous Output Level = " << strPreviousOutputLevel << "\n" <<
            "Output Level = " << strOutputLevel << "\n" <<
            "Control Voltage = " << strControlVoltage << "\n" <<
            "Gain in Decibels = " << strGainInDecibels << "\n" <<
            "Knee Width = " << strKnee << "\n" <<
            "Gain = " << strGain << "\n" <<
            "Overall Gain = " << strOverallGain << "\n";
        file.close();
    }

private:

    AudioPlayHead* playHead;
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    float bpm { 130.0f };

    float pSampleRate{ 44100.0f};
    float pInputGain;
    float pOutputGain;
    float pInputLevel;
    float pPreviousOutputLevel;
    float pOutputLevel;
    float pControlVoltage;
    float gainInDecibels;

    float pAttackTime;
    float pReleaseTime;

    std::atomic<float>* pThreshold = nullptr;
    float pRatio;
    float pKneeWidth;

    float pOverallGain;
    float pGain;

    int numChannels;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessor)
};
