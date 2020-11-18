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
    slThresholdAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, THRESHOLD_ID, slThreshold);

    //SLIDERS

    addAndMakeVisible(&slThreshold);
    slThreshold.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slThreshold.setTextValueSuffix("dB");
    slThreshold.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slThreshold.setRange(-50.0, 0.0);
    slThreshold.setValue(processor.getThreshold());
    slThreshold.addListener(this);

    setSize(200, 400);
}

DevilPumperInfinityAudioProcessorEditor::~DevilPumperInfinityAudioProcessorEditor()
{
}

//==============================================================================
void DevilPumperInfinityAudioProcessorEditor::paint(Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::BACK_jpg, BinaryData::BACK_jpgSize);
    g.drawImageAt(background, 0, 0);
    g.setFont(15.0f);
    g.drawFittedText("DevilPumper", getWidth() / 2 - 75, 0, 150, getWidth() / 2 - 75, Justification::top, 8);
}

void DevilPumperInfinityAudioProcessorEditor::resized()
{
    Rectangle<int>bounds = getLocalBounds();
    FlexBox flexBox;

    flexBox.items.add(FlexItem(200, 100, slThreshold));

    flexBox.performLayout(bounds);
}

void DevilPumperInfinityAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &slThreshold)
    {
        processor.setThreshold(slThreshold.getValue());
    }
}