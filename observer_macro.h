
// place this macro in your class's header file, in your class's definition
#define DECLARE_OBSERVER(callback, param_type) \
protected: \
	typedef callback _callback; \
	typedef const param_type _param_type; \
	typedef struct callback##Info { \
		DECLARE_UNCOPYABLE(callback##Info) \
	public: \
		callback##Info() : _udata(NULL), _on_result(NULL) {} \
		callback##Info(void* udata, _callback on_result) : _udata(udata), _on_result(on_result) {} \
		void* _udata; \
		_callback _on_result; \
	}_callbackInfo; \
	std::list<_callbackInfo *> _observerList; \
	CLock _lock4ObserverList; \
public: \
	void RegisterObserver(void* udata, callback cb); \
	void UnRegisterObserver(void* udata); \
	void NotifyObservers(_param_type param);
	

// place this macro in your class's cpp file
#define IMPLEMENT_OBSERVER(class_name) \
void class_name::RegisterObserver(void* udata, _callback cb) \
{ \
	LOG_FUNCTION_AUTO; \
	_lock4ObserverList.Lock(); \
	_callbackInfo *observer = new _callbackInfo(udata, cb); \
	_observerList.push_back(observer); \
	_lock4ObserverList.UnLock(); \
} \
void class_name::UnRegisterObserver(void* udata) \
{ \
	LOG_FUNCTION_AUTO; \
	_lock4ObserverList.Lock(); \
	std::list<_callbackInfo *>::iterator iter = _observerList.begin(); \
	while (iter != _observerList.end()) { \
		_callbackInfo* observer = *iter; \
		if (observer->_udata == udata) { \
			delete observer; \
			_observerList.erase(iter); \
			break; \
		} \
		iter++; \
	} \
	_lock4ObserverList.UnLock(); \
} \
void class_name::NotifyObservers(_param_type param) \
{ \
	LOG_FUNCTION_AUTO; \
	_lock4ObserverList.Lock(); \
	std::list<_callbackInfo *>::iterator iter = _observerList.begin(); \
	while (iter != _observerList.end()) { \
		_callbackInfo * observer = *iter++; \
		observer->_on_result(observer->_udata, param); \
	} \
	_lock4ObserverList.UnLock(); \
}


// place this macro in your class's destruct function.
#define DESTROY_OBSERVER \
{ \
	std::list<_callbackInfo *>::iterator iter = _observerList.begin(); \
	while (iter != _observerList.end()) { \
		_callbackInfo * observer = *iter++; \
		delete observer; \
	} \
	_observerList.clear(); \
}



	
	