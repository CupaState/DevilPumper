#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DevilPumperInfinityAudioProcessorEditor::DevilPumperInfinityAudioProcessorEditor(DevilPumperInfinityAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    sl_threshold_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, THRESHOLD_ID, sl_threshold);
    mode_attach = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, SWITCHER_ID, mode);
    //SLIDER

    addAndMakeVisible(sl_threshold);
    sl_threshold.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    sl_threshold.setTextValueSuffix("dB");
    sl_threshold.setSliderStyle(Slider::SliderStyle::LinearVertical);
    sl_threshold.setRange(-50.0, 0.0);
    sl_threshold.setValue(processor.getThreshold());
    sl_threshold.addListener(this);

    //BUTTON
    mode.setButtonText("LIMITER");
    mode.setState(Button::ButtonState::buttonNormal);
    mode.addListener(this);
    addAndMakeVisible(mode);

    setLookAndFeel(&otherLookAndFeel);

    setSize(200, 400);
}

DevilPumperInfinityAudioProcessorEditor::~DevilPumperInfinityAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void DevilPumperInfinityAudioProcessorEditor::paint(Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::BACK_jpg, BinaryData::BACK_jpgSize);
    g.drawImageAt(background, 0, 0);

    auto edge = 2;
    auto button_area = mode.getLocalBounds();
    mode.setColour(juce::TextButton::ColourIds::buttonColourId, Colours::brown);
    mode.setColour(juce::TextButton::textColourOffId, Colours::blanchedalmond);
    // shadow
    g.setColour(juce::Colours::beige.withAlpha(0.5f));
}

void DevilPumperInfinityAudioProcessorEditor::resized()
{
    auto border = 4;
    Rectangle<int>bounds = getLocalBounds();
    FlexBox flexBox;

    flexBox.items.add(FlexItem(200, 100, sl_threshold));

    auto button_height = 30;

    mode.setBounds(bounds.removeFromBottom(button_height).reduced(border));

    flexBox.performLayout(bounds);
}

void DevilPumperInfinityAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &sl_threshold)
    {
        processor.setThreshold(sl_threshold.getValue());
    }
}

void DevilPumperInfinityAudioProcessorEditor::buttonClicked(Button* click)
{
    if (click == &mode)
    {
        if (processor.getMode() == 0)
        {
            processor.setMode(1.0f);
            mode.setButtonText("COMPRESSOR");
        }
        else
        {
            processor.setMode(0.0f);
            mode.setButtonText("LIMITER");
        }
    }
}
