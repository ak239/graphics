#pragma once

#include "GLContext.h"

#define GET_CONTEXT() GLContextGetter get(getContext())

class Object
{
public:
	Object() : m_isUsed(false){}
	Object(GLContext _context): m_isUsed(false), m_context(_context){}

	virtual ~Object(void){}

	void setContext(GLContext context);

protected:
	const GLContext& getContext() const;

private:
	mutable bool m_isUsed;
	GLContext    m_context;
};

