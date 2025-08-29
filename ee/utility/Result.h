// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <utility>
#include <variant>

namespace ee
{
	// A structure to make functions that need to either return a value or an
	// error code pretty - replaces the old 'ErrorType func( ResultType& result )'
	// pattern where result is initialized if and only if func succeeds.
	// Similar to Rust's Result type or C++23's std::expected / std::unexpected
	// classes, with perhaps slightly better syntax?
	template< typename ResultType, typename ErrorType = bool >
	struct [[nodiscard]] Result
	{
		static_assert( !std::is_same_v< ResultType, ErrorType >,
					   "The result type must be different from the error type" );

		Result()
			: mValue( std::monostate{} )
		{}

		Result( const ResultType& result )
			: mValue{ result }
		{}

		Result( ResultType&& result )
			: mValue( std::move( ResultType( std::move( result ) ) ) )
		{}

		Result( const ErrorType& failure )
			: mValue{ failure }
		{}

		Result( ErrorType&& failure )
			: mValue{ std::move( failure ) }
		{}

		const ResultType& GetResult( void ) const
		{
			Validate();
			return std::get< ResultType >( mValue );
		}

		ResultType& GetResult( void )
		{
			Validate();
			return std::get< ResultType >( mValue );
		}

		const ErrorType& GetError( void ) const
		{
			Validate();
			return std::get< ErrorType >( mValue );
		}

		// Use the dereference operator as a shorthand for GetResult()
		ResultType& operator * ()
		{
			return GetResult();
		}

		const ResultType& operator * () const
		{
			return GetResult();
		}

		// Same with the member-access operator as a shorthand for GetResult()
		ResultType* operator -> ()
		{
			return &GetResult();
		}

		const ResultType* operator -> () const
		{
			return &GetResult();
		}

		// Enable a shorthand for testing if Result holds a ResultType or ErrorType:
		// if( result ) { /* success! do something with ResultType */ }
		explicit operator bool () const
		{
			return std::holds_alternative< ResultType >( mValue );
		}

		template< typename T >
		bool operator == ( const Result& other ) const
		{
			return mValue == other.mValue;
		}

		template< typename T >
		bool operator == ( const T& val ) const
		{
			Validate();

			// Strip off any potential qualifiers or decorations on T
			using D = std::decay_t< T >;

			static constexpr bool isResult = std::is_same_v< D, ResultType >;
			static constexpr bool isError = std::is_same_v< D, ErrorType >;
			static_assert( !( isResult && isError ), "Ambiguous comparison - the type being compared to is the same as both the result and error types." );

			if constexpr ( isResult )
			{
				// std::get_if() will always return a pointer type
				if( auto* v = std::get_if< ResultType >( &mValue ) )
					return *v == val;
			}
			else if constexpr ( isError )
			{
				if( auto* v = std::get_if< ErrorType >( &mValue ) )
					return *v == val;
			}

			return false;
		}

		template< typename T >
		bool operator != ( const T& val ) const
		{
			return !( *this == val );
		}

	private:
		constexpr void Validate( void ) const
		{
			eeAssert( !std::holds_alternative< std::monostate >( mValue ), "Result validation failed - no result or error was assigned" );
		}

		std::variant< std::monostate, ResultType, ErrorType > mValue;

	}; // struct Result

} // namespace ee
