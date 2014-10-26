
/*
 *  SettingsBasicApp.cpp
 *  XmlSettings
 *
 *  Created by Andrea Cuius.
 *  Nocte Studio Copyright 2013 . All rights reserved.
 *	www.nocte.co.uk
 *
 */


#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "XmlSettings.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class SettingsBasicApp : public AppNative {
  public:
	void setup();
	void keyDown( KeyEvent event );
	void update();
	void draw();
    
    
	nocte::XmlSettingsRef   mSettings;
	
	int                     mTestInt32;
	float                   mTestFloat;
	double                  mTestDouble;
	bool                    mTestBool;
	
	Vec2f                   mTestVec2f;
	Vec3f                   mTestVec3f;
	
	Color                   mTestColor;
	ColorA                  mTestColorA;
	
	string                  mTestString;
};


void SettingsBasicApp::setup()
{
    mSettings = nocte::XmlSettings::create();							// create a new instance, at this point XmlSettings doesn't have any file associated
	
    //	mSettings->addParam( "mTestInt32",	&mTestInt32 );
	mSettings->addParam( "mTestFloat",	&mTestFloat );				// add a new variable to the XmlSettings
	mSettings->addParam( "mTestDouble",	&mTestDouble );
	mSettings->addParam( "mTestBool",	&mTestBool );
	mSettings->addParam( "mTestVec2f",	&mTestVec2f );
	mSettings->addParam( "mTestVec3f",	&mTestVec3f );
	mSettings->addParam( "mTestColor",	&mTestColor );
	mSettings->addParam( "mTestColorA",	&mTestColorA );
	mSettings->addParam( "mTestString",	&mTestString );
	
	mTestInt32	= 0;
	mTestFloat	= 0.0f;
	mTestDouble	= 0.0f;
	mTestBool	= false;
	mTestVec2f	= Vec2f::zero();
	mTestVec3f	= Vec3f::zero();
	mTestColor	= Color::black();
	mTestColorA = ColorA::black();
	mTestString = "zero";
	
	string filename = getOpenFilePath().generic_string();
	
	if ( filename != "" )
		mSettings->load(filename);									// load the xml settings file, this method override all the current values in XmlSettings
	
	mSettings->addParam( "mTestInt32",	&mTestInt32 );				// if mTestInt32 has been loaded already from the xml file ( load() ), but not added yet, addParam() just replace the pointer
	mTestInt32	= 333;
	
	console() << mSettings->getValueByName<ColorA>("mTestColorA") << endl;
}


void SettingsBasicApp::keyDown( KeyEvent event )
{
    char c = event.getChar();
	
	if ( c == 'l' )													// Load a new xml settings file
	{
        fs::path path = getOpenFilePath().generic_string();
		if ( !path.empty() )
            mSettings->load( path );
	}
	
	else if ( c == 'r' )											// Reload the current xml settings file, if none has been loaded, it doesn't do anything
		mSettings->load();
	
	else if ( c == 's' )											// Save the current settings to the app root folder, the default filename is appSettings.xml, you can also specify another one
    {
        fs::path path = getSaveFilePath().generic_string();
		if ( !path.empty() )
            mSettings->save( path );
        
    }
}


void SettingsBasicApp::update()
{
}


void SettingsBasicApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    if ( mSettings )
		mSettings->drawDebug();
}


CINDER_APP_NATIVE( SettingsBasicApp, RendererGl )

