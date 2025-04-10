// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <memory>
#include <charconv>

#include "Config.h"

#include <ee/io/File.h>
#include <ee/io/FileInputStream.h>
#include <ee/io/FileOutputStream.h>
#include <ee/io/StreamReader.h>
#include <ee/io/StreamWriter.h>
#include <ee/utility/HashUtils.h>
#include <ee/utility/StringUtils.h>

using namespace ee;

//******************************************************************************
//******************************************************************************

void ConfigDB::Clear( void )
{
	mSections.clear();
}

void ConfigDB::SetValue( const std::string_view& section, const std::string_view& key, const std::string_view& value )
{
	uint64_t sectionHash = HashUtils::CalculateStringHash( section );
	uint64_t keyHash = HashUtils::CalculateStringHash( key );

	Values& values = mSections[ sectionHash ];
	ValuePair& valuePair = values[ keyHash ];
	valuePair.mSetCurrent = true;
	valuePair.mCurrentValue = value;
}

std::string_view ConfigDB::GetValue( const std::string_view& section, const std::string_view& key, const std::string_view& defaultValue )
{
	uint64_t sectionHash = HashUtils::CalculateStringHash( section );
	uint64_t keyHash = HashUtils::CalculateStringHash( key );

	Values& values = mSections[ sectionHash ];
	ValuePair& valuePair = values[ keyHash ];

	// We want to save off the default value the code is passing in if this
	// is the first time, so we can write the defaults file later.
	if( !valuePair.mSetDefault )
	{
		valuePair.mSetDefault = true;
		valuePair.mDefaultValue = defaultValue;

		// We may have already set the current value, if it was read in from a
		// user or shared file. Don't overwrite it with the default in that case
		if( !valuePair.mSetCurrent )
		{
			valuePair.mSetCurrent = true;
			valuePair.mCurrentValue = defaultValue;
		}
	}

	return std::string_view( valuePair.mCurrentValue );
}

bool ConfigDB::HasValue( const std::string_view& section, const std::string_view& key ) const
{
	uint64_t sectionHash = HashUtils::CalculateStringHash( section );

	if( mSections.contains( sectionHash ) )
	{
		const Values& values = mSections.at( sectionHash );
		uint64_t keyHash = HashUtils::CalculateStringHash( key );

		return values.contains( keyHash );
	}

	return false;
}

// SaveConfig will write only this config properties set to non-default
// values, unless the 'defaults' parameter is set to true, in which
// case every config property will be written with their default values
// assigned to them. This is used to document the set of all properties.
bool ConfigDB::SaveConfig( File& file, bool defaults )
{
	const FileStatus& status = file.GetStatus();

	if( !status.IsFile() || status.IsReadOnly() )
		return false;

	std::unique_ptr<FileOutputStream> stream = MakeFileOutputStream( file );
	if( !stream->Open() )
		return false;

	StreamWriter writer( *stream );
	
	for( auto sections : mSections )
	{
		bool wroteHeader = false;

		// Write the key/value pairs for this section
		for( auto values : sections.second )
		{
			ConfigDB::ValuePair& value = values.second;

			// Only write changed values to the .cfg file
			if( defaults || value.mCurrentValue != value.mDefaultValue )
			{
				// Write the section header if we haven't yet (we only write it
				// when we hit a value we're actually going to write, in case
				// there aren't any).
				if( !wroteHeader )
				{
					wroteHeader = true;

					writer.Write( "[" );
					writer.Write( sections.first );
					writer.Write( "]" );
					writer.EndLine();
				}

				// Write the value
				writer.Write( values.first );
				writer.Write( " = " );
				writer.Write( defaults ? value.mDefaultValue : value.mCurrentValue );
				writer.EndLine();

			} // if( value.mCurrentValue != value.mDefaultValue )

		} // for( auto values : sections.second )

		if( wroteHeader )
			writer.EndLine();

	} // for( auto sections : mSections )

	writer.Close();

	return true;
}

bool ConfigDB::LoadConfig( File& file )
{
	char sec[ 256 ];
	std::string sectionName;

	// load values from the config file
	std::unique_ptr<FileInputStream> stream = MakeFileInputStream( file );
	if( !stream->Open() )
		return false;

	StreamReader reader( *stream );

	while( reader.Ready() )
	{
		std::string line = reader.ReadLn();

		if( line.empty() )
			continue;

		// Skip all comments
		if( line[ 0 ] == '#' )
			continue;

		// parse section header
		int ret = sscanf( line.c_str(), "[%[^]]]", sec );
		if( ret == 1 && *sec )
		{
			sectionName = sec;
			TrimWhitespace( sectionName );
			continue;
		}

		// Does this line assign a value to a property?
		auto equals = line.find_first_of( '=' );
		if( equals != std::string::npos )
		{
			// If so, extract the property name (the key) and its value.
			// The key is anything before the = and the value is anything after.
			std::string key = line.substr( 0, equals );
			std::string value = line.substr( equals + 1 );
			TrimWhitespace( key );
			TrimWhitespace( value );

			SetValueFromFile( sectionName, key, value );
		}

	} // while( reader.Ready() )

	return true;
}

void ConfigDB::SetValueFromFile( const std::string_view& section, const std::string_view& key, const std::string_view& value )
{
	uint64_t sectionHash = HashUtils::CalculateStringHash( section );
	uint64_t keyHash = HashUtils::CalculateStringHash( key );

	Values& values = mSections[ sectionHash ];
	ValuePair& valuePair = values[ keyHash ];
	valuePair.mSetCurrent = true;
	valuePair.mCurrentValue = value;
}

//******************************************************************************
//******************************************************************************

void StaticString::SetValue( const char* value )
{
	size_t valueSize = strlen( value );

	// Subtract one because we assume that mString is zero-terminated
	if( valueSize > mLength - 1 )
	{
		*mStringPtr = '\0';
		eeAssertAlways( "StaticString passed %d byte string that exceeds its buffer size of %d bytes", valueSize, mLength );
	}
	else
	{
		strcpy( mStringPtr, value );
	}
}

//******************************************************************************
//******************************************************************************

ConfigProperty* ConfigProperty::mList = NULL;

void ConfigProperty::Initialize( const std::string_view& section, const std::string_view& key )
{
	mSection = std::string( section );
	mKey = std::string( key );

	// now put this element at the head of the list and point our
	// next pointer at whatever used to be the head of the list
	mNext = mList;
	mList = this;
}

void ConfigProperty::FromString( const std::string_view& value ) const
{
	switch (mType)
	{
	case ConfigProperty::kBool:
	{
		int result;
		auto [ptr, ec] = std::from_chars( value.data(), value.data() + value.size(), result );
		if( ec == std::errc() )
			*mBool = result ? true : false;
	}
	break;

	case ConfigProperty::kInt32:
	{
		int result;
		auto [ptr, ec] = std::from_chars( value.data(), value.data() + value.size(), result );
		if( ec == std::errc() )
			*mInt32 = result;
	}
	break;

	case ConfigProperty::kUInt32:
	{
		unsigned int result;
		auto [ptr, ec] = std::from_chars( value.data(), value.data() + value.size(), result );
		if( ec == std::errc() )
			*mUInt32 = result;
	}
	break;

	case ConfigProperty::kFloat:
	{
		float result;
		auto [ptr, ec] = std::from_chars( value.data(), value.data() + value.size(), result );
		if( ec == std::errc() )
			*mFloat = result;
	}
	break;

	case ConfigProperty::kString:
		*mString = std::string( value );
		break;

	} // switch (mType)
}

std::string ConfigProperty::ToString( void ) const
{
	switch( mType )
	{
	case ConfigProperty::kBool:
		return std::to_string( *mBool );

	case ConfigProperty::kInt32:
		return std::to_string( *mInt32 );

	case ConfigProperty::kUInt32:
		return std::to_string( *mUInt32 );

	case ConfigProperty::kFloat:
		return std::to_string( *mFloat );

	case ConfigProperty::kString:
		return std::string( std::string_view( *mString ) );

	} // switch( mType )

	// default:
	return std::string();
}

//******************************************************************************
//******************************************************************************

void Config::SaveConfig( File& file )
{
	// Copy the registered properties to the DB
	SaveToConfigDB( mDB );
	// And then write that DB to file
	mDB.SaveConfig( file );
}

void Config::LoadConfig( File& file )
{
	// Initialize the DB from a .cfg file
	mDB.LoadConfig( file );
	// and then copy those values to any matching registered ConfigProperty objects
	LoadFromConfigDB( mDB );
}

void Config::SetValue( const std::string_view& section, const std::string_view& key, const std::string_view& value )
{
	mDB.SetValue( section, key, value );
}

std::string_view Config::GetValue( const std::string_view& section, const std::string_view& key, const std::string_view& defaultValue )
{
	return mDB.GetValue( section, key, defaultValue );
}

bool Config::HasValue( const std::string_view& section, const std::string_view& key ) const
{
	return mDB.HasValue( section, key );
}

void Config::LoadFromConfigDB( ConfigDB& db )
{
	ConfigProperty* shadowValue = ConfigProperty::mList;
	while( shadowValue != NULL )
	{
		std::string_view curValue = db.GetValue( shadowValue->mSection, shadowValue->mKey, shadowValue->ToString() );
		shadowValue->FromString( curValue );

		shadowValue = shadowValue->mNext;
	}
}

void Config::SaveToConfigDB( ConfigDB& db )
{
	ConfigProperty* shadowValue = ConfigProperty::mList;
	while( shadowValue != NULL )
	{
		db.SetValue( shadowValue->mSection, shadowValue->mKey, shadowValue->ToString() );

		shadowValue = shadowValue->mNext;
	}
}

ConfigProperty* Config::Get( const std::string_view& section, const std::string_view& key )
{
	ConfigProperty* shadowValue = ConfigProperty::mList;
	while( shadowValue != NULL )
	{
		if( section == shadowValue->mSection && key == shadowValue->mKey )
		{
			return shadowValue;
		}

		shadowValue = shadowValue->mNext;
	}
	return NULL;
}

void Config::SetString( const std::string_view& section, const std::string_view& key, const std::string_view& value )
{
	ConfigProperty* shadowValue = Get( section, key );
	if( shadowValue != NULL )
	{
		shadowValue->FromString( value );
	}
}

std::string Config::GetString( const std::string_view& section, const std::string_view& key )
{
	ConfigProperty* shadowValue = Get( section, key );
	if( shadowValue != NULL )
	{
		return shadowValue->ToString();
	}

	return std::string( "0" );
}
