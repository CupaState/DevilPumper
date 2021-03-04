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
    threshold = parameters.getRawParameterValue(THRESHOLD_ID);
    ratio = 4.0f;
    attack_time = 0.030f;
    release_time = 0.091f;
    input_gain = 0.0f;
    log_output_gain = 0.0f;
    log_output_level = 0.0f;
    log_input_level = 0.0f;
    log_threhsold = 0.0f;
    crest_PEAK = MINVAL;
    crest_RMS = MINVAL;
    crest_factor = 0.0f;
    cv_dev_const = 0.1f;
    average_time = 0.0f;
    alpha_for_cv = 0.999992f; // alpha_time_for_cv = 3.0f; std::exp(-1.0f / (pSampleRate * alpha_time_for_cv));
    log_range = -4.605170f; //range = -40.0; log_range = std::log(std::powf(10.0f, range / 20.0f));
    mode = parameters.getRawParameterValue(SWITCHER_ID);

    parameters.state = ValueTree("savedParameters");
}

DevilPumperInfinityAudioProcessor::~DevilPumperInfinityAudioProcessor()
{
    threshold = nullptr;
    delete threshold;
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout DevilPumperInfinityAudioProcessor::createParameter()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    auto threshold_param = std::make_unique<AudioParameterFloat>(THRESHOLD_ID, THRESHOLD_NAME, -50.0f, 0.0f, 0.0f);
    params.push_back(std::move(threshold_param));
    auto switcher_param = std::make_unique<AudioParameterFloat>(SWITCHER_ID, SWITCHER_NAME, 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(switcher_param));

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
    sample_rate = (float)getSampleRate();
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
    const int bufferSize = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels(); // number of channels

    for (int ch = 0; ch < 1; ++ch) //For each channel pair of channels
    {
        float alpha_attack = 0.0f;
        float alpha_release = 0.0f;

        for (int sample = 0; sample < bufferSize; ++sample)
        {
            average_time = (MAXATTACKTIME + MAXRELEASETIME) / 2.0f;
            alpha_average = 1.0 - std::exp(-1.0f / (sample_rate * average_time));
            input_gain = std::fabs(buffer.getWritePointer(ch)[sample]);
            input_square = input_gain * input_gain;

            crest_PEAK = std::fmax(input_square, crest_PEAK + alpha_average * (input_square - crest_PEAK));
            crest_RMS = crest_RMS + alpha_average * (input_square - crest_RMS);
            crest_factor = std::log(std::sqrt(crest_PEAK / crest_RMS));

            if (crest_factor <= 0.0f)
            {
                crest_factor = MINVAL;
            }

            switch ((int)*mode)
            {
            case 0:
            {
                attack_time = (2.0f * MAXATTACKTIME / crest_factor) / 3.0f;
                release_time = std::fmax((2.0f * MAXRELEASETIME / crest_factor - attack_time) / 10.0f, 0.0f);
                break;
            }
            case 1:
            {
                attack_time = (2.0f * MAXATTACKTIME / crest_factor) / 10.0f;
                release_time = std::fmax((2.0f * MAXRELEASETIME / crest_factor - attack_time) / 5.0f, 0.0f);
                break;
            }
            }

            if (attack_time > release_time)
            {
                release_time = attack_time + release_time;
            }

            alpha_attack = 1.0f - std::exp(-1.0f / (sample_rate * attack_time));
            alpha_release = 1.0f - std::exp(-1.0f / (sample_rate * release_time));

            if (input_gain < MINVAL)
            {
                log_input_gain = std::log(MINVAL);
            }
            else
            {
                log_input_gain = std::log(input_gain);
            }

            log_threhsold = std::log(std::pow(10, *threshold / 20.0));

            switch ((int)*mode)
            {
            case 0:
            {
                if (log_input_gain > log_threhsold)
                {
                    log_output_gain = log_threhsold + log_range;
                }
                else
                {
                    log_output_gain = log_input_gain;
                }
                break;
            }
            case 1:
            {
                if (log_input_gain > log_threhsold)
                {
                    log_output_gain = log_threhsold + (log_input_gain - log_threhsold) / ratio;
                }
                else
                {
                    log_output_gain = log_input_gain;
                }
                break;
            }
            }

            log_input_level = log_input_gain - log_output_gain;

            if (log_input_level > log_output_level)
            {
                log_output_level = log_output_level + alpha_attack * (log_input_level - log_output_level);
            }
            else
            {
                log_output_level = (log_output_level + alpha_release * (log_input_level - log_output_level)) / (1.0f + alpha_release * 2.5f);
            }

            cv_estimate = log_threhsold / 2.0f;

            cv_dev = alpha_for_cv * cv_dev_const + (1.0f - alpha_for_cv) * (log_output_level + cv_estimate);

            control_voltage = std::exp(-(log_output_level - cv_dev));

            buffer.getWritePointer(ch)[sample] *= control_voltage;
            buffer.getWritePointer(ch + 1)[sample] *= control_voltage;
        }
    }
}


void DevilPumperInfinityAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();

    // Initialise Buffer

    AudioSampleBuffer Output;

    // Set buffer to an input

    Output.makeCopyOf(buffer);

    //compression
    if (*threshold < 0)
    {
        compressorMath(Output);
    }
    else
    {
        Output.applyGain(1.0f);
    }

    buffer.clear();

    for (int channel = 0; channel < getTotalNumInputChannels(); channel++)
    {
        buffer.addFrom(channel, 0, Output, channel, 0, numSamples, 1.0);
    }

    switch ((int)*mode)
    {
    case(0):
    {
        buffer.applyGain(1.0f + setOverallGain(*threshold));
        break;
    }
    case(1):
    {
        buffer.applyGain(1.0f + (setOverallGain(*threshold) / 10.0f));
        break;
    }
    }
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
    XmlElement xml("savedParameters");
    xml.setAttribute(THRESHOLD_ID, *threshold);
    xml.setAttribute(SWITCHER_ID, *mode);
    for (int i = 0; i < getNumParameters(); ++i)
    {
        if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
        {
            xml.setAttribute(p->paramID, p->getValue());

            if (p->paramID == SWITCHER_ID)
            {
                p->setValue(*mode);
                xml.setAttribute(SWITCHER_ID, p->getValue());
            }
        }
    }

    copyXmlToBinary(xml, destData);
}

void DevilPumperInfinityAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName("savedParameters"))
        {
            // ok, now pull out our last state..
            *threshold = xmlState->getIntAttribute(THRESHOLD_ID, *threshold);
            *mode = xmlState->getIntAttribute(SWITCHER_ID, *mode);
            // Now reload our parameters..
            for (int i = 0; i < getNumParameters(); ++i)
                if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
                {
                    p->setValue((float)xmlState->getDoubleAttribute(p->paramID, p->getValue()));
                }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DevilPumperInfinityAudioProcessor();
}
