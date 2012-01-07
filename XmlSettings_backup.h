/*
 *  XmlSettings.h
 *  ExxonMobil
 *
 *  Created by Q on 26/11/2011.
 *  Copyright 2011 . All rights reserved.
 *
 */

#pragma once

#include "cinder/Xml.h"
#include "cinder/Utilities.h"

/*
 <?xml version="1.0" encoding="utf-8"?>
 <settings>
 <float name="resolution" width="1706" height="960" />
 <bool name="runDebugMode" value="1" />
 </settings>
 */

static const std::string typeString[10] = { 
	"int", 
	"float", 
	"double", 
	"bool", 
	"Vec2f", 
	"Vec3f", 
	"Color", 
	"ColorA", 
	"Font", 
	"string" 
};


class Param {
	
public:
	
	enum ParamType {
		PARAM_INT,
		PARAM_FLOAT,
		PARAM_DOUBLE,
		PARAM_BOOL,
		PARAM_VEC2F,
		PARAM_VEC3F,
		PARAM_COLOR,
		PARAM_COLORA,
		PARAM_FONT,
		PARAM_STRING
	};
	
	Param(const std::string &name, void *param, ParamType ptype) { mName = name; mParam = param; mType = ptype; };
	
	template <class T>
	T getValue() { return *static_cast<T*>(mParam); };

	template <class T> 
	void setValue(T val) { *static_cast<T*>(mParam) = val; };
	
	std::string getName() { return mName; };

	std::string	getType() { return typeString[mType]; };
	
	std::string getAsString() { 
		
		if ( mType == PARAM_INT )
			return ci::toString(*static_cast<int*>(mParam));
		else if ( mType == PARAM_FLOAT )
			return ci::toString(*static_cast<float*>(mParam));
		else if ( mType == PARAM_DOUBLE )
			return ci::toString(*static_cast<double*>(mParam));
		else if ( mType == PARAM_BOOL )
			return ci::toString(*static_cast<bool*>(mParam));
		else if ( mType == PARAM_VEC2F )
			return ci::toString(*static_cast<ci::Vec2f*>(mParam));
		else if ( mType == PARAM_VEC3F )
			return ci::toString(*static_cast<ci::Vec3f*>(mParam));
		else if ( mType == PARAM_COLOR )
			return ci::toString(*static_cast<ci::Color*>(mParam));
		else if ( mType == PARAM_COLORA )
			return ci::toString(*static_cast<ci::ColorA*>(mParam));
		else if ( mType == PARAM_FONT )
			return "TODO !!!!";//ci::toString(*static_cast<ci::ColorA*>(mParam));
		else if ( mType == PARAM_STRING )
			return *static_cast<std::string*>(mParam);
	};

private:
	
	std::string	mName;
	void		*mParam;
	ParamType	mType;

};


class XmlSettings {
	
public:
	
	XmlSettings() { };
	
	XmlSettings( ci::DataSourceRef fileRef )
	{
		try {
			mXmlAsset = ci::XmlTree( fileRef ).getChild("settings");
		}
		catch ( ... ) {
			ci::app::console() << "failed to load XML settings file: " << mFilename << std::endl;
			return;
		}
		
	}
	
	XmlSettings( std::string filename )
	{
		mFilename = filename;
		
		try {
			mXmlAsset = ci::XmlTree( ci::loadFile(mFilename) ).getChild("settings");
		}
		catch ( ... ) {
			ci::app::console() << "failed to load XML settings file: " << mFilename << std::endl;
			return;
		}
		
	};
	
	
	void loadParam( ci::XmlTree node )
	{
		
		std::string tag		= node.getTag();
		std::string name	= node.getAttributeValue<std::string>("name");
		bool paramFound		= false;
		
		for( int k=0; k < mParams.size(); k++)
		{				
			if ( mParams[k].getName() == name )
			{
				paramFound = true;
				if ( tag == "int" )
					mParams[k].setValue<int>( node.getAttributeValue<int>("value") );
				
				else if ( tag == "float" )
					mParams[k].setValue<float>( node.getAttributeValue<float>("value") );

				else if ( tag == "double" )
					mParams[k].setValue<double>( node.getAttributeValue<double>("value") );

				else if ( tag == "Vec2f" )
					mParams[k].setValue<ci::Vec2f>( ci::Vec2f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );
				
//				else if ( tag == "Vec2i" )
//					mParams[k].setValue<ci::Vec2i>( ci::Vec2i( node.getAttributeValue<int>("x"), node.getAttributeValue<int>("y") ) );
				
				else if ( tag == "Vec3f" )
					mParams[k].setValue<ci::Vec3f>( ci::Vec3f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );
				
//				else if ( tag == "Vec3i" )
//					mParams[k].setValue<ci::Vec3i>( ci::Vec3i( node.getAttributeValue<int>("x"), node.getAttributeValue<int>("y"), node.getAttributeValue<int>("z") ) );
				
				else if ( tag == "bool" )
					mParams[k].setValue<bool>( node.getAttributeValue<bool>("value") );
				
				else if ( tag == "Color" )
					mParams[k].setValue<ci::Color>( ci::Color( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b") ) );
				
				else if ( tag == "ColorA" )
					mParams[k].setValue<ci::ColorA>( ci::ColorA( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b"), node.getAttributeValue<float>("a") ) );
				
				else if ( tag == "Font" )
					mParams[k].setValue<ci::Font>( ci::Font( node.getAttributeValue<std::string>("typeface"), node.getAttributeValue<int>("size") ) );
				
				else if ( tag == "string" )
					mParams[k].setValue<std::string>( node.getAttributeValue<std::string>("value") );
			} 
		}
		
		if ( !paramFound )
		{
			if ( tag == "int" )
				addParam( name, &node.getAttributeValue<int>("value"));
			
//			else if ( tag == "float" )
//				mParams[k].setValue<float>( node.getAttributeValue<float>("value") );
//			
//			else if ( tag == "double" )
//				mParams[k].setValue<double>( node.getAttributeValue<double>("value") );
//			
//			else if ( tag == "Vec2f" )
//				mParams[k].setValue<ci::Vec2f>( ci::Vec2f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );
//			
//			else if ( tag == "Vec3f" )
//				mParams[k].setValue<ci::Vec3f>( ci::Vec3f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );
//			
//			else if ( tag == "bool" )
//				mParams[k].setValue<bool>( node.getAttributeValue<bool>("value") );
//			
//			else if ( tag == "Color" )
//				mParams[k].setValue<ci::Color>( ci::Color( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b") ) );
//			
//			else if ( tag == "ColorA" )
//				mParams[k].setValue<ci::ColorA>( ci::ColorA( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b"), node.getAttributeValue<float>("a") ) );
//			
//			else if ( tag == "Font" )
//				mParams[k].setValue<ci::Font>( ci::Font( node.getAttributeValue<std::string>("typeface"), node.getAttributeValue<int>("size") ) );
//			
//			else if ( tag == "string" )
//				mParams[k].setValue<std::string>( node.getAttributeValue<std::string>("value") );
//			
			
			ci::app::console() << "params \"" + name << "\" only exist in the XML file" << std::endl;
		}
	};
	
	
	bool	hasParam(std::string name);
	
	
	void	addParam(const std::string &name, int *param)			{ mParams.push_back( Param( name, param, Param::PARAM_INT ) );		};
	void	addParam(const std::string &name, float *param)			{ mParams.push_back( Param( name, param, Param::PARAM_FLOAT ) );	};
	void	addParam(const std::string &name, double *param)		{ mParams.push_back( Param( name, param, Param::PARAM_DOUBLE ) );	};
	void	addParam(const std::string &name, bool *param)			{ mParams.push_back( Param( name, param, Param::PARAM_BOOL ) );		};
	void	addParam(const std::string &name, ci::Vec2f *param)		{ mParams.push_back( Param( name, param, Param::PARAM_VEC2F ) );	};
	void	addParam(const std::string &name, ci::Vec3f *param)		{ mParams.push_back( Param( name, param, Param::PARAM_VEC3F ) );	};
	void	addParam(const std::string &name, ci::Color *param)		{ mParams.push_back( Param( name, param, Param::PARAM_COLOR ) );	};
	void	addParam(const std::string &name, ci::ColorA *param)	{ mParams.push_back( Param( name, param, Param::PARAM_COLORA ) );	};
	void	addParam(const std::string &name, ci::Font *param)		{ mParams.push_back( Param( name, param, Param::PARAM_FONT ) );	};
	void	addParam(const std::string &name, std::string *param)	{ mParams.push_back( Param( name, param, Param::PARAM_STRING ) );	};
	
	
	void resetParam( std::string name )
	{
		bool paramFound = true;
		for(int k=0; k < mXmlNodes.size(); k++)
		{
			if ( mXmlNodes[k].getAttributeValue<std::string>("name") == name )
			{
				paramFound = true;
				loadParam( mXmlNodes[k] );
			}
		}
		
		if ( !paramFound )
			ci::app::console() << "couldn't find \"" + name + "\" in the XML file!" << std::endl;
	};
	
	
	void drawDebug()
	{		
		ci::TextLayout textLayout = ci::TextLayout();
		textLayout.setColor( ci::Color::white() );
		textLayout.clear( ci::Color::black() );
		textLayout.setBorder(10, 10);
		textLayout.setLeadingOffset(3);
		textLayout.setFont( ci::Font("Arial", 12) );
		
		textLayout.addLine( "XML SETTINGS" );
		textLayout.addLine( " " );
		
		for( int k=0; k < mParams.size(); k++)
			textLayout.addLine( mParams[k].getType() + " \t" + mParams[k].getName() + " \t" + mParams[k].getAsString() );
		
		ci::gl::Texture tex = ci::gl::Texture(textLayout.render(true));
		ci::gl::draw(tex);
	};
	
	void load() 
	{		
		for(ci::XmlTree::Iter nodeIt = mXmlAsset.begin(); nodeIt != mXmlAsset.end(); ++nodeIt)
		{
			mXmlNodes.push_back( *nodeIt );
			loadParam( *nodeIt );			
		}
		
		for (int k=0; k < mParams.size(); k++ ) 
		{
			bool paramFound = false;
			
			for(int i=0; i < mXmlNodes.size(); i++)
				if ( mXmlNodes[i].getAttributeValue<std::string>("name") == mParams[k].getName() )
					paramFound = true;
			
			if ( !paramFound )
				ci::app::console() << "couldn't find param \"" + mParams[k].getName() << "\" in the XML file" << std::endl;
		}
		
		ci::app::console() << "XML settings loaded from: " << mFilename << std::endl;
	};
	
	
	
	/*
	void load() 
	{		
		for(ci::XmlTree::Iter nodeIt = mXmlAsset.begin(); nodeIt != mXmlAsset.end(); ++nodeIt)
		{
			mXmlNodes.push_back( *nodeIt );
			setParamWithNode( *nodeIt );			
		}
		
		for (int k=0; k < mParams.size(); k++ ) 
		{
			bool paramFound = false;
			
			for(int i=0; i < mXmlNodes.size(); i++)
				if ( mXmlNodes[i].getAttributeValue<std::string>("name") == mParams[k].getName() )
					paramFound = true;
			
			if ( !paramFound )
				ci::app::console() << "couldn't find param \"" + mParams[k].getName() << "\" in the XML file" << std::endl;
		}
		
		ci::app::console() << "XML settings loaded from: " << mFilename << std::endl;
	};
	*/
	
//	void save()
//	{
//		
//	};
	
private:
	std::string					mFilename;
	std::vector<Param>			mParams;
	ci::XmlTree					mXmlAsset;
	std::vector<ci::XmlTree>	mXmlNodes;
};
