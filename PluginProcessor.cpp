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
    pGain = 1.0f;
    pThreshold = parameters.getRawParameterValue(THRESHOLD_ID);
    pRatio = 80.0f;
    pAttackTime = 5.0f;
    pReleaseTime = 25.0f;
    pOverallGain = 1.0f;
    pKneeWidth = 5.0f;

    parameters.state = ValueTree("savedParameters");
}

DevilPumperInfinityAudioProcessor::~DevilPumperInfinityAudioProcessor()
{
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout DevilPumperInfinityAudioProcessor::createParameter()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    auto thresholdParam = std::make_unique<AudioParameterFloat>(THRESHOLD_ID, THRESHOLD_NAME, -50.0f, 0.0f, 0.0f);
    params.push_back(std::move(thresholdParam));

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

/*void DevilPumperInfinityAudioProcessor::getCurrentProgramStateInformation(MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement>xml(parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DevilPumperInfinityAudioProcessor::setCurrentProgramStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement>xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.state = juce::ValueTree::fromXml(*xmlState);
        }
    }
}*/

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
    gainInDecibels = Decibels::gainToDecibels(pGain);
    pSampleRate = getSampleRate();
    pPreviousOutputLevel = 0.0f;
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


void DevilPumperInfinityAudioProcessor::compressorMath(AudioSampleBuffer& buffer)
{
    int bufferSize = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels(); // number of channels
    int M = round(numChannels / 2); // number of stereo channels

    // create blank input buffer to add to
    AudioSampleBuffer inputBuffer(M, bufferSize);
    inputBuffer.clear();

    for (int m = 0; m < M; ++m) //For each channel pair of channels
    {
        if (*pThreshold < 0)
        {
            // Mix down left-right to analyse the input
            inputBuffer.addFrom(m, 0, buffer, m * 2, 0, bufferSize, 0.5);
            inputBuffer.addFrom(m, 0, buffer, m * 2 + 1, 0, bufferSize, 0.5);

            // compression : calculates the control voltage
            float alphaAttack = exp(-1 / (0.001 * pSampleRate * pAttackTime));
            float alphaRelease = exp(-1 / (0.001 * pSampleRate * pReleaseTime));

            for (int i = 0; i < bufferSize; ++i)
            {
                //Level detection- estimate level using peak detector
                if (fabs(buffer.getWritePointer(m)[i]) < 0.000001)
                {
                    pInputGain = -120.0;
                }
                else
                {
                    pInputGain = 20 * log10(fabs(buffer.getWritePointer(m)[i]));
                }

                // Gain computer - apply input/output curve with kneewidth
                // Incorporating the SOFT KNEE
                if (2 * (pInputGain - *pThreshold) >= pKneeWidth)
                {
                    // above knee
                    pOutputGain = *pThreshold + (pInputGain - *pThreshold) / pRatio;
                }
                else if (2 * fabs(pInputGain - *pThreshold) <= pKneeWidth)
                {
                    // in knee
                    pOutputGain = pInputGain + (1 / pRatio - 1) * pow(pInputGain - *pThreshold + pKneeWidth / 2, 2) / (2 * pKneeWidth);
                }
                else // below knee
                {
                    pOutputGain = pInputGain;
                }

                pInputLevel = pInputGain - pOutputGain;

                //Ballistics- smoothing of the gain
                if (pInputLevel > pPreviousOutputLevel)
                {
                    pOutputLevel = alphaAttack * pPreviousOutputLevel + (1 - alphaAttack) * pInputLevel;
                }
                else
                {
                    pOutputLevel = alphaRelease * pPreviousOutputLevel + (1 - alphaRelease) * pInputLevel;
                }

                //find control voltage
                pControlVoltage = pow(10, (gainInDecibels - pOutputLevel) / 20);
                pPreviousOutputLevel = pOutputLevel;

                // apply control voltage to both channels
                buffer.getWritePointer(2 * m + 0)[i] *= pControlVoltage;
                buffer.getWritePointer(2 * m + 1)[i] *= pControlVoltage;
            }
        }
        else // if threshold = 0, still apply make up gain.
        {
            buffer.applyGain(pow(10, (gainInDecibels) / 20));
        }
    }
}


void DevilPumperInfinityAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    const int numSamples = buffer.getNumSamples();

    playHead = this->getPlayHead();
    if (playHead != nullptr && playHead -> getCurrentPosition(currentPositionInfo))
    {
        bpm = currentPositionInfo.bpm;
    }

    // Initialise Buffer

    AudioSampleBuffer Output;

    // Set buffer to an input

    Output.makeCopyOf(buffer);

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    //compression

    compressorMath(Output);

    // Sum Each Band

    buffer.clear();

    for (int channel = 0; channel < totalNumInputChannels; channel++)
    {
        buffer.addFrom(channel, 0, Output, channel, 0, numSamples, 1.0);
    }

    buffer.applyGain(pOverallGain);
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
    saveToTxt(bpm, *pThreshold, pSampleRate, pInputGain, pOutputGain, pInputLevel, pPreviousOutputLevel, pOutputLevel, pControlVoltage, gainInDecibels, pKneeWidth, pGain, pOverallGain);
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
