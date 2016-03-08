#include "stdafx.h"
#include "ReferenceObject.h"

ReferenceObject::ReferenceObject() 
	: m_refCount(0)
{
	IncRef();
}

void ReferenceObject::IncRef() 
{
	++m_refCount;
}

void ReferenceObject::DecRef()
{
	if (m_refCount.decrement() == 0)
		delete this;
}