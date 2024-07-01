#include <iostream>
#include <mutex>

// the policies
namespace
{
	//creators
	template<class T>
	struct OpNewCreator
	{
		static T* Create()
		{
			return new T;
		}
	};

	template<class T>
	struct MallocCreator
	{
		static T* Create()
		{
			void* buf = std::malloc(sizeof(T));
			if (!buf) return 0;
			return new(buf) T;
		}
	};

	// for smartptr examples 
	template <class T> struct NoChecking
	{
		static void Check(T*) {}
	};

	template <class T> struct EnforceNotNull
	{
		class NullPointerException : public std::exception {};
		static void Check(T* ptr)
		{
			if (!ptr) throw NullPointerException();
		}
	};

	template <class T> struct EnsureNotNull
	{
		static void Check(T*& ptr)
		{
			if (!ptr) ptr = nullptr; // TODO: GetDefaultValue();
		}
	};

	template <class T> struct MultiThread
	{
		class Lock
		{
			std::mutex m_mutex;
		public:
			Lock(T& ptr)
			{
				m_mutex.lock();
				std::cout << "derefence a pointer inside a lock" << std::endl;
				/*.do work..*/
			}

			~Lock()
			{
				m_mutex.unlock();
			}
		};
	};
}

// host clases
namespace
{
	class Widget
	{
	public:
		void description()
		{
			std::cout << "Widget example" << std::endl;
		}
	};

	template <class CreationPolicy>
	class WidgetManagerSimple : public CreationPolicy
	{
	};

	//template template parameters
	template <template <class /*T formal argument for CreationPolicy no for WidgetManager*/> class CreationPolicy = OpNewCreator>
	class WidgetManager : public CreationPolicy<Widget>
	{
	};

}

//host clases combining policies
namespace
{
	template
	<
		class T,
		template <class> class CheckingPolicy,
		template <class> class ThreadingModel
	>
	class SmartPtr
		: public CheckingPolicy<T>, public ThreadingModel<T>
	{
	public:
		
		SmartPtr()
			: m_pointer(new T)
		{}

		T* operator->()
		{
			typename ThreadingModel<SmartPtr>::Lock guard(*this);
			CheckingPolicy<T>::Check(m_pointer);
			return m_pointer;
		}

	private:

		T* m_pointer;
	};
}

int main()
{
	// the client needs to specify the type widget to the widget manager
	WidgetManagerSimple<OpNewCreator<Widget>> widgetManagerSimple;

	Widget* widget = widgetManagerSimple.Create();
	widget->description();
	delete widget;

	// template template parameters
	WidgetManager<> widgetManager;
	widget = widgetManager.Create();
	widget->description();
	delete widget;

	// smart ptr
	SmartPtr<Widget, NoChecking, MultiThread> smartptr;

	smartptr->description();

	return 0;
}