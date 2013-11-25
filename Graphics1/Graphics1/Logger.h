#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <cstdarg>
#include <boost\format.hpp>

#define LOG_DEBUG(_str) Logger::instance().writeDebug(boost::str(boost::format("%s:%s:%s:%d") % _str % __FILE__ % __FUNCTION__ % __LINE__))
#define LOG_ERROR(_str) Logger::instance().writeError(boost::str(boost::format("%s:%s:%s:%d") % _str % __FILE__ % __FUNCTION__ % __LINE__))

#define OPENGL_CHECK_FOR_ERRORS() Logger::instance().checkForGLErrors()

class Logger
{
public:
	static Logger& instance(){
		static Logger log;
		return log;
	}

	void writeDebug(const std::string& info);
	void writeError(const std::string& info);
	void checkForGLErrors();
	void setStream(std::ostream& _stream);

private:
	Logger():stream(0){}
	~Logger(){}

	std::ostream* stream;
};

#endif /* LOGGER_H */