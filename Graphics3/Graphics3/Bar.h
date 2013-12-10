#pragma once

#include <string>
using std::string;

#include <map>
using std::map;

struct Position{};
struct Dir{};
struct Default{};

class Bar
{
public:
	Bar(const string& name);
	~Bar(void);

	template<typename T>
	T* addVar(const string& name, const T& value);

	// how to automatic set T =/
	template<typename T, typename V>
	T* addVar(const string& name, const T& value);

	// NOT SAFE (!!!)
	template<typename T>
	T* getVar(const string& name);

	void addButton(const string& name, TwButtonCallback cb);

private:
	template<typename T, typename V>
	void registerVar(const string& name, T* var);

	TwBar* bar_;
	map<string, void*> vars_;
};

template<typename T>
T* Bar::addVar(const string& name, const T& value)
{
	return addVar<T, Default>(name, value);
}

template<typename T, typename V>
T* Bar::addVar(const string& name, const T& value)
{
	T* tmp = new T(value);
	vars_[name] = tmp;
	registerVar<T,V>(name, tmp);
	return tmp;
}

template<typename T>
T* Bar::getVar(const string& name)
{
	auto it = vars_.find(name);
	if (it != vars_.end())
		return static_cast<T*>(it->second);
	return 0;
}

template<>
inline void Bar::registerVar<GLfloat, Default>(const string& name, GLfloat* var)
{
	TwAddVarRW(bar_, name.c_str(), TW_TYPE_FLOAT, var, "");
}

template<>
inline void Bar::registerVar<vec3, Position>(const string& name, vec3* var)
{
	TwAddVarRW(bar_, name.c_str(), TW_TYPE_DIR3F, var, "");
}

template<>
inline void Bar::registerVar<vec3, Dir>(const string& name, vec3* var)
{
	TwAddVarRW(bar_, name.c_str(), TW_TYPE_DIR3F, var, "");
}

template<>
inline void Bar::registerVar<GLint, Default>(const string& name, GLint* var)
{
	TwAddVarRW(bar_, name.c_str(), TW_TYPE_INT32, var, "");
}
