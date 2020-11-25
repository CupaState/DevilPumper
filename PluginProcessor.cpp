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
    releaseTimeMax = 1000.0f;
    attackTimeMaximum = 80.0f;
    crestFactor = 2.0f;
    alphaAttack = 0.0f;
    alphaRelease = 0.0f;
    slope = 0.0f;

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
    int channels = round(numChannels / 2); // number of stereo channels
    float inSquare = 0.0f;
    slope = 1 / pRatio - 1;

    // create blank input buffer to add to
    AudioSampleBuffer inputBuffer(channels, bufferSize);
    inputBuffer.clear();

    for (int channel = 0; channel < channels; ++channel) //For each channel pair of channels
    {
        if (*pThreshold < 0)
        {
            // Mix down left-right to analyse the input
            inputBuffer.addFrom(channel, 0, buffer, channel * 2, 0, bufferSize, 0.5);
            inputBuffer.addFrom(channel, 0, buffer, channel * 2 + 1, 0, bufferSize, 0.5);

            for (int sample = 0; sample < bufferSize; ++sample)
            {
                //Level detection- estimate level using peak detector
                if (fabs(buffer.getWritePointer(channel)[sample]) < 0.000001)
                {
                    pInputGain = -120.0;
                }
                else
                {
                    pInputGain = 20 * log10(fabs(buffer.getWritePointer(channel)[sample]));
                }

                pAttackTime = 2 * attackTimeMaximum / Square(crestFactor);
                pReleaseTime = MAX(2 * releaseTimeMax / Square(crestFactor) - pAttackTime, 0.0f);

                // compression : calculates the control voltage
                alphaAttack = exp(-1 / (pSampleRate * pAttackTime));
                alphaRelease = exp(-1 / (pSampleRate * pReleaseTime));

                inSquare = MAX(Square(std::fabs(buffer.getWritePointer(channel)[sample])), MINVAL);
                //Calculate SQUARE of PEAK
                PEAK = MAX(Square(std::fabs(buffer.getWritePointer(channel)[sample])), Square(previousPEAK) + alphaAttack * (Square(std::fabs(buffer.getWritePointer(channel)[sample]))) - Square(previousPEAK));
                //Calculate SQUARE of RMS
                RMS = Square(previousRMS) + alphaAttack * (Square(std::fabs(buffer.getWritePointer(channel)[sample])) - Square(previousRMS));
                //Calculate CrestFactor
                crestFactor = std::sqrt(PEAK / RMS);
                
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
                previousPEAK = PEAK;
                previousRMS = RMS;

                // apply control voltage to both channels
                buffer.getWritePointer(2 * channel + 0)[sample] *= pControlVoltage;
                buffer.getWritePointer(2 * channel + 1)[sample] *= pControlVoltage;
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
    XmlElement xml("MYPLUGINSETTINGS");
    xml.setAttribute(THRESHOLD_ID, *pThreshold);
    for (int i = 0; i < getNumParameters(); ++i)
    {
        if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
            xml.setAttribute(p->paramID, p->getValue());
    }

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
