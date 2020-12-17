#pragma once

#include <JuceHeader.h>

#define THRESHOLD_ID "threshold_id"
#define THRESHOLD_NAME "threshold_name"
#define SWITCHER_ID "switcher_id"
#define SWITCHER_NAME "switcher_name"

static const float MINVAL = 0.000001f;
static const float MAXATTACKTIME = 0.2f;
static const float MAXRELEASETIME = 2.0f;

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

    void setThreshold(float threshold_UI) { *threshold = threshold_UI; }
    float setOverallGain(float threshold) { return 1.0 - std::pow(10.0, threshold / 20.0); }
    void setMode(int mode_UI) { mode = mode_UI; }

    //==============================================================================
    // Getter Functions for each parameter

    float getThreshold() { return *threshold; }
    int getMode() { return mode; }

    juce::AudioProcessorValueTreeState parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameter();

private:

    float sample_rate;
    float input_gain;
    float output_gain;
    float input_level;
    float control_voltage;

    float log_input_gain;
    float log_input_level;
    float log_threhsold;
    float log_output_gain;
    float log_output_level;

    float crest_PEAK, crest_RMS, crest_factor, average_time, alpha_average, input_square;
    float cv_estimate, cv_dev, alpha_for_cv;
    float log_range;
    float cv_dev_const;

    float attack_time;
    float release_time;

    std::atomic<float>* threshold = nullptr;
    float ratio;

    int mode;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessor)
};
