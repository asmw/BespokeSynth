//
//  Neighborhooder.cpp
//  modularSynth
//
//  Created by Ryan Challinor on 3/10/13.
//
//

#include "Neighborhooder.h"
#include "OpenFrameworksPort.h"
#include "Scale.h"
#include "ModularSynth.h"

Neighborhooder::Neighborhooder()
: mOctave(3)
, mOctaveSlider(nullptr)
{
}

void Neighborhooder::CreateUIControls()
{
   IDrawableModule::CreateUIControls();
   mOctaveSlider = new IntSlider(this,"octave",4,3,116,15,&mOctave,0,8);
}

void Neighborhooder::DrawModule()
{
   if (Minimized() || IsVisible() == false)
      return;
   
   mOctaveSlider->Draw();
}

void Neighborhooder::CheckboxUpdated(Checkbox *checkbox)
{
   if (checkbox == mEnabledCheckbox)
      mNoteOutput.Flush();
}

void Neighborhooder::IntSliderUpdated(IntSlider* slider, int oldVal)
{
   if (slider == mOctaveSlider)
      mNoteOutput.Flush();
}

void Neighborhooder::PlayNote(double time, int pitch, int velocity, int voiceIdx /*= -1*/, ModulationChain* pitchBend /*= nullptr*/, ModulationChain* modWheel /*= nullptr*/, ModulationChain* pressure /*= nullptr*/)
{
   if (!mEnabled)
   {
      PlayNoteOutput(time, pitch, velocity, voiceIdx, pitchBend, modWheel, pressure);
      return;
   }
   
   pitch -= TheScale->ScaleRoot();
   while (pitch < 0)
      pitch += TheScale->GetTet();
   pitch %= TheScale->GetTet();
   pitch += TheScale->ScaleRoot();
   pitch += mOctave * 12;
   PlayNoteOutput(time, pitch, velocity, voiceIdx, pitchBend, modWheel, pressure);
}

void Neighborhooder::LoadLayout(const ofxJSONElement& moduleInfo)
{
   mModuleSaveData.LoadString("target", moduleInfo);
   mModuleSaveData.LoadBool("enabled", moduleInfo);

   SetUpFromSaveData();
}

void Neighborhooder::SetUpFromSaveData()
{
   SetUpPatchCables(mModuleSaveData.GetString("target"));
   SetEnabled(mModuleSaveData.GetBool("enabled"));
}


