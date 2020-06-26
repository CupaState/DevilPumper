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
    auto gainParameter = std::make_unique<AudioParameterFloat>(MAKEUPGAIN_ID, MAKEUPGAIN_NAME, -50.0, 10.0, 0.0);
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
    mSampleRate = getSampleRate();

    mAttackTime = *treeState.getRawParameterValue(ATTACK_ID);

    //mMakeUpGain = pow(10, *treeState.getRawParameterValue(MAKEUPGAIN_ID) / 20);
    mMakeUpGain = *treeState.getRawParameterValue(MAKEUPGAIN_ID);

    mThreshold = *treeState.getRawParameterValue(THRESHOLD_ID);

    mRatio = *treeState.getRawParameterValue(RATIO_ID);

    mKneeWidth = *treeState.getRawParameterValue(KNEE_ID);
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
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    int bufferSize = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    int MonoChannel = round(numChannels / 2);


    AudioSampleBuffer inputBuffer(MonoChannel, bufferSize);
    //auto totalNumInputChannels = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    inputBuffer.clear();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    //float curGain = pow(10, *treeState.getRawParameterValue(GAIN_ID) / 20.0);

    //for (auto i = numChannels; i < numChannels; ++i)
    //    buffer.clear(i, 0, bufferSize);

    for (int channel = 0; channel < MonoChannel; ++channel)
    {
        if (mThreshold < 0)
        {
            inputBuffer.addFrom(channel, 0, buffer, channel * 2.0, bufferSize, 0.5);
            inputBuffer.addFrom(channel, 0, buffer, channel * 2.0 + 1.0, bufferSize, 0.5);

            float alphaAttack = exp(-1 / (0.001 * mSampleRate * mAttackTime));
            float alphaRelease = exp(-1 / (0.001 * mSampleRate * mReleaseTime));

            for (int sample = 0; sample < bufferSize; ++sample)
            {
                //Level detection- estimate level using peak detector
                if (fabs(buffer.getWritePointer(channel)[sample]) < 0.000001)
                {
                    mInputGain = -120.0;
                }
                else
                {
                    mInputGain = 20 * log10(fabs(buffer.getWritePointer(channel)[sample]));
                }
                
                // Gain computer - apply input/output curve with kneewidth
                   // Incorporating the SOFT KNEE
                if (2 * (mInputGain - mThreshold) >= mKneeWidth)
                {
                    // above knee
                    mOutputGain = mThreshold + (mInputGain - mThreshold) / mRatio;
                }
                else if (2 * fabs(mInputGain - mThreshold) <= mKneeWidth)
                {
                    // in knee
                    mOutputGain = mInputGain + (1 / mRatio - 1) * pow(mInputGain - mThreshold + mKneeWidth / 2, 2) / (2 * mKneeWidth);
                }
                else // below knee
                {
                    mOutputGain = mInputGain;
                }
                mInputLevel = mInputGain - mOutputGain;

                //Ballistics- smoothing of the gain
                if (mInputLevel > mPreviousOutputLevel)
                {
                    mOutputLevel = alphaAttack * mPreviousOutputLevel + (1 - alphaAttack) * mInputLevel;
                }
                else
                {
                    mOutputLevel = alphaRelease * mPreviousOutputLevel + (1 - alphaRelease) * mInputLevel;
                }

                //find control voltage
                mControlVoltage = pow(10, (mMakeUpGain - mOutputLevel) / 20.0);
                mPreviousOutputLevel = mOutputLevel;

                // apply control voltage to both channels
                buffer.getWritePointer(2 * channel + 0)[sample] *= mControlVoltage;
                buffer.getWritePointer(2 * channel + 1)[sample] *= mControlVoltage;
            }
        }
        else // if threshold = 0, still apply make up gain.
        {
            buffer.applyGain(pow(10, (mMakeUpGain) / 20.0));
        }

    }


    //if (curGain == mMakeUpGain)
    //{
    //    buffer.applyGain(curGain);
    //}
    //else
    //{
    //    buffer.applyGainRamp(0, buffer.getNumSamples(), mOutputGain, curGain);
    //    mOutputGain = curGain;
    //}
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
