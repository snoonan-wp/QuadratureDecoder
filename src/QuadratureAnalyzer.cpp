#include "QuadratureAnalyzer.h"
#include "QuadratureAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

QuadratureAnalyzer::QuadratureAnalyzer()
:	Analyzer2(),  
	mSettings( new QuadratureAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

QuadratureAnalyzer::~QuadratureAnalyzer()
{
	KillThread();
}

void QuadratureAnalyzer::SetupResults()
{
	mResults.reset( new QuadratureAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mQ1 );
}

void QuadratureAnalyzer::WorkerThread()
{
	mQ1 = GetAnalyzerChannelData( mSettings->mQ1 );
	mQ2 = GetAnalyzerChannelData( mSettings->mQ2 );

  long long Count = 10000;

  int state = 0;
  U64 start_edge = 0;
  U64 end_edge = 0;

  U64 q1_edge = mQ1->GetSampleOfNextEdge();
  U64 q2_edge = mQ2->GetSampleOfNextEdge();

  state = (mQ1->GetBitState() == BIT_HIGH)?1:0;
  state |= (mQ2->GetBitState() == BIT_HIGH)?2:0;

	for( ; ; )
	{

    /*
     * Forward
     * St 0 1 3 2 0 1 3 2
     *      ___     ___
     * Q1 _/   \___/   \___
     *        ___     ___
     * Q1 ___/   \___/   \
     * state 0:
     *   q1 change to high ++ state = 1
     *   q2 change to high -- state = 2
     * state 1:
     *   q1 change to low  -- state = 0
     *   q2 change to high ++ state = 3
     * state 2:
     *   q1 change to high -- state = 3
     *   q2 change to low  ++ state = 0
     * state 3:
     *   q1 change to low  ++ state = 2
     *   q2 change to low  -- state = 1
     *
     */

    if (q1_edge < q2_edge) { // Q1 transition
      switch (state) {
        case 0:
        case 3:
          Count++;
          break;
        case 1:
        case 2:
          Count--;
          break;
      }
      start_edge = q1_edge;

      mQ1->AdvanceToNextEdge();
      q1_edge = mQ1->GetSampleOfNextEdge();

    } else { // Q2 transition
      switch (state) {
        case 1:
        case 2:
          Count++;
          break;
        case 0:
        case 3:
          Count--;
          break;
      }
      start_edge = q2_edge;

      mQ2->AdvanceToNextEdge();
      q2_edge = mQ2->GetSampleOfNextEdge();
    }

    state = (mQ1->GetBitState() == BIT_HIGH)?1:0;
    state |= (mQ2->GetBitState() == BIT_HIGH)?2:0;

    end_edge = std::min(q1_edge, q2_edge);
    //we have a byte to save. 
    Frame frame;
    frame.mData1 = Count;
    frame.mFlags = 0;
    frame.mStartingSampleInclusive = start_edge;
    frame.mEndingSampleInclusive = end_edge;

    mResults->AddFrame( frame );
    mResults->CommitResults();
    ReportProgress( frame.mEndingSampleInclusive );
  }
}

bool QuadratureAnalyzer::NeedsRerun()
{
	return false;
}

U32 QuadratureAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 QuadratureAnalyzer::GetMinimumSampleRateHz()
{
	return 1;
}

const char* QuadratureAnalyzer::GetAnalyzerName() const
{
	return "Quadrature decoder";
}

const char* GetAnalyzerName()
{
	return "Quadrature decoder";
}

Analyzer* CreateAnalyzer()
{
	return new QuadratureAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
