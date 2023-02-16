#ifndef QUADRATURE_ANALYZER_SETTINGS
#define QUADRATURE_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class QuadratureAnalyzerSettings : public AnalyzerSettings
{
public:
	QuadratureAnalyzerSettings();
	virtual ~QuadratureAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mQ1;
	Channel mQ2;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputQ1Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputQ2Interface;
};

#endif //QUADRATURE_ANALYZER_SETTINGS
