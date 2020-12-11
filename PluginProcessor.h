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

    // Setter Functions for each parameter

    void setThreshold(float threshold) { *pThreshold = threshold; }
    float setOverallGain(float threshold) { return 1.0 + (1.0 - std::pow(10.0, threshold / 20.0));}

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
    float cv_estimate, cv_dev, alpha_time_for_cv, alpha_for_cv;
    float range{ -40.0f };
    float logRange{ std::log(pow(10.0f, range / 20.0f)) };
    float cv_dev_prev = 0.1 ;

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
