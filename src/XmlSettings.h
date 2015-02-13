/*
 *  XmlSettings.h
 *
 *  Created by Andrea Cuius
 *  The MIT License (MIT)
 *  Copyright (c) 2014 Nocte Studio Ltd.
 *
 *  www.nocte.co.uk
 *
 */

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"

/*
 <?xml version="1.0" encoding="utf-8"?>
 <settings>
 <float name="resolution" width="1706" height="960" />
 <bool name="runDebugMode" value="1" />
 </settings>
 */

namespace nocte { 
	
    typedef std::shared_ptr<class XmlSettings>          XmlSettingsRef;
    
	static const std::string typeString[10] = { 
		"int", 
		"float", 
		"double", 
		"bool", 
		"vec2", 
		"vec3", 
		"Color", 
		"ColorA", 
		"Font", 
		"string" 
	};


	class Param {
		
		friend class XmlSettings;
		
	public:
		
		enum ParamType {
			PARAM_INT,
			PARAM_FLOAT,
			PARAM_DOUBLE,
			PARAM_BOOL,
			PARAM_VEC2,
			PARAM_VEC3,
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
			else if ( mType == PARAM_VEC2 )
				return ci::toString(*static_cast<ci::vec2*>(mParam));
			else if ( mType == PARAM_VEC3 )
				return ci::toString(*static_cast<ci::vec3*>(mParam));
			else if ( mType == PARAM_COLOR )
				return ci::toString(*static_cast<ci::Color*>(mParam));
			else if ( mType == PARAM_COLORA )
				return ci::toString(*static_cast<ci::ColorA*>(mParam));
			else if ( mType == PARAM_FONT )
				return "TODO !!!!";//toString(*static_cast<ColorA*>(mParam));
			else if ( mType == PARAM_STRING )
				return *static_cast<std::string*>(mParam);
            
            return "";
		};

		
		ci::XmlTree getXmlNode()
		{
			ci::XmlTree node( getType(), "" );
			node.setAttribute( "name", mName );
			
			if ( mType == PARAM_INT )
				node.setAttribute( "value", *static_cast<int*>(mParam) );
			else if ( mType == PARAM_FLOAT )
				node.setAttribute( "value", *static_cast<float*>(mParam) );
			else if ( mType == PARAM_DOUBLE )
				node.setAttribute( "value", *static_cast<double*>(mParam) );
			else if ( mType == PARAM_BOOL )
				node.setAttribute( "value", *static_cast<bool*>(mParam) );
			else if ( mType == PARAM_VEC2 )
			{
				node.setAttribute( "x", static_cast<ci::vec2*>(mParam)->x );
				node.setAttribute( "y", static_cast<ci::vec2*>(mParam)->y );
			}
			else if ( mType == PARAM_VEC3 )
			{
				node.setAttribute( "x", static_cast<ci::vec3*>(mParam)->x );
				node.setAttribute( "y", static_cast<ci::vec3*>(mParam)->y );
				node.setAttribute( "z", static_cast<ci::vec3*>(mParam)->z );
			}
			else if ( mType == PARAM_COLOR )
			{
				node.setAttribute( "r", static_cast<ci::Color*>(mParam)->r );
				node.setAttribute( "g", static_cast<ci::Color*>(mParam)->g );
				node.setAttribute( "b", static_cast<ci::Color*>(mParam)->b );
			}
			else if ( mType == PARAM_COLORA )
			{
				node.setAttribute( "r", static_cast<ci::ColorA*>(mParam)->r );
				node.setAttribute( "g", static_cast<ci::ColorA*>(mParam)->g );
				node.setAttribute( "b", static_cast<ci::ColorA*>(mParam)->b );
				node.setAttribute( "a", static_cast<ci::ColorA*>(mParam)->a );
			}
			else if ( mType == PARAM_FONT )
			{
				node.setAttribute( "typeface", static_cast<ci::Font*>(mParam)->getName() );
				node.setAttribute( "size", static_cast<ci::Font*>(mParam)->getSize() );
			}
			else if ( mType == PARAM_STRING )
				node.setAttribute( "value", *static_cast<std::string*>(mParam) );
			
			return node;
			
		};
		
		
	protected:
		
        std::string	mName;
		void		*mParam;
		ParamType	mType;

	};


	class XmlSettings {
		
	public:
		
        static XmlSettingsRef create( ci::fs::path filePath = ci::fs::path("") ) { return XmlSettingsRef( new XmlSettings( filePath ) ); }
        
		~XmlSettings() {}
        
		void parseNode( ci::XmlTree node );
		
		bool hasParam( std::string name );
		
		Param* getParam( std::string name );
		
		void	addParam( const std::string &name, int *param )			{ addOrBind(name, param, Param::PARAM_INT); };
		void	addParam( const std::string &name, float *param )		{ addOrBind(name, param, Param::PARAM_FLOAT); };
		void	addParam( const std::string &name, double *param )		{ addOrBind(name, param, Param::PARAM_DOUBLE); };
		void	addParam( const std::string &name, bool *param )		{ addOrBind(name, param, Param::PARAM_BOOL); };
		void	addParam( const std::string &name, ci::vec2 *param )	{ addOrBind(name, param, Param::PARAM_VEC2); };
		void	addParam( const std::string &name, ci::vec3 *param )	{ addOrBind(name, param, Param::PARAM_VEC3); };
		void	addParam( const std::string &name, ci::Color *param )	{ addOrBind(name, param, Param::PARAM_COLOR); };
		void	addParam( const std::string &name, ci::ColorA *param )	{ addOrBind(name, param, Param::PARAM_COLORA); };
		void	addParam( const std::string &name, ci::Font *param )	{ addOrBind(name, param, Param::PARAM_FONT); };
		void	addParam( const std::string &name, std::string *param )	{ addOrBind(name, param, Param::PARAM_STRING); };

		
		template <class T>
		T		getValueByName( const std::string &name )
		{ 
			Param *p = getParam(name);
			
			if ( !p )
			{
				ci::app::console() << "param not found: " << name << std::endl;
				exit(-1); // throw exception instead
			}
			
			return *static_cast<T*>( p->mParam ); 
		};
		
		void drawDebug();

		void load( ci::fs::path filePath = ci::fs::path("") );
		
		void save( ci::fs::path filePath );
		
	private:
		
		XmlSettings( ci::fs::path filePath );
        
		void addOrBind( const std::string &name, void *param, Param::ParamType ptype );
        
        
    private:
        
		ci::fs::path                mFilePath;
		std::vector<Param*>			mParams;
        ci::XmlTree					mXmlAsset;
		std::vector<ci::XmlTree>	mXmlNodes;
        

    public:

        static XmlSettings*         getPtr() { return thisPtr; }

    private:

        static  XmlSettings*       thisPtr;
        
	};

}