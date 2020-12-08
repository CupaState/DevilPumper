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
    pAttackTime = 0.030f;
    pReleaseTime = 0.0f;
    pOverallGain = 1.0f;
    pKneeWidth = 2.0f;
    logOutputGain = 0.0f;
    logOutputLevel = 0.0f;
    crestPeak = MINVAL;
    crestRMS = MINVAL;
    crestFactor = 0.0f;

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
    pSampleRate = getSampleRate();
    pPreviousOutputLevel = 0.0f;
    logInputLevel = 0.0f;
    logThreshold = 0.0f;
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

            float alpha_attack = 0.0f;
            float alpha_release = 0.0f;

            for (int sample = 0; sample < bufferSize; ++sample)
            {
                averageTime = (MAXATTACKTIME + MAXRELEASETIME) / 2.0;
                alpha = 1.0 - std::exp(-1.0 / (pSampleRate * averageTime));
                inputSquare = std::pow(std::fabs(buffer.getWritePointer(m)[sample]), 2.0);

                crestPeak = std::max(inputSquare, crestPeak + alpha * (inputSquare - crestPeak));
                crestRMS = crestRMS + alpha * (inputSquare - crestRMS);
                crestFactor = std::log(std::sqrt(crestPeak / crestRMS));

                pAttackTime = (2 * MAXATTACKTIME / crestFactor) / 10.0;
                pReleaseTime = std::max((2 * MAXRELEASETIME / crestFactor - pAttackTime) / 20.0, 0.0);

                alpha_attack = 1.0 - std::exp(-1.0 / (pSampleRate * pAttackTime));
                alpha_release = 1.0 - std::exp(-1.0 / (pSampleRate * pReleaseTime));

                if (std::fabs(buffer.getWritePointer(m)[sample]) < 0.000001)
                {
                    logInputGain = std::log(std::pow(10.0, -120.0f / 20.0));
                }
                else
                {
                    logInputGain = std::log(std::fabs(buffer.getWritePointer(m)[sample]));
                }

                logThreshold = std::log(std::pow(10, *pThreshold / 20.0));

                if (logInputGain > logThreshold)
                {
                    logOutputGain = logThreshold + (logInputGain - logThreshold) / pRatio;
                }
                else
                {
                    logOutputGain = logInputGain;
                }

                logInputLevel = logInputGain - logOutputGain;

                if (logInputLevel > pPreviousOutputLevel)
                {
                    logOutputLevel = pPreviousOutputLevel + alpha_attack * (logInputLevel - pPreviousOutputLevel);
                }
                else
                {
                    logOutputLevel = pPreviousOutputLevel + alpha_release * (logInputLevel - pPreviousOutputLevel);
                }

                pPreviousOutputLevel = logOutputLevel;

                pControlVoltage = std::exp(-logOutputLevel);

                buffer.getWritePointer(2 * m + 0)[sample] *= pControlVoltage;
                buffer.getWritePointer(2 * m + 1)[sample] *= pControlVoltage;
            }
        }
        else // if threshold = 0, still apply make up gain.
        {
            buffer.applyGain(pGain);
        }
    }
}


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
    XmlElement xml("MYPLUGINSETTINGS");
    xml.setAttribute(THRESHOLD_ID, *pThreshold);
    for (int i = 0; i < getNumParameters(); ++i)
    {
        if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
            xml.setAttribute(p->paramID, p->getValue());
    }

    saveToTxt(pAttackTime, pReleaseTime);

    copyXmlToBinary(xml, destData);
}

void DevilPumperInfinityAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our last window size..
            *pThreshold = xmlState->getIntAttribute(THRESHOLD_ID, *pThreshold);

            // Now reload our parameters..
            for (int i = 0; i < getNumParameters(); ++i)
                if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
                    p->setValue((float)xmlState->getDoubleAttribute(p->paramID, p->getValue()));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DevilPumperInfinityAudioProcessor();
}
