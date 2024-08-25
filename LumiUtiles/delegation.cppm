export module LumiUtiles:Async:delegation;

import std;

export namespace LumiUtiles::Async {

	template<typename Signature>
	class Delegation;

	template<typename ReturnType, typename... Args>
	class Delegation<ReturnType(Args...)>
	{
	protected:
		using FunctionType = std::function<ReturnType(Args...)>;

		std::vector<FunctionType> functions;

	public:

		Delegation():
			functions()
		{}
		Delegation(const Delegation& other):
			functions(other.functions)
		{}
		Delegation(Delegation&& other) :
			functions(std::move(other.functions))
		{}
		Delegation(const std::vector<FunctionType>& funcs) :
			functions(funcs)
		{}

		void operator+=(FunctionType func)
		{
			functions.push_back(func);
		}

		void operator-=(FunctionType func)
		{
			auto it = std::remove_if(functions.begin(), functions.end(),
				[&func](const FunctionType& f) { 
					return f == func; 
				}
			);
			functions.erase(it, functions.end());
		}

		std::list<ReturnType> operator()(Args... args)
		{
			std::list<ReturnType> result{};
			std::list<std::exception_ptr> exceptions{};
			for (auto& func : functions)
			{
				const Args& _args = args;
				try
				{
					result.push_back(func(_args...));
				}
				catch (...)
				{
					exceptions.push_back(std::current_exception());
				}
			}
			if (exceptions.empty()){
				return std::move(result);
			}
			else {
				throw std::exception(std::make_pair(result, exceptions));
			}
		}

		auto begin() { return functions.begin(); }
		auto end() { return functions.end(); }
	};

}/// End of namespace LumiUtiles::Async