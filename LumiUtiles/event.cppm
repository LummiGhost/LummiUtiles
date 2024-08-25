export module LumiUtiles:Async:event;

import std;

//#include "delegation.hpp"

export namespace LumiUtiles::Async
{
	template<typename Signature>
	using EventDelegation = Delegation<Signature>;

	template<typename Signature>
	class Event :
		public std::enable_shared_from_this<Event>
	{
	protected:
		EventDelegation<Signature>& delegation;
		Delegation<void(void)> destroy_callbacks;

	public:
		Event(EventDelegation<Signature>& _delegation):
			delegation(_delegation),
			destroy_callbacks()
		{}
		Event() = delete;
		Event(const Event&) = delete;
		Event(Event&&) = delete;

		~Event() {
			destroy_callbacks();
		}

		class Listener;

		Listener Register(std::function<Signature> callback)
		{
			delegation += callback;
			return Listener(shared_from_this(), callback);
		}

		void unRegister(std::function<Signature> callback) {
			delegation -= callback;
		}

		void unRegister(const Listener& listener) {
			delegation -= listener.callback;
		}

		Event& operator+= (std::function<Signature> callback) {
			delegation += callback;
			return *this;
		}

		Event& operator-=(std::function<Signature> callback) {
			delegation -= callback;
			return *this;
		}

		friend class Listener;
	};

	template<typename Signature>
	class Event<Signature>::Listener {
	protected:
		Listener(std::weak_ptr<Event> _event, std::function<Signature> _callback) :
			m_event(_event),
			callback(_callback)
		{}

		std::weak_ptr<Event> m_event;
		std::function<Signature> callback;

	public:

		Listener(Listener&& other):
			m_event(std::move(other.m_event)),
			callback(std::move(other.callback))
		{}

		Listener onDestroy(std::function<void(void)> _callback) {
			auto event = m_event.lock();
			if (event) {
				event->destroy_callbacks += _callback;
			}
			return std::move(*this);
		}

		void unRegister() {
			auto event = m_event.lock();
			if (event) {
				event->unRegister(callback);
				m_event.reset();
			}
		}

		bool isValid() {
			return !m_event.expired();
		}

		~Listener() {
			unRegister();
		}
	};

}/// End of namespace LumiUtiles::Async