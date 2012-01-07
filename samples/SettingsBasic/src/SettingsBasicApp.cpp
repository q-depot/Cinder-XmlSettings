/*
 *  SettingsBasicApp.cpp
 *  CindeXmlSettings
 *
 *  Created by Andrea Cuius on 26/11/2011.
 *  Nocte Studio Copyright 2011 . All rights reserved.
 *	www.nocte.co.uk
 *
 */


#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "Resources.h"
#include "XmlSettings.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SettingsBasicApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	
	nocte::XmlSettings	*mSettings;
	
	int			mTestInt32;
	float		mTestFloat;
	double		mTestDouble;
	bool		mTestBool;
	
	Vec2f		mTestVec2f;
	Vec3f		mTestVec3f;
	
	Color		mTestColor;
	ColorA		mTestColorA;
	
	string		mTestString;
};

void SettingsBasicApp::setup()
{
	
//	mSettings = new XmlSettings( loadResource(RES_SETTINGS) );

	mSettings = new nocte::XmlSettings();
	
//	mSettings->addParam( "mTestInt32",	&mTestInt32 );
	mSettings->addParam( "mTestFloat",	&mTestFloat );
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
	

	mSettings->load("/Users/Q/Code/Cinder/blocks/XmlSettings/samples/SettingsBasic/xcode/build/Debug/appSettings.xml");
	
	mSettings->addParam( "mTestInt32",	&mTestInt32 );	
	mTestInt32	= 333;
	
	
	console() << mSettings->getValueByName<ColorA>("mTestColorA") << endl;
}

void SettingsBasicApp::mouseDown( MouseEvent event )
{
	mSettings->save();
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


CINDER_APP_BASIC( SettingsBasicApp, RendererGl )
