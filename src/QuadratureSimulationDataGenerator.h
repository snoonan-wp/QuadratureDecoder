#ifndef QUADRATURE_SIMULATION_DATA_GENERATOR
#define QUADRATURE_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class QuadratureAnalyzerSettings;

class QuadratureSimulationDataGenerator
{
public:
	QuadratureSimulationDataGenerator();
	~QuadratureSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, QuadratureAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	QuadratureAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:

	SimulationChannelDescriptor mQ1SimulationData;
	SimulationChannelDescriptor mQ2SimulationData;

};
#endif //QUADRATURE_SIMULATION_DATA_GENERATOR
