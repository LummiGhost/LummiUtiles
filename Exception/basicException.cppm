module LumiUtiles:Exception;

import std;

namespace LumiUtiles::Exception {

	static std::string _at_here(const char* file, const char* function, int line) noexcept {
		std::stringstream ss;
		ss << function << '( ' << file << ": " << line << ' )';
		return ss.str();
	}

	template <typename T>
	static std::ostream& print_data_value(std::ostream& ss, const T& value) {
		//ss << value;
		return ss;
	}
	template <>
	static std::ostream& print_data_value<std::string>(std::ostream& ss, const std::string& value) {
		ss << value;
		return ss;
	}

#define LUMI_EXCEPTION_THROW_AT_HERE _at_here(__FILE__, __FUNCTION__, __LINE__)

    class basic_exception :
        public std::exception
    {
    protected:
		constexpr std::type_info const& type() const noexcept {
			return typeid(*this);
		}

        std::string message;
		std::string at_where;
		std::exception_ptr nested_exception;
		std::map<std::string, std::any> data;
#ifndef LUMI_EXCEPTION_NO_STACK_TRACE
		std::vector<std::string> stack_trace;
#endif

    public:
		basic_exception() = default;
		basic_exception(const std::string& message) : message(message) {}
		basic_exception(const std::string& message, std::exception_ptr nested_exception) : message(message), nested_exception(nested_exception) {}
		basic_exception(const std::string& message, std::exception_ptr nested_exception, std::map<std::string, std::any> data) : message(message), nested_exception(nested_exception), data(data) {}

		virtual const char* what() const noexcept override {
			//return message.c_str();
			std::stringstream ss;
			ss << type().name();
			ss << "  At: " << at_where;
			ss << "\n\t" << message;
			//ss << "\nData:";
			for (const auto& [key, value] : data) {
				ss << "\n\t$" << key << " : " << value.type().name();
				/// TODO: print value
			}
			if (nested_exception) {
				try {
					std::rethrow_exception(nested_exception);
				}
				catch (const std::exception& e) {
					ss << "\nCause By: " << e.what();
				}
			}
			return ss.str().c_str();
		}

    };

}