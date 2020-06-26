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

    Label lRelease;
    Label lMakeUpGain;
    Label lAttack;
    Label lThreshold;
    Label lRatio;
    Label lKneeWidth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevilPumperInfinityAudioProcessorEditor)
public:
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slMakeUpGainAttach;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slAttackTimeAttach;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slReleaseTimeAttach;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slThresholdAttach;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slRatioAttach;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slKneeAttach;
};
