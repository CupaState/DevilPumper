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
    //SLIDERS

    addAndMakeVisible(&slOverallGain);
    slOverallGain.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slOverallGain.setTextValueSuffix("dB");
    slOverallGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slOverallGain.setRange(-40.0, 10.0);
    slOverallGain.setValue(0.0);
    slOverallGain.addListener(this);

    addAndMakeVisible(&slAttackTime);
    slAttackTime.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slAttackTime.setTextValueSuffix("ms");
    slAttackTime.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slAttackTime.setRange(1.0, 250.0);
    slAttackTime.setValue(25.0);
    slAttackTime.addListener(this);

    addAndMakeVisible(&slRelease);
    slRelease.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slRelease.setTextValueSuffix("ms");
    slRelease.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slRelease.setRange(1.0, 2500.0);
    slRelease.setValue(25.0);
    slRelease.addListener(this);

    addAndMakeVisible(&slThreshold);
    slThreshold.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slThreshold.setTextValueSuffix("dB");
    slThreshold.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slThreshold.setRange(-50.0, 0.0);
    slThreshold.setValue(-3.0);
    slThreshold.addListener(this);

    addAndMakeVisible(&slRatio);
    slRatio.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slRatio.setTextValueSuffix(" :1");
    slRatio.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slRatio.setRange(1.0, 100.0);
    slRatio.setValue(1.0);
    slRatio.addListener(this);

    addAndMakeVisible(&slKneeWidth);
    slKneeWidth.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slKneeWidth.setTextValueSuffix("dB");
    slKneeWidth.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slKneeWidth.setRange(0.0, 10.0);
    slKneeWidth.setValue(5.0);
    slKneeWidth.addListener(this);

    addAndMakeVisible(&slGain);
    slGain.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    slGain.setTextValueSuffix("dB");
    slGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slGain.setRange(-80.0, 40.0);
    slGain.setValue(0.0);
    slGain.addListener(this);

    //LABELS

    addAndMakeVisible(lOverallGain);
    addAndMakeVisible(lAttack);
    addAndMakeVisible(lRelease);
    addAndMakeVisible(lThreshold);
    addAndMakeVisible(lRatio);
    addAndMakeVisible(lKneeWidth);
    addAndMakeVisible(lGain);

    lOverallGain.setText("Output Gain", dontSendNotification);
    lAttack.setText("Attack", dontSendNotification);
    lRelease.setText("Release", dontSendNotification);
    lThreshold.setText("Threshold", dontSendNotification);
    lRatio.setText("Ratio", dontSendNotification);
    lKneeWidth.setText("Knee", dontSendNotification);
    lGain.setText("Gain", dontSendNotification);

    lOverallGain.setFont(Font(15.0f, Font::plain));
    lAttack.setFont(Font(15.0f, Font::plain));
    lRelease.setFont(Font(15.0f, Font::plain));
    lThreshold.setFont(Font(15.0f, Font::plain));
    lRatio.setFont(Font(15.0f, Font::plain));
    lKneeWidth.setFont(Font(15.0f, Font::plain));
    lGain.setFont(Font(15.0f, Font::plain));

    lOverallGain.attachToComponent(&slOverallGain, true);
    lAttack.attachToComponent(&slAttackTime, true);
    lRelease.attachToComponent(&slRelease, true);
    lThreshold.attachToComponent(&slThreshold, true);
    lRatio.attachToComponent(&slRatio, true);
    lKneeWidth.attachToComponent(&slKneeWidth, true);
    lGain.attachToComponent(&slGain, true);

    //BUTTON

    addAndMakeVisible(btnAnalogDigital = new TextButton("Analog"));
    btnAnalogDigital->setButtonText("Analog");
    btnAnalogDigital->setColour(TextButton::buttonOnColourId, Colours::firebrick);
    btnAnalogDigital->setColour(TextButton::textColourOnId, Colours::palevioletred);
    btnAnalogDigital->setColour(TextButton::textColourOffId, Colours::black);
    btnAnalogDigital->addListener(this);

    setSize(1600, 600);
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
    g.drawFittedText("DevilPumper", getWidth() / 2 - 75, 0, 150, getWidth() / 2 - 75, Justification::centred, 1);
    //g.fillAll(Colours::black);
}

void DevilPumperInfinityAudioProcessorEditor::resized()
{
    Rectangle<int>bounds = getLocalBounds();
    FlexBox flexBox;

    flexBox.items.add(FlexItem(200, 100, slOverallGain));
    flexBox.items.add(FlexItem(200, 100, slAttackTime));
    flexBox.items.add(FlexItem(200, 100, slThreshold));
    flexBox.items.add(FlexItem(200, 100, slRelease));
    flexBox.items.add(FlexItem(200, 100, slRatio));
    flexBox.items.add(FlexItem(200, 100, slKneeWidth));
    flexBox.items.add(FlexItem(200, 100, slGain));
    flexBox.items.add(FlexItem(50, 20, *btnAnalogDigital));

    flexBox.performLayout(bounds);
}

void DevilPumperInfinityAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &slOverallGain)
    {
        processor.setOverallGain(slOverallGain.getValue());
    }
    else if (slider == &slAttackTime)
    {
        processor.setAttack(slAttackTime.getValue());
    }
    else if (slider == &slRelease)
    {
        processor.setRelease(slRelease.getValue());
    }
    else if (slider == &slThreshold)
    {
        processor.setThreshold(slThreshold.getValue());
    }
    else if (slider == &slRatio)
    {
        processor.setRatio(slRatio.getValue());
    }
    else if (slider == &slKneeWidth)
    {
        processor.setKneeWidth(slKneeWidth.getValue());
    }
    else if (slider == &slGain)
    {
        processor.setGain(slGain.getValue());
    }
}


void DevilPumperInfinityAudioProcessorEditor::buttonClicked(Button* button)
{
    int counter = 0;
    if (button == btnAnalogDigital)
    {
        btnAnalogDigital->setToggleState(true, false);
        counter++;
        if (counter % 2 == 0)
        {
            btnAnalogDigital->setToggleState(false, false);
        }
        else
        {
            btnAnalogDigital->setToggleState(true, false);
        }
    }
    processor.setAnalogDigital(btnAnalogDigital->getToggleState());
}