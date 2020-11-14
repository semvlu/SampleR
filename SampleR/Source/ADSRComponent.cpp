/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 20 Jun 2020 2:52:28pm
    Author:  SEM

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(SampleRAudioProcessor& p) : processor(p)
{
    mAttackSlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::firebrick);
    mAttackSlider.setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::powderblue);
    mAttackSlider.setColour(Slider::ColourIds::thumbColourId, Colours::orangered);
    mAttackSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::orangered);
    mAttackSlider.setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::lightcoral);
    addAndMakeVisible(mAttackSlider);
    mAttackLabel.setFont(14.0f);
    mAttackLabel.setText("Attack", NotificationType::dontSendNotification);
    mAttackLabel.setColour(Label::ColourIds::textColourId, Colours::firebrick);
    mAttackLabel.setJustificationType(Justification::centredBottom);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    mAttackAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "ATTACK", mAttackSlider);
    
    mDecaySlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::firebrick);
    mDecaySlider.setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::powderblue);
    mDecaySlider.setColour(Slider::ColourIds::thumbColourId, Colours::honeydew);
    mDecaySlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::honeydew);
    mDecaySlider.setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::lightcoral);
    mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    addAndMakeVisible(mDecaySlider);
    mDecayLabel.setFont(14.0f);
    mDecayLabel.setText("Decay", NotificationType::dontSendNotification);
    mDecayLabel.setColour(Label::ColourIds::textColourId, Colours::firebrick);
    mDecayLabel.setJustificationType(Justification::centredBottom);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    mDecayAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "DECAY", mDecaySlider);

    
    mSustainSlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::firebrick);
    mSustainSlider.setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::powderblue);
    mSustainSlider.setColour(Slider::ColourIds::thumbColourId, Colours::purple);
    mSustainSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::purple);
    mSustainSlider.setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::lightcoral);
    mSustainSlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(mSustainSlider);
    mSustainLabel.setFont(14.0f);
    mSustainLabel.setText("Sustain", NotificationType::dontSendNotification);
    mSustainLabel.setColour(Label::ColourIds::textColourId, Colours::firebrick);
    mSustainLabel.setJustificationType(Justification::centredBottom);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    mSustainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "SUSTAIN", mSustainSlider);

    mReleaseSlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::firebrick);
    mReleaseSlider.setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::powderblue);
    mReleaseSlider.setColour(Slider::ColourIds::thumbColourId, Colours::lightseagreen);
    mReleaseSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::lightseagreen);
    mReleaseSlider.setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::lightcoral);
    mReleaseSlider.setRange(0.0f, 5.0f, 0.01f);
    addAndMakeVisible(mReleaseSlider);
    mReleaseLabel.setFont(14.0f);
    mReleaseLabel.setText("Release", NotificationType::dontSendNotification);
    mReleaseLabel.setColour(Label::ColourIds::textColourId, Colours::firebrick);
    mReleaseLabel.setJustificationType(Justification::centredBottom);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    mReleaseAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "RELEASE", mReleaseSlider);
    
    auto metaImage = ImageCache::getFromMemory(BinaryData::meta_png, BinaryData::meta_pngSize);
    if (! metaImage.isNull())
        mImageComponent.setImage (metaImage, RectanglePlacement::stretchToFit);
    else
        jassert (! metaImage.isNull());
    addAndMakeVisible(mImageComponent);


}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (Graphics& g)
{
    g.fillAll(Colours::powderblue);
}

void ADSRComponent::resized()
{
    const auto startX = 0.6f;
    const auto startY = 0.2f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.75f;
    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);
    mImageComponent.setBoundsRelative(0.0f, 0.65f, 0.2f, 0.35f);

}
