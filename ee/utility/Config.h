// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>

#include <ee/utility/HashUtils.h>

namespace ee
{
	class File;

	// Configuration options in the Elevation engine are handled by a system
	// with two halves. The first half is the ConfigDB. The ConfigDB is
	// initialized from a .cfg file. It contains the list of configuration
	// options that are specified in that file. The second half is the
	// ConfigProperty list. It's the list of config variables defined in
	// the code. When a .cfg is file read in, for every config variable
	// defined there if a matching ConfigProperty is found then that variable
	// is updated to reflect the value stored in the .cfg file. This way
	// these two systems - what's in the .cfg file and what the code's
	// looking for - don't need to be kept in lockstep sync.

	//
	// This manages key/value pairs organized into sections.  The default value
	// that the code passes in when it requests a value is saved, so we can tell
	// which ones are customized at write time (see Load/SaveConfigDB for details).
	//
	class ConfigDB
	{
	public:
		void Clear( void );

		void SetValue( const std::string_view& section, const std::string_view& key, const std::string_view& value );
		std::string_view GetValue( const std::string_view& section, const std::string_view& key, const std::string_view& defaultValue );

		bool HasValue( const std::string_view& section, const std::string_view& key ) const;

		bool HasSection( const std::string_view& section ) const
		{
			return mSections.contains( HashUtils::CalculateStringHash( section ) );
		};

		// SaveConfig will write only this config properties set to non-default
		// values, unless the 'defaults' parameter is set to true, in which
		// case every config property will be written with their default values
		// assigned to them. This is used to document the set of all properties.
		bool SaveConfig( std::shared_ptr< File >& file, bool defaults = false );
		bool LoadConfig( std::shared_ptr< File >& file );

	private:
		void SetValueFromFile( const std::string_view& section, const std::string_view& key, const std::string_view& value );

		class ValuePair
		{
		public:
			// Storing the section name here; section names will be duplicated
			// because the map key is the hashed section name, not the section
			// name itself so we can't just use iterator::first for the name
			std::string mSection;
			std::string mDefaultValue;
			std::string mCurrentValue;
			bool mSetDefault = false;
			bool mSetCurrent = false;
		};

		// Because C++ requires that you create a std::string if you want to use
		// a std::string_view to look up a key in a std::map<std::string, ...>
		// we hash the key strings instead. Yes, that means that we'll need to
		// calculate a hash every time we want to do a lookup, but at least
		// that means that we aren't instantiating an ephemeral std::string
		// upon every lookup, allocating memory and doing locale checks.
		typedef std::map< uint64_t, ValuePair > Values;
		typedef std::map< uint64_t, Values > Sections;

		Sections mSections;
	};

	//
	// A helper so we can define global string values without causing memory
	// leaks.  It supports both std::string_view and const char*, but if you
	// want a std::string you'll need to create it yourself.
	//
	class StaticString
	{
	public:
		void operator = ( const char* newValue )
		{
			SetValue( newValue );
		}

		void operator = ( const std::string_view& newValue )
		{
			SetValue( newValue.data() );
		}

		operator const char* ( void ) const
		{
			return mStringPtr;
		}

		operator std::string_view ( void ) const
		{
			return std::string_view( mStringPtr, mLength );
		}

	protected:
		void SetValue( const char* value );

	protected:
		char*	mStringPtr;
		size_t	mLength;

	}; // class StaticString

	// Templated so we can define the max string length
	template< size_t Length >
	class StaticStringBuffer : public StaticString
	{
	public:
		StaticStringBuffer( const char* initalValue )
		{
			mStringPtr = mString;
			mLength = Length;
			SetValue( initalValue );
		}

	private:
		char mString[ Length ];
	};

	// Used to hold a pointer to a local variable that is driven by the
	// config file data. See: REGISTER_CONFIG_PROPERTY() below
	class ConfigProperty
	{
	public:
		// These are the types that config variables can have
		enum ValueType
		{
			kBool,
			kInt32,
			kUInt32,
			kFloat,
			kString,
			kTypeCount
		};

		ConfigProperty( bool* value, const std::string_view& section, const std::string_view& key )
			: mType( kBool )
			, mBool( value )
		{
			Initialize( section, key );
		}

		ConfigProperty( int32_t* value, const std::string_view& section, const std::string_view& key )
			: mType( kInt32 )
			, mInt32( value )
		{
			Initialize( section, key );
		}

		ConfigProperty( uint32_t* value, const std::string_view& section, const std::string_view& key )
			: mType( kUInt32 )
			, mUInt32( value )
		{
			Initialize( section, key );
		}

		ConfigProperty( float* value, const std::string_view& section, const std::string_view& key )
			: mType( kFloat )
			, mFloat( value )
		{
			Initialize( section, key );
		}

		ConfigProperty( StaticString* value, const std::string_view& section, const std::string_view& key )
			: mType( kString )
			, mString( value )
		{
			Initialize( section, key );
		}

		ValueType			mType;
		union
		{
			bool*			mBool;
			int32_t*		mInt32;
			uint32_t*		mUInt32;
			float*			mFloat;
			StaticString*	mString;
		};

		std::string			mSection;
		std::string			mKey;
		ConfigProperty*		mNext;

		static ConfigProperty* mList;

		void SetValue( const bool& value ) const
		{
			eeAssert( mType == kBool, "Bad value type" );
			*mBool = value;
		}

		void SetValue( const int32_t& value ) const
		{
			eeAssert( mType == kInt32, "Bad value type" );
			*mInt32 = value;
		}

		void SetValue( const uint32_t& value ) const
		{
			eeAssert( mType == kUInt32, "Bad value type" );
			*mUInt32 = value;
		}

		void SetValue( const float& value ) const
		{
			eeAssert( mType == kFloat, "Bad value type" );
			*mFloat = value;
		}

		void SetValue( const std::string_view& value ) const
		{
			eeAssert( mType == kString, "Bad value type" );
			*mString = value;
		}

		void FromString( const std::string_view& value ) const;
		std::string ToString( void ) const;

	private:
		void Initialize( const std::string_view& section, const std::string_view& key );

	}; // class ConfigProperty

	//
	// Each config variable defined by code is represented in two ways-
	// one as a ConfigProperty in the list of all known ConfigPropertys,
	// and one as a global variable that is the code-side representation
	// of that config variable and its value. The ConfigProperty contains
	// properties like the name, category, and current and default values.
	// It's what's written to the .cfg file and it's initialized from what's
	// stored in the .cfg file. If it's not referenced by the .cfg file then
	// its value will be its default value. Code should use the global variable
	// declared when the config property is registered.
	//
	// The golbal variable is const, so you'll have to set it with
	// SET_CONFIG_PROPERTY. You could obviously access the hidden property
	// without it, but please use it so it's easier to track things.
	//
	// REGISTER_CONFIG_PROPERTY( "Physics", "TerminalVelocity", gTerminalVelocity, float, 1599.f );
	//
	// You can now use the variable gTerminalVelocity (or whatever)
	// in your code as a const.
	//
	#define REGISTER_CONFIG_PROPERTY( SECTION, KEY, VARNAME, TYPE, DEFAULT_VALUE )			\
		TYPE VARNAME##_hidden( DEFAULT_VALUE );												\
		const TYPE& VARNAME( VARNAME##_hidden );											\
		ee::ConfigProperty config_shadow_##VARNAME##_instance( &VARNAME##_hidden, SECTION, KEY );

	#define EXTERN_CONFIG_PROPERTY( VARNAME, TYPE )											\
		extern const TYPE& VARNAME;															\
		extern TYPE VARNAME##_hidden;

	//
	// Strings work a little differently, since we don't want to cause memory
	// leaks by defining them at global scope.  You need to specify the max
	// string length when you declare it, like so:
	//
	// REGISTER_CONFIG_STRING( "Debug", "DefaultMap", gDefaultMap, 255, "MyDefaultMap" );
	//
	#define REGISTER_CONFIG_STRING( SECTION, KEY, VARNAME, LENGTH, DEFAULT_VALUE )			\
		StaticStringBuffer< LENGTH > VARNAME##_hidden( DEFAULT_VALUE );						\
		const char* VARNAME( VARNAME##_hidden );											\
		ee::ConfigProperty config_shadow_##VARNAME##_instance( &VARNAME##_hidden, SECTION, KEY );

	#define EXTERN_CONFIG_STRING( VARNAME )													\
		extern const char* VARNAME;															\
		extern StaticString VARNAME##_hidden;

	//
	// Use this macro to set any type of config property
	//
	#define SET_CONFIG_PROPERTY( VARNAME, VALUE )											\
		VARNAME##_hidden = VALUE

	class Config
	{
	public:
		void SaveConfig( std::shared_ptr< File >& file );
		void LoadConfig( std::shared_ptr< File >& file );

		void SetValue( const std::string_view& section, const std::string_view& key, const std::string_view& value );
		std::string_view GetValue( const std::string_view& section, const std::string_view& key, const std::string_view& defaultValue );

		bool HasValue( const std::string_view& section, const std::string_view& key ) const;
		bool HasSection( const std::string_view& section ) const
		{
			return mDB.HasSection( section );
		};

	private:
		void LoadFromConfigDB( ConfigDB& cfg );
		void SaveToConfigDB( ConfigDB& cfg );

		ConfigProperty* Get( const std::string_view& section, const std::string_view& key );

		void SetString( const std::string_view& section, const std::string_view& key, const std::string_view& value );
		std::string GetString( const std::string_view& section, const std::string_view& key );

		ConfigDB mDB;
	};

} // namespace ee
