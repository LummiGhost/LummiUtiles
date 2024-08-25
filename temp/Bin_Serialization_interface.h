#pragma once

//#include "net_iostream.h"

namespace Stun {
	namespace Network {
		template<typename T>
		class Net_IOStream;
		using Text_Net_IOStream = Net_IOStream<char>;
	}
}

using namespace Stun::Network;

namespace Stun
{
	namespace Serialization
	{
		namespace Bin
		{

			struct IDumpable
			{
				virtual Text_Net_IOStream& dump(Text_Net_IOStream& out) const = 0;
				virtual const size_t size() const = 0;
				virtual ~IDumpable() {};
			};

			struct ILoadable
			{
				virtual Text_Net_IOStream& load(Text_Net_IOStream& in) = 0;
				virtual const size_t size() const = 0;
				virtual ~ILoadable() {};
			};

			struct IPackageable :
				IDumpable, ILoadable
			{
				virtual ~IPackageable() {};
			};

		}/// Bin
	}/// Serialization
}/// Stun