#ifndef QUADRATURE_ANALYZER_H
#define QUADRATURE_ANALYZER_H

#include <Analyzer.h>
#include "QuadratureAnalyzerResults.h"
#include "QuadratureSimulationDataGenerator.h"

class QuadratureAnalyzerSettings;
class ANALYZER_EXPORT QuadratureAnalyzer : public Analyzer2
{
public:
	QuadratureAnalyzer();
	virtual ~QuadratureAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< QuadratureAnalyzerSettings > mSettings;
	std::auto_ptr< QuadratureAnalyzerResults > mResults;
	AnalyzerChannelData* mQ1;
	AnalyzerChannelData* mQ2;

	QuadratureSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //QUADRATURE_ANALYZER_H
