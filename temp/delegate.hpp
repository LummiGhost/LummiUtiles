// #include "nvm/object.h"
 
namespace nvm
{
	template <typename T, typename E> class Delegate_metadata_class;
	template <typename T, typename R, typename... A>
	class Delegate_metadata_class<T, R(A...)>; // sealed
 
	template <typename E> class Delegate_metadata_method;
	template <typename R, typename... A>
	class Delegate_metadata_method<R(A...)>;
 
	class DelegateProxifier;
 
	template <typename T> class Delegate;
	template <typename R, typename... A>
	class Delegate<R(A...)>
	{
	private:
		void* m_target;
		R(*m_callback)(A...);
		const void* m_tag;
		bool m_placement;
 
	public:
		Delegate()
		{
			Delegate<R(A...)>& i_ = *this;
			i_ = *(Delegate<R(A...)>*)NULL;
		}
		Delegate(const Delegate<R(A...)>& d_)
		{
			Delegate<R(A...)>& i_ = *this;
			i_ = d_;
		}
		virtual R Invoke(A... args) { };
		virtual void*& GetMethod() { };
		virtual void*& GetTarget() { };
		virtual void SetTag(const void* value) { };
		virtual const void* GetTag() { };
		virtual bool IsClass() { };
		inline bool IsPlacementNew()
		{
			return this->m_placement;
		};
		template<typename T>
		inline T*& GetTarget()
		{
			void** p_ = &this->GetTarget();
			return *(T**)p_;
		}
		inline bool IsNull()
		{
			return Delegate<R(A...)>::IsNull(*this);
		}
		inline DelegateProxifier* GetProxifier()
		{
			return (DelegateProxifier*)this;
		}
		inline Delegate<R(A...)>* Clone()
		{
			return this;
		}
		inline Delegate<R(A...)>& operator=(const Delegate<R(A...)>& right_)
		{
			if (&right_ == NULL)
			{
				*(void**)this = NULL;
				this->m_callback = NULL;
				this->m_tag = NULL;
				this->m_target = NULL;
				this->m_placement = false;
			}
			else
			{
				*(void**)this = *(void**)&right_;
				this->m_callback = right_.m_callback;
				this->m_tag = right_.m_tag;
				this->m_target = right_.m_target;
				this->m_placement = right_.m_placement;
			}
			return *this;
		}
	public:
		inline static bool IsNull(const Delegate<R(A...)>& d_)
		{
			void* i_ = (void*)&d_;
			if (i_ == NULL)
			{
				return true;
			}
			void* vfbtl = *(void**)i_;
			if (vfbtl == NULL)
			{
				return true;
			}
			return false;
		}
		inline static void* GetMethodAddress(R(*callback_)(A...))
		{
			return (void*)callback_;
		}
		template<typename T>
		inline static void* GetMethodAddress(R(T::* callback_)(A...))
		{
			union
			{
				R(T::* x1_)(A...);
				void* x2_;
			} u_;
			return u_.x2_;
		}
		template<typename T>
		inline static Delegate<R(A...)>* Create(T* target_, R(T::* callback_)(A...), Delegate<R(A...)>* placement_ = NULL)
		{
			if (placement_ == NULL)
			{
				Delegate_metadata_class<T, R(A...)>* delegate_ = new Delegate_metadata_class<T, R(A...)>(target_, callback_);
				return delegate_->GetPointer();
			}
			else
			{
				Delegate_metadata_class<T, R(A...)> delegate_(target_, callback_);
				*placement_ = *(delegate_.GetPointer());
				placement_->m_placement = true;
			}
			return placement_;
		}
		inline static Delegate<R(A...)>* Create(void* target_, R(*callback_)(A...), Delegate<R(A...)>* placement_ = NULL)
		{
			if (placement_ == NULL)
			{
				Delegate_metadata_method<R(A...)>* delegate_ = new Delegate_metadata_method<R(A...)>(target_, callback_);
				return delegate_->GetPointer();
			}
			else
			{
				Delegate_metadata_method<R(A...)> delegate_(target_, callback_);
				*placement_ = *(delegate_.GetPointer());
				placement_->m_placement = true;
			}
			return placement_;
		}
	};
 
	class DelegateProxifier
	{
	private:
		virtual void* Invoke() { };
	public:
		template <typename R, typename... A>
		inline R Invoke(A... args) 
		{ 
			void** vftbl = *(void***)this;
			union
			{
				R(DelegateProxifier::*f)(A...);
				void* p;
			} u_;
			u_.p = vftbl[5];
			DelegateProxifier& i_ = *this;
			return (i_.*(u_.f))(args...);
		};
		virtual void*& GetMethod() { };
		virtual void*& GetTarget() { };
		virtual void SetTag(const void* value) { };
		virtual const void* GetTag() { };
		virtual bool IsClass() { };
		template<typename T>
		inline T*& GetTarget()
		{
			void** p_ = &this->GetTarget();
			return *(T**)p_;
		}
		inline DelegateProxifier* Clone()
		{
			return this;
		}
		template <typename R, typename... A>
		inline Delegate<R(A...)>* GetDelegate()
		{
			return (Delegate<R(A...)>*)this;
		}
	};
 
	template <typename E> class Delegate_metadata_method;
	template <typename R, typename... A>
	class Delegate_metadata_method<R(A...)> : public nvm::Object
	{
	private:
		void* m_target;
		R(*m_callback)(A...);
		const void* m_tag;
 
		friend class Delegate<R(A...)>;
 
		Delegate_metadata_method(void* target_, R(*callback_)(A...))
		{
			this->m_tag = NULL;
			this->m_target = target_;
			this->m_callback = callback_;
		}
	public:
		inline virtual R Invoke(A... args)
		{
			return this->m_callback(args...);
		}
		inline virtual void*& GetMethod()
		{
			return *(void**)&this->m_callback;
		}
		inline virtual void*& GetTarget()
		{
			return this->m_target;
		}
		inline virtual void SetTag(const void* value)
		{
			this->m_tag = value;
		}
		inline virtual const void* GetTag()
		{
			return this->m_tag;
		}
		virtual bool IsClass()
		{
			return false;
		}
		inline Delegate<R(A...)>* GetPointer()
		{
			return (Delegate<R(A...)>*)this;
		}
	};
 
	template <typename T, typename E> class Delegate_metadata_class;
	template <typename T, typename R, typename... A>
	class Delegate_metadata_class<T, R(A...)> : public nvm::Object 
	{
	private:
		T * m_target;
		R(T::*m_callback)(A...);
		const void* m_tag;
 
		friend class Delegate<R(A...)>;
 
		Delegate_metadata_class(T* target_, R(T::*callback_)(A...))
		{
			this->m_tag = NULL;
			this->m_target = target_;
			this->m_callback = callback_;
		}
	public:
		inline virtual R Invoke(A... args)
		{
			T& i_ = *this->m_target;
			return (i_.*this->m_callback)(args...);
		}
		inline virtual void*& GetMethod()
		{
			union
			{
				R(T::**x1_)(A...);
				void** x2_;
			} u;
			u.x1_ = &this->m_callback;
			return *u.x2_;
		}
		inline virtual T*& GetTarget()
		{
			return this->m_target;
		}
		inline virtual void SetTag(const void* value)
		{
			this->m_tag = value;
		}
		inline virtual const void* GetTag()
		{
			return this->m_tag;
		}
		virtual bool IsClass()
		{
			return true;
		}
		inline Delegate<R(A...)>* GetPointer()
		{
			return (Delegate<R(A...)>*)this;
		}
	};
}
