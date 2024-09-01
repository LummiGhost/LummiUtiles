module LumiUtiles:Exception;

import std;

namespace LumiUtiles::Exception {

	static std::string _at_here(const char* file, int line) noexcept {
		std::stringstream ss;
		ss << file << ":" << line;
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

#define LUMI_EXCEPTION_THROW_AT_HERE _at_here(__FILE__, __LINE__)

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

    public:
		basic_exception() = default;
		basic_exception(const std::string& message) : message(message) {}
		basic_exception(const std::string& message, std::exception_ptr nested_exception) : message(message), nested_exception(nested_exception) {}
		basic_exception(const std::string& message, std::exception_ptr nested_exception, std::map<std::string, std::any> data) : message(message), nested_exception(nested_exception), data(data) {}

		virtual const char* what() const noexcept override {
			//return message.c_str();
			std::stringstream ss;
			ss << type().name() << ": " << message;
			ss << " \tAt: " << at_where;
			if (!data.empty()) {
				//ss << "\nData:";
				for (const auto& [key, value] : data) {
					ss << "\n\t" << key << ":\t" << value.type().name();
				}
			}
			if (nested_exception) {
				try {
					std::rethrow_exception(nested_exception);
				}
				catch (const std::exception& e) {
					ss << "\nNested exception: " << e.what();
				}
			}
			return ss.str().c_str();
		}

    };

}