#pragma once

class ReferenceObject
{
public:
	ReferenceObject();

	// Increment the reference count
	void IncRef();

	// Decrease the reference count and delete the object if it hits 0 (i.e. no more references).
	void DecRef();

	virtual ~ReferenceObject() {}

private:
	Atomic<uint32> m_refCount;
};