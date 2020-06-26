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
    slMakeUpGainAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, MAKEUP_ID, slMakeUpGain);
    slAttackTimeAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, ATTACK_ID, slAttackTime);
    slReleaseTimeAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, RELEASE_ID, slRelease);
    slThresholdAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, THRESHOLD_ID, slThreshold);
    slRatioAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, RATIO_ID, slRatio);
    slKneeAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, KNEE_ID, slKneeWidth);

    //SLIDERS

    addAndMakeVisible(slMakeUpGain);
    slMakeUpGain.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slMakeUpGain.setTextValueSuffix("dB");
    slMakeUpGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slMakeUpGain.setRange(-50.0, 10.0);
    slMakeUpGain.setSize(10, 400);

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

    addAndMakeVisible(slKneeWidth);
    slKneeWidth.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slKneeWidth.setTextValueSuffix("dB");
    slKneeWidth.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slKneeWidth.setRange(0.0, 72.0);
    slKneeWidth.setValue(5.0);

    //LABELS

    addAndMakeVisible(lMakeUpGain);
    addAndMakeVisible(lAttack);
    addAndMakeVisible(lRelease);
    addAndMakeVisible(lThreshold);
    addAndMakeVisible(lRatio);
    addAndMakeVisible(lKneeWidth);

    lMakeUpGain.setText("Output Gain", dontSendNotification);
    lAttack.setText("Attack", dontSendNotification);
    lRelease.setText("Release", dontSendNotification);
    lThreshold.setText("Threshold", dontSendNotification);
    lRatio.setText("Ratio", dontSendNotification);
    lKneeWidth.setText("Knee", dontSendNotification);

    lMakeUpGain.attachToComponent(&slMakeUpGain, true);
    lAttack.attachToComponent(&slAttackTime, true);
    lRelease.attachToComponent(&slRelease, true);
    lThreshold.attachToComponent(&slThreshold, true);
    lRatio.attachToComponent(&slRatio, true);
    lKneeWidth.attachToComponent(&slKneeWidth, true);

    setSize(1200, 600);
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

    flexBox.items.add(FlexItem(200, 100, slMakeUpGain));
    flexBox.items.add(FlexItem(200, 100, slAttackTime));
    flexBox.items.add(FlexItem(200, 100, slThreshold));
    flexBox.items.add(FlexItem(200, 100, slRelease));
    flexBox.items.add(FlexItem(200, 100, slRatio));
    flexBox.items.add(FlexItem(200, 100, slKneeWidth));


    flexBox.performLayout(bounds);
}

void DevilPumperInfinityAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &slMakeUpGain)
    {
        processor.mMakeUpGain = slMakeUpGain.getValue();
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
        processor.mRatio = slRatio.getValue();
    }
    else if (slider == &slKneeWidth)
    {
        processor.mKneeWidth = slKneeWidth.getValue();
    }
}
