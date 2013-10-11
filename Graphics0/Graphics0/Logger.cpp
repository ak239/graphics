#include "stdafx.h"
#include "Logger.h"

void Logger::writeDebug(const std::string& info)
{
	if (stream)
		*stream << "[DEBUG]" << info << std::endl;
	else
		std::cerr << "[DEBUG]" << info << std::endl;
}

void Logger::writeError(const std::string& info)
{
	if (stream)
		*stream << "[INFO]" << info << std::endl;
	else
		std::cerr << "[INFO]" << info << std::endl;
}

void Logger::setStream(std::ostream& _stream)
{
	stream = &_stream;
}

void Logger::checkForGLErrors()
{
	GLint error = glGetError();
	if (error != GL_NO_ERROR){
		LOG_ERROR(boost::str(boost::format("OpenGL error 0x%X") % (unsigned)error));
	}
}
