#include "QuadratureAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


QuadratureAnalyzerSettings::QuadratureAnalyzerSettings()
:	mQ1( UNDEFINED_CHANNEL ),
  mQ2( UNDEFINED_CHANNEL )
{
	mInputQ1Interface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputQ1Interface->SetTitleAndTooltip( "Q1", "Q1" );
	mInputQ1Interface->SetChannel( mQ1 );

	mInputQ2Interface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputQ2Interface->SetTitleAndTooltip( "Q2", "Q2" );
	mInputQ2Interface->SetChannel( mQ2 );

	AddInterface( mInputQ1Interface.get() );
	AddInterface( mInputQ2Interface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mQ1, "Q1", false );
	AddChannel( mQ2, "Q2", false );
}

QuadratureAnalyzerSettings::~QuadratureAnalyzerSettings()
{
}

bool QuadratureAnalyzerSettings::SetSettingsFromInterfaces()
{
	mQ1 = mInputQ1Interface->GetChannel();
	mQ2 = mInputQ2Interface->GetChannel();

	ClearChannels();
	AddChannel( mQ1, "Q1", false );
	AddChannel( mQ2, "Q2", false );

	return true;
}

void QuadratureAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputQ1Interface->SetChannel( mQ1 );
	mInputQ2Interface->SetChannel( mQ2 );
}

void QuadratureAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mQ1;
	text_archive >> mQ2;

	ClearChannels();
	AddChannel( mQ1, "Q1", false );
	AddChannel( mQ2, "Q2", false );

	UpdateInterfacesFromSettings();
}

const char* QuadratureAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mQ1;
	text_archive << mQ2;

	return SetReturnString( text_archive.GetString() );
}
