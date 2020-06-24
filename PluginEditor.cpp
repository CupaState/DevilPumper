/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DevilPumperInfinityAudioProcessorEditor::DevilPumperInfinityAudioProcessorEditor(DevilPumperInfinityAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    slOutputGainAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, slOutputGain);
    slAttackTimeAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, ATTACK_ID, slAttackTime);
    slReleaseTimeAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, RELEASE_ID, slRelease);
    slThresholdAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, THRESHOLD_ID, slThreshold);
    slRatioAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, RATIO_ID, slRatio);

    //SLIDERS

    addAndMakeVisible(slOutputGain);
    slOutputGain.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slOutputGain.setTextValueSuffix("dB");
    slOutputGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slOutputGain.setRange(-50.0, 10.0);
    slOutputGain.setSize(10, 400);

    addAndMakeVisible(slAttackTime);
    slAttackTime.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slAttackTime.setTextValueSuffix("ms");
    slAttackTime.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slAttackTime.setRange(1.0, 250.0);
    slAttackTime.setValue(5.0);

    addAndMakeVisible(slRelease);
    slRelease.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slRelease.setTextValueSuffix("ms");
    slRelease.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slRelease.setRange(1.0, 2500.0);
    slRelease.setValue(25.0);

    addAndMakeVisible(slThreshold);
    slThreshold.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slThreshold.setTextValueSuffix("dB");
    slThreshold.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slThreshold.setRange(-50.0, 0.0);
    slThreshold.setValue(0.0);

    addAndMakeVisible(slRatio);
    slRatio.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slRatio.setTextValueSuffix(" :1");
    slRatio.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slRatio.setRange(0.0, 100.0);
    slRatio.setValue(0.0);

    //LABELS

    addAndMakeVisible(lOutputGain);
    addAndMakeVisible(lAttack);
    addAndMakeVisible(lRelease);
    addAndMakeVisible(lThreshold);
    addAndMakeVisible(lRatio);

    lOutputGain.setText("Output Gain", dontSendNotification);
    lAttack.setText("Attack", dontSendNotification);
    lRelease.setText("Release", dontSendNotification);
    lThreshold.setText("Threshold", dontSendNotification);
    lRatio.setText("Ratio", dontSendNotification);

    lOutputGain.attachToComponent(&slOutputGain, true);
    lAttack.attachToComponent(&slAttackTime, false);
    lRelease.attachToComponent(&slRelease, true);
    lThreshold.attachToComponent(&slThreshold, true);
    lRatio.attachToComponent(&slRatio, true);

    setSize(1000, 600);
}

DevilPumperInfinityAudioProcessorEditor::~DevilPumperInfinityAudioProcessorEditor()
{
}

//==============================================================================
void DevilPumperInfinityAudioProcessorEditor::paint(Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::BACK_jpg, BinaryData::BACK_jpgSize);
    g.drawImageAt(background, 0, 0);
}

void DevilPumperInfinityAudioProcessorEditor::resized()
{
    Rectangle<int>bounds = getLocalBounds();
    FlexBox flexBox;

    flexBox.items.add(FlexItem(200, 100, slOutputGain));
    flexBox.items.add(FlexItem(200, 100, slAttackTime));
    flexBox.items.add(FlexItem(200, 100, slThreshold));
    flexBox.items.add(FlexItem(200, 100, slRelease));
    flexBox.items.add(FlexItem(200, 100, slRatio));


    flexBox.performLayout(bounds);
}

void DevilPumperInfinityAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &slOutputGain)
    {
        processor.mOutputGain = slOutputGain.getValue();
    }
    else if (slider == &slAttackTime)
    {
        processor.mAttackTime = slAttackTime.getValue();
    }
    else if (slider == &slRelease)
    {
        processor.mReleaseTime = slRelease.getValue();
    }
    else if (slider == &slThreshold)
    {
        processor.mThreshold = slThreshold.getValue();
    }
    else if (slider == &slRatio)
    {
        processor.mRatio = pow(10, slRatio.getValue());
    }
}
