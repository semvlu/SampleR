#include "PluginProcessor.h"
#include "PluginEditor.h"


SampleRAudioProcessorEditor::SampleRAudioProcessorEditor (SampleRAudioProcessor& p)
    : AudioProcessorEditor (&p), mWavethumb(p), mADSR(p), processor (p)
{    
    addAndMakeVisible(mWavethumb);
    addAndMakeVisible(mADSR);
    startTimerHz(50);
    setSize (600, 375);
}

SampleRAudioProcessorEditor::~SampleRAudioProcessorEditor()
{
    stopTimer();
}

void SampleRAudioProcessorEditor::paint (Graphics& g)
{
    //g.fillAll(Colours::powderblue);
}

void SampleRAudioProcessorEditor::resized()
{
    mWavethumb.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.7);
    mADSR.setBoundsRelative(0.0f, 0.7f, 1.0f, 0.3f);
}
void SampleRAudioProcessorEditor::timerCallback()
{
    repaint();
}

