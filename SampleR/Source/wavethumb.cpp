/*
  ==============================================================================

    wavethumb.cpp
    Created: 20 Jun 2020 1:55:24pm
    Author:  SEM
*/

#include <JuceHeader.h>
#include "wavethumb.h"

//==============================================================================
wavethumb::wavethumb(SampleRAudioProcessor& p) : processor (p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

wavethumb::~wavethumb()
{
}

void wavethumb::paint (Graphics& g)
{
    g.fillAll(Colours::cadetblue.darker());
    
    auto waveform = processor.getWaveForm();
    if (waveform.getNumSamples() > 0)
    {
        Path p;
        mAudioPoints.clear();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        for (int sample = 0; sample < waveform.getNumSamples(); sample+=ratio)
        {
            mAudioPoints.push_back (buffer[sample]);
        }
        g.setColour(Colours::azure);
        p.startNewSubPath(0, getHeight() / 2);
        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = jmap<float> (mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
            p.lineTo(sample, point);
        }
        g.strokePath(p, PathStrokeType(2));
        g.setColour(Colours::white);
        g.setFont(15.0f);
        auto textBounds = getLocalBounds().reduced(10, 10);
        g.drawFittedText(mFileName, textBounds, Justification::topRight, 1);
        auto playHeadPosition = jmap<int> (processor.getSampleCount(), 0, processor.getWaveForm().getNumSamples(),0, getWidth());
        g.setColour(Colours::red);
        g.drawLine(playHeadPosition, 0, playHeadPosition, getHeight(), 2.0f);
        g.setColour(Colours::blue.withAlpha(0.12f));
        g.fillRect(0, 0, playHeadPosition, getHeight());
    }
    else
    {
        g.setColour(Colours::white);
        g.setFont(40.0f);
        g.drawFittedText("Drop a file", getLocalBounds(), Justification::centred, 1);
    }
}

void wavethumb::resized()
{

}
bool wavethumb::isInterestedInFileDrag(const StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains (".wav") || file.contains (".aif") || file.contains (".mp3") || file.contains (".m4a"))
        {
            return true;
        }
    }
    return false;
}

void wavethumb::filesDropped (const StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            auto myFile = std::make_unique<File>(file);
            mFileName = myFile->getFileNameWithoutExtension();
            processor.loadFile(file);
        }
    }
    repaint();
}
 
