/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DevilPumperInfinityAudioProcessor::DevilPumperInfinityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr, juce::Identifier("parameters"), createParameter())
#endif
{
    pGain = parameters.getRawParameterValue(GAIN_ID);
    pThreshold = parameters.getRawParameterValue(THRESHOLD_ID);
    pRatio = parameters.getRawParameterValue(RATIO_ID);
    pAttackTime = parameters.getRawParameterValue(ATTACK_ID);
    pReleaseTime = parameters.getRawParameterValue(RELEASE_ID);
    pOverallGain = parameters.getRawParameterValue(OVERALL_GAIN_ID);
    pKneeWidth = parameters.getRawParameterValue(KNEE_ID);

    ON_OFF = 1;

    processorComp = new Compressor;

    parameters.state = ValueTree("savedParameters");
}

DevilPumperInfinityAudioProcessor::~DevilPumperInfinityAudioProcessor()
{
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout DevilPumperInfinityAudioProcessor::createParameter()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    auto attackParam = std::make_unique<AudioParameterFloat>(ATTACK_ID, ATTACK_NAME, 1.0f, 250.0f, 5.0f);
    params.push_back(std::move(attackParam));

    auto releaseParam = std::make_unique<AudioParameterFloat>(RELEASE_ID, RELEASE_NAME, 1.0f, 2500.0f, 25.0f);
    params.push_back(std::move(releaseParam));

    auto thresholdParam = std::make_unique<AudioParameterFloat>(THRESHOLD_ID, THRESHOLD_NAME, -50.0f, 0.0f, 0.0f);
    params.push_back(std::move(thresholdParam));

    auto ratioParam = std::make_unique<AudioParameterFloat>(RATIO_ID, RATIO_NAME, 1.0f, 100.0f, 1.0f);
    params.push_back(std::move(ratioParam));

    auto kneeParam = std::make_unique<AudioParameterFloat>(KNEE_ID, KNEE_NAME, 0.0f, 10.0f, 5.0f);
    params.push_back(std::move(kneeParam));

    auto gainParam = std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, -48.0f, 10.0f, 0.0f);
    params.push_back(std::move(gainParam));

    auto overallGainParam = std::make_unique<AudioParameterFloat>(OVERALL_GAIN_ID, OVERALL_GAIN_NAME, -48.0f, 10.0f, 0.0f);
    params.push_back(std::move(overallGainParam));

    return { params.begin(), params.end() };
}
const String DevilPumperInfinityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DevilPumperInfinityAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DevilPumperInfinityAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DevilPumperInfinityAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DevilPumperInfinityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DevilPumperInfinityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DevilPumperInfinityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DevilPumperInfinityAudioProcessor::setCurrentProgram(int index)
{
}

const String DevilPumperInfinityAudioProcessor::getProgramName(int index)
{
    return {};
}

void DevilPumperInfinityAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void DevilPumperInfinityAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    numChannels = getTotalNumInputChannels();

    (*processorComp).prepareToPlay(sampleRate, samplesPerBlock, getTotalNumInputChannels());
    (*processorComp).setParameters(*pRatio, *pThreshold, *pAttackTime, *pReleaseTime, *pGain, *pKneeWidth);
}

void DevilPumperInfinityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DevilPumperInfinityAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void DevilPumperInfinityAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    const int numSamples = buffer.getNumSamples();

    // Initialise Buffer

    AudioSampleBuffer Output;

    // Set buffer to an input

    Output.makeCopyOf(buffer);
    
    float SampleRate = getSampleRate();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Set the Compressor Parameters
    (*processorComp).setParameters(*pRatio, *pThreshold, *pAttackTime, *pReleaseTime, *pGain, *pKneeWidth);

    //compression

    (*processorComp).processBlock(Output);

    // Sum Each Band

    buffer.clear();

    for (int channel = 0; channel < totalNumInputChannels; channel++)
    {
        buffer.addFrom(channel, 0, Output, channel, 0, numSamples, 1.0);
    }

    buffer.applyGain(*pOverallGain);
}

//==============================================================================
bool DevilPumperInfinityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DevilPumperInfinityAudioProcessor::createEditor()
{
    return new DevilPumperInfinityAudioProcessorEditor(*this);
}

//==============================================================================
void DevilPumperInfinityAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement>xml(parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DevilPumperInfinityAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement>xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.state = juce::ValueTree::fromXml(*xmlState);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DevilPumperInfinityAudioProcessor();
}
