/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Compressor.h"

//==============================================================================
/**
*/
class DevilPumperInfinityAudioProcessorEditor : public AudioProcessorEditor, public Slider::Listener
{
public:
    DevilPumperInfinityAudioProcessorEditor(DevilPumperInfinityAudioProcessor&);
    ~DevilPumperInfinityAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;


private:
    DevilPumperInfinityAudioProcessor& processor;
    Slider slMakeUpGain;
    Slider slAttackTime;
    Slider slRelease;
    Slider slThreshold;
    Slider slRatio;
    Slider slKneeWidth;
    Slider slGain;

    Label lRelease;
    Label lMakeUpGain;
    Label lAttack;
    Label lThreshold;
    Label lRatio;
    Label lKneeWidth;
    Label lGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessorEditor)
};
