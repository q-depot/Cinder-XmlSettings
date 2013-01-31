/*
 *  XmlSettings.cpp
 *  XmlSettings
 *
 *  Created by Andrea Cuius on 26/11/2011.
 *  Nocte Studio Copyright 2011 . All rights reserved.
 *	www.nocte.co.uk
 *
 */

#include "cinder/app/AppBasic.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "XmlSettings.h"

using namespace ci;
using namespace app;
using namespace std;

nocte::XmlSettings*      nocte::XmlSettings::thisPtr;    // would be better to use shared_ptr and shared_from_this(), however it seems not working with the static variable.


namespace nocte {
    
    XmlSettings::XmlSettings()
    {
        mFilename               = "";
        XmlSettings::thisPtr    = this;
    };

    void XmlSettings::parseNode( XmlTree node )
    {
        string tag          = node.getTag();
        string name         = node.getAttributeValue<string>("name");
        bool paramFound		= false;
        
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

                else if ( tag == "Vec2f" )
                    mParams[k]->setValue<Vec2f>( Vec2f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );
                
                else if ( tag == "Vec3f" )
                    mParams[k]->setValue<Vec3f>( Vec3f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );
                
                else if ( tag == "bool" )
                    mParams[k]->setValue<bool>( node.getAttributeValue<bool>("value") );
                
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
            
            else if ( tag == "Vec2f" )
                addParam( name, new Vec2f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );
            
            else if ( tag == "Vec3f" )
                addParam( name, new Vec3f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );
            
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
    

    void XmlSettings::load( string filename ) 
    {
        
        if ( filename != "" )
            mFilename = filename;
        
        if ( mFilename == "" )
        {
            app::console() << "cannot load XML settings, specify filename" << endl;
            return;
        }
        
        try {
            mXmlAsset = XmlTree( loadFile(mFilename) );
        }
        catch ( ... ) {
            app::console() << "failed to load XML settings file: " << mFilename << endl;
            return;
        }
        
        list<XmlTree> settings = mXmlAsset.getChild("/settings").getChildren();
        
        for(list<XmlTree>::iterator nodeIt = settings.begin(); nodeIt != settings.end(); ++nodeIt)
            parseNode( *nodeIt );

        app::console() << "XML settings loaded from: " << mFilename << endl;
    };
    
    
    void XmlSettings::save( string filename )
    {	
        if ( filename == "" )
            filename = app::getAppPath().parent_path().generic_string() + "/appSettings.xml";
        
        mFilename = filename;
        
        mXmlAsset = XmlTree("settings", "");
        
        for(int k=0; k < mParams.size(); k++)
            mXmlAsset.push_back( mParams[k]->getXmlNode() );
        
        mXmlAsset.write( writeFile( mFilename ) );
        
        app::console() << "XML settings saved: " << mFilename << endl;
        
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