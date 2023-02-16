#include "QuadratureSimulationDataGenerator.h"
#include "QuadratureAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

QuadratureSimulationDataGenerator::QuadratureSimulationDataGenerator()
{
}

QuadratureSimulationDataGenerator::~QuadratureSimulationDataGenerator()
{
}

void QuadratureSimulationDataGenerator::Initialize( U32 simulation_sample_rate, QuadratureAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mQ1SimulationData.SetChannel( mSettings->mQ1 );
	mQ1SimulationData.SetSampleRate( simulation_sample_rate );
	mQ1SimulationData.SetInitialBitState( BIT_HIGH );

	mQ2SimulationData.SetChannel( mSettings->mQ2 );
	mQ2SimulationData.SetSampleRate( simulation_sample_rate );
	mQ2SimulationData.SetInitialBitState( BIT_HIGH );
}

U32 QuadratureSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

  // Forward 20 steps
  for (int i=0; i < 10; i++) {
    mQ1SimulationData.Advance( 10 );
    mQ2SimulationData.Advance( 10 );
    if (i%2 == 0) {
      mQ1SimulationData.Transition();
    } else {
      mQ2SimulationData.Transition();
    }
  }

  
	while( mQ1SimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{

    // back 40 steps
  for (int i=0; i < 20; i++) {
    mQ1SimulationData.Advance( 10 );
    mQ2SimulationData.Advance( 10 );
    if (i%2 == 0) {
      mQ2SimulationData.Transition();
    } else {
      mQ1SimulationData.Transition();
    }
  }

    // forward 40 steps
  for (int i=0; i < 20; i++) {
    mQ1SimulationData.Advance( 10 );
    mQ2SimulationData.Advance( 10 );
    if (i%2 == 0) {
      mQ1SimulationData.Transition();
    } else {
      mQ2SimulationData.Transition();
    }
  }
	}

	*simulation_channel++ = &mQ1SimulationData;
	*simulation_channel = &mQ2SimulationData;
	return 2;
}
