#pragma once

// #include "pch.h"

#include <functional>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <exception>
#include <iostream>

namespace Stun {
	namespace Utils {

		template<typename Signature>
		class Promise;

		template<typename Return, typename... Args>
		class Promise<Return(Args...)> :
			public std::enable_shared_from_this<Promise<Return(Args...)>>
		{
		protected:
			std::function<Return(Args...)> task;
			std::function<void(Return)> callback;
			std::function<bool(std::exception_ptr)> error_callback;
			std::exception_ptr exception;
			std::thread thread;
			std::mutex mutex;

			void resolve(Args... args) {
				auto shared_this = this->shared_from_this();
				try {
					Return result = std::invoke(shared_this->task, args...);
					std::lock_guard<std::mutex> lock(shared_this->mutex);
					if (shared_this->callback) {
						shared_this->callback(result);
					}
				}
				catch (...) {
					shared_this->exception = std::current_exception();
					std::lock_guard<std::mutex> lock(shared_this->mutex);
					if (shared_this->error_callback && shared_this->error_callback(shared_this->exception)) {
						shared_this->exception = nullptr;
					}
				}
			}

			Promise(std::function<Return(Args...)> _task) : task(std::move(_task)) {}

		public:
			Promise(const Promise&) = delete;
			Promise(Promise&& other) noexcept
				: task(std::move(other.task)),
				callback(std::move(other.callback)),
				error_callback(std::move(other.error_callback)),
				exception(std::move(other.exception)),
				thread(std::move(other.thread)) {}

			~Promise() {
				if (thread.joinable()) {
					thread.join();
				}
			}

			static Promise& create(std::function<Return(Args...)> _task) {
				return *new Promise(std::move(_task));
			}

			Promise& operator()(Args... args) {
				thread = std::thread(&Promise::resolve, this, std::forward<Args>(args)...);
				return *this;
			}

			Promise& then(std::function<void(Return)> _callback) {
				std::lock_guard<std::mutex> lock(mutex);
				callback = std::move(_callback);
				return *this;
			}

			Promise& then(Promise<void(Return)> _promise) {
				std::lock_guard<std::mutex> lock(mutex);
				callback = [_promise = std::move(_promise)](Return result) mutable {
					_promise(result);
					};
				return *this;
			}

			Promise& _catch(std::function<bool(std::exception_ptr)> _error_callback) {
				std::lock_guard<std::mutex> lock(mutex);
				error_callback = std::move(_error_callback);
				return *this;
			}

			void await() {
				auto shared_this = this->shared_from_this();
				if (thread.joinable()) {
					thread.join();
				}
				if (exception) {
					std::rethrow_exception(exception);
				}
			}
		};

	}
}
