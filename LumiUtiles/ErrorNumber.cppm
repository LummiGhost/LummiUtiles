
module LumiUtiles:Exception;

import std;

namespace LumiUtiles::Exception {

	template<typename T, T value, typename _Template = basic_exception>
	class ErrorNumber:
		public _Template
	{
	protected:

	public:
		using _Template::_Template;
		constexpr static T value() noexcept {
			return value;
		}
	};

}