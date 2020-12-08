/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>

#define THRESHOLD_ID "threshold_id"
#define THRESHOLD_NAME "threshold_name"

static const double MINVAL = 1.0e-6;
static const double MINATTACKTIME = 0.0001;
static const double MAXATTACKTIME = 0.2;
static const double MINRELEASETIME = 0.005;
static const double MAXRELEASETIME = 2.0;

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

    void setThreshold(float Threshold) { *pThreshold = Threshold; }

    //==============================================================================
    // Getter Functions for each parameter

    float getThreshold() { return *pThreshold; }

    juce::AudioProcessorValueTreeState parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameter();

    void saveToTxt(
        float attack,
        float release
    )
    {
        std::ofstream file;
        file.open("D:\\2.txt");
        file <<
            "Attack = " << attack * 1000 << "\n" <<
            "Release = " << release * 1000 << "\n";
        file.close();
    }

private:

    double param_to_log(double x, double mn, double mx)
    {
        return exp(log(mn) + x * (log(mx) - log(mn)));
    }

    float pSampleRate{ 44100.0f };
    float pInputGain;
    float pOutputGain;
    float pInputLevel;
    float pPreviousOutputLevel;
    float pOutputLevel;
    float pControlVoltage;

    float logInputGain;
    float logInputLevel;
    float logThreshold;
    float coeffThreshold;
    float logOutputGain;
    float logOutputLevel;

    float crestPeak, crestRMS, crestFactor, averageTime, inputSquare, alpha;

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
