#ifndef id_h__
#define id_h__

struct BaseID
{
public:
	BaseID( BaseID& id )
	{
		this->val = id.val;
	}

	BaseID()
	{
		val = 0;
	}

	inline bool operator==( const BaseID& rhs  )
	{
		return( this->val == rhs.val );
	}

	inline bool operator==( BaseID& rhs )
	{
		return this->val == rhs.val;
	}

	inline bool operator!=( const BaseID& rhs )
	{
		return( !(*this==rhs));
	}

	inline bool operator==( const unsigned int& rhs  )
	{
		return( this->val == rhs );
	}

	inline bool operator!=( const unsigned int& rhs )
	{
		return( !(*this==rhs));
	}

	inline bool operator=( BaseID arg )
	{
		this->val = arg.val;
	}

	inline bool operator=( unsigned int arg )
	{
		this->val = arg;
	}

	unsigned int _get_value() const
	{
		return val;
	}

private:
	unsigned int val;
};


struct EntID : public BaseID{
	using BaseID::operator=;
	using BaseID::operator==;

};
struct EntGUID : public BaseID{
	using BaseID::operator=;
	using BaseID::operator==;
};
struct PlayerGUID : public BaseID{
	using BaseID::operator=;
	using BaseID::operator==;
};

namespace std {

	template <>
	struct hash<EntID>
	{
		std::size_t operator()(const EntID& k) const
		{
			using std::size_t;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return hash<unsigned int>()( k._get_value() );
		}
	};
}


#endif // id_h__
