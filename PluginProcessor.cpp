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
    treeState(*this, nullptr, "Parameter", createParameter())
#endif
{
    treeState.state = ValueTree("savedParameters");
}

AudioProcessorValueTreeState::ParameterLayout DevilPumperInfinityAudioProcessor::createParameter()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameter;
    auto gainParameter = std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, -50.0, 10.0, 0.0);
    parameter.push_back(std::move(gainParameter));

    auto attackParameter = std::make_unique<AudioParameterFloat>(ATTACK_ID, ATTACK_NAME, 1.0, 250.0, 5.0);
    parameter.push_back(std::move(attackParameter));

    auto releaseParameter = std::make_unique<AudioParameterFloat>(RELEASE_ID, RELEASE_NAME, 1.0, 2500.0, 25.0);
    parameter.push_back(std::move(releaseParameter));

    auto thresholdParameter = std::make_unique<AudioParameterFloat>(THRESHOLD_ID, THRESHOLD_NAME, -50.0, 0.0, 0.0);
    parameter.push_back(std::move(thresholdParameter));

    auto ratioParameter = std::make_unique<AudioParameterFloat>(RATIO_ID, RATIO_NAME, 0.0, 100.0, 0.0);
    parameter.push_back(std::move(ratioParameter));

    auto kneeParameter = std::make_unique<AudioParameterFloat>(KNEE_ID, KNEE_NAME, 0.0, 72.0, 5.0);
    parameter.push_back(std::move(kneeParameter));

    auto makeUpParameter = std::make_unique<AudioParameterFloat>(MAKEUP_ID, MAKEUP_NAME, -50.0, 0.0, 0.0);
    parameter.push_back(std::move(makeUpParameter));

    return { parameter.begin(), parameter.end() };
}

DevilPumperInfinityAudioProcessor::~DevilPumperInfinityAudioProcessor()
{
}

//==============================================================================
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

    pAttackTime = *treeState.getRawParameterValue(ATTACK_ID);

    pReleaseTime = *treeState.getRawParameterValue(RELEASE_ID);

    pThreshold = *treeState.getRawParameterValue(THRESHOLD_ID);

    pRatio = *treeState.getRawParameterValue(RATIO_ID);

    pKneeWidth = *treeState.getRawParameterValue(KNEE_ID);

    pGain = *treeState.getRawParameterValue(GAIN_ID);

    pMakeUpGain = *treeState.getRawParameterValue(MAKEUP_ID);
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

void DevilPumperInfinityAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();

    // Initialise Buffer

    AudioSampleBuffer Output;

    // Set buffer to an input

    Output.makeCopyOf(buffer);
    
    float SampleRate = getSampleRate();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Set the Compressor Parameters
    (*processorComp).setParameters(pRatio, pThreshold, pAttackTime, pReleaseTime, pMakeUpGain, pKneeWidth);

    //compression

    (*processorComp).processBlock(Output);

    // Sum Each Band

    buffer.clear();

    for (int channel = 0; channel < totalNumInputChannels; channel++)
    {
        buffer.addFrom(channel, 0, Output, channel, 0, numSamples);
    }

    buffer.applyGain(pGain);
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
    std::unique_ptr<XmlElement>xml(treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DevilPumperInfinityAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement>xmlParameter(getXmlFromBinary(data, sizeInBytes));
    if (xmlParameter != nullptr)
    {
        if (xmlParameter->hasTagName(treeState.state.getType()))
        {
            treeState.state = ValueTree::fromXml(*xmlParameter);
        }
    }

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DevilPumperInfinityAudioProcessor();
}
