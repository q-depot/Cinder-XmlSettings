/*
 *  XmlSettings.cpp
 *
 *  Created by Andrea Cuius
 *  The MIT License (MIT)
 *  Copyright (c) 2014 Nocte Studio Ltd.
 *
 *  www.nocte.co.uk
 *
 */

#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "XmlSettings.h"

using namespace ci;
using namespace app;
using namespace std;

nocte::XmlSettings*      nocte::XmlSettings::thisPtr;    // would be better to use shared_ptr and shared_from_this(), however it seems not working with the static variable.


namespace nocte {
    
    XmlSettings::XmlSettings( ci::fs::path filePath )
    {
        mFilePath               = filePath;
        XmlSettings::thisPtr    = this;

        load( mFilePath );
    };

    void XmlSettings::parseNode( XmlTree node )
    {        
        string  tag         = node.getTag();
        string  name        = node.getAttributeValue<string>("name");
        bool    paramFound	= false;
        
        for( int k=0; k < mParams.size(); k++)
        {				
            if ( mParams[k]->getName() == name )
            {
                paramFound = true;
                
                if ( tag == "int" )
                    mParams[k]->setValue<int>( node.getAttributeValue<int>("value") );
                
                else if ( tag == "float" )
                    mParams[k]->setValue<float>( node.getAttributeValue<float>("value") );

                else if ( tag == "double" )
                    mParams[k]->setValue<double>( node.getAttributeValue<double>("value") );

                else if ( tag == "vec2" )
                    mParams[k]->setValue<vec2>( vec2( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );
                
                else if ( tag == "vec3" )
                    mParams[k]->setValue<vec3>( vec3( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );
                
                else if ( tag == "bool" )
                {
                    console() << "parse bool: " << name << " " << node.getAttributeValue<bool>("value") << endl;
                    mParams[k]->setValue<bool>( node.getAttributeValue<bool>("value") );
                }
                else if ( tag == "Color" )
                    mParams[k]->setValue<Color>( Color( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b") ) );
                
                else if ( tag == "ColorA" )
                    mParams[k]->setValue<ColorA>( ColorA( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b"), node.getAttributeValue<float>("a") ) );
                
                else if ( tag == "Font" )
                    mParams[k]->setValue<Font>( Font( node.getAttributeValue<string>("typeface"), node.getAttributeValue<int>("size") ) );
                
                else if ( tag == "string" )
                    mParams[k]->setValue<string>( node.getAttributeValue<string>("value") );
            } 
        }
        
        if ( !paramFound )
        {
            if ( tag == "int" )
                addParam( name, new int(node.getAttributeValue<int>("value")) );
            else if ( tag == "float" )
                addParam( name, new float(node.getAttributeValue<float>("value")) );
            
            else if ( tag == "double" )
                addParam( name, new double(node.getAttributeValue<double>("value")) );
            
            else if ( tag == "vec2" )
                addParam( name, new vec2( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );
            
            else if ( tag == "vec3" )
                addParam( name, new vec3( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );
            
            else if ( tag == "bool" )
                addParam( name, new bool(node.getAttributeValue<bool>("value")) );
            
            else if ( tag == "Color" )
                addParam( name, new Color( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b") ) );
            
            else if ( tag == "ColorA" )
                addParam( name, new ColorA( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b"), node.getAttributeValue<float>("a") ) );
            
            else if ( tag == "Font" )
                addParam( name, new Font( node.getAttributeValue<string>("typeface"), node.getAttributeValue<int>("size") ) );
            
            else if ( tag == "string" )
                addParam( name, new string(node.getAttributeValue<string>("value")) );
        }
    };
    
    
    bool XmlSettings::hasParam(string name)
    {
        for( int k=0; k < mParams.size(); k++)
            if ( mParams[k]->getName() == name )
                return true;
        
        return false;
    };
    
    
    Param* XmlSettings::getParam(string name) 
    {
        for( int k=0; k < mParams.size(); k++)
            if ( mParams[k]->getName() == name )
                return mParams[k];
        
        return NULL;
    };
    

    void XmlSettings::drawDebug()
    {
        /*
        TextLayout textLayout = TextLayout();
        textLayout.setColor( Color::white() );
        textLayout.clear( Color::black() );
        textLayout.setBorder(10, 10);
        textLayout.setLeadingOffset(3);
        textLayout.setFont( Font("Arial", 12) );
        
        textLayout.addLine( "XML SETTINGS" );
        textLayout.addLine( " " );
        
        for( int k=0; k < mParams.size(); k++)
            textLayout.addLine( mParams[k]->getType() + " \t" + mParams[k]->getName() + " \t" + mParams[k]->getAsString() );
        
        gl::Texture tex = gl::Texture(textLayout.render(true));
        gl::draw(tex);
         */
    };
    

    void XmlSettings::load( fs::path filePath )
    {
        mFilePath = filePath;
        
        if ( mFilePath.empty() )
        {
            app::console() << "cannot load XML Settings, specify filename" << endl;
            return;
        }
        
        try {
            mXmlAsset = XmlTree( loadFile( mFilePath ) ).getChild( "/settings" );
        }
        catch ( ... ) {
            app::console() << "failed to load XML settings file: " << mFilePath.generic_string() << endl;
            return;
        }
        
        for( XmlTree::ConstIter item = mXmlAsset.begin(); item != mXmlAsset.end(); ++item )
        {
            parseNode( *item );
        }
        app::console() << "XML settings loaded from: " << mFilePath.generic_string() << endl;
    };
    
    
    void XmlSettings::save( fs::path filePath )
    {	        
        mXmlAsset = XmlTree("settings", "");
        
        for(int k=0; k < mParams.size(); k++)
            mXmlAsset.push_back( mParams[k]->getXmlNode() );
        
        mXmlAsset.write( writeFile( filePath ) );
        
        app::console() << "XML settings saved: " << mFilePath.generic_string() << endl;
    };
    

    void XmlSettings::addOrBind( const string &name, void *param, Param::ParamType ptype )
    {
        Param *p = getParam(name);
        if ( p )
            p->mParam = param;
        else
            mParams.push_back( new Param( name, param, ptype ) );
    }
	
}