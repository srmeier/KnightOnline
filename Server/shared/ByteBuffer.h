#pragma once

#include <vector>

class ByteBuffer
{
public:
	const static size_t DEFAULT_SIZE = 32;
	bool m_doubleByte;

	ByteBuffer(): _rpos(0), _wpos(0), m_doubleByte(true) { _storage.reserve(DEFAULT_SIZE); }
	ByteBuffer(size_t res): _rpos(0), _wpos(0), m_doubleByte(true) { _storage.reserve(res <= 0 ? DEFAULT_SIZE : res); }
	ByteBuffer(const ByteBuffer &buf): _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }
	virtual ~ByteBuffer() {}

	void clear() 
	{
		_storage.clear();
		_rpos = _wpos = 0;
	}

	template <typename T> void append(T value) { append((uint8 *)&value, sizeof(value)); }
	template <typename T> void put(size_t pos,T value) { put(pos,(uint8 *)&value, sizeof(value)); }

	// stream like operators for storing data
	ByteBuffer &operator<<(bool value) { append<char>((char)value); return *this; }

	// unsigned
	ByteBuffer &operator<<(uint8 value)		{ append<uint8> (value); return *this; }
	ByteBuffer &operator<<(uint16 value)	{ append<uint16>(value); return *this; }
	ByteBuffer &operator<<(uint32 value)	{ append<uint32>(value); return *this; }
	ByteBuffer &operator<<(uint64 value)	{ append<uint64>(value); return *this; }
	// signed as in 2e complement
	ByteBuffer &operator<<(int8 value)		{ append<int8> (value); return *this; }
	ByteBuffer &operator<<(int16 value)		{ append<int16>(value); return *this; }
	ByteBuffer &operator<<(int32 value)		{ append<int32>(value); return *this; }
	ByteBuffer &operator<<(int64 value)		{ append<int64>(value); return *this; }
	ByteBuffer &operator<<(float value)		{ append<float>(value); return *this; }

	ByteBuffer &operator<<(ByteBuffer &value)
	{
		if (value.wpos())
			append(value.contents(), value.wpos());
		return *this;
	}

	// stream like operators for reading data
	ByteBuffer &operator>>(bool &value)		{ value = read<char>() > 0 ? true : false; return *this; }
	// unsigned
	ByteBuffer &operator>>(uint8 &value)	{ value = read<uint8>(); return *this; }
	ByteBuffer &operator>>(uint16 &value)	{ value = read<uint16>(); return *this; }
	ByteBuffer &operator>>(uint32 &value)	{ value = read<uint32>(); return *this; }
	ByteBuffer &operator>>(uint64 &value)	{ value = read<uint64>(); return *this; }
	// signed as in 2e complement
	ByteBuffer &operator>>(int8 &value)		{ value = read<int8>(); return *this; }
	ByteBuffer &operator>>(int16 &value)	{ value = read<int16>(); return *this; }
	ByteBuffer &operator>>(int32 &value)	{ value = read<int32>(); return *this; }
	ByteBuffer &operator>>(int64 &value)	{ value = read<int64>(); return *this; }
	ByteBuffer &operator>>(float &value)	{ value = read<float>(); return *this; }

	// Hacky KO string flag - either it's a single byte length, or a double byte.
	void SByte() { m_doubleByte = false; }
	void DByte() { m_doubleByte = true; }

	ByteBuffer &operator<<(const std::string &value)  { *this << value.c_str(); return *this; }
	ByteBuffer &operator<<(std::string &value) { *this << value.c_str(); return *this; }
	ByteBuffer &operator<<(const char *str) 
	{
		uint16 len = (uint16)strlen(str);
		if (m_doubleByte)
			append((uint8*)&len, 2);
		else
			append((uint8*)&len, 1);
		append((uint8 *)str, len);
		return *this;
	}	
	ByteBuffer &operator<<(char *str)  { *this << (const char*)str; return *this; }

	ByteBuffer &operator>>(std::string& value) 
	{
		uint16 len;
		value.clear();
		if (m_doubleByte)
			len = read<uint16>();
		else
			len = read<uint8>();

		if (_rpos + len <= size())
		{
			for (uint16 i = 0; i < len; i++)
				value.push_back(read<char>());
		}
		return *this;
	}

	uint8 operator[](size_t pos) { return read<uint8>(pos); }

	INLINE size_t rpos() { return _rpos; };
	INLINE size_t rpos(size_t rpos) { return _rpos = rpos; };
	INLINE size_t wpos() { return _wpos; };
	INLINE size_t wpos(size_t wpos) { return _wpos = wpos; };

	template <typename T> T read() 
	{
		T r = read<T>(_rpos);
		_rpos += sizeof(T);
		return r;
	};

	template <typename T> T read(size_t pos) const 
	{
		//ASSERT(pos + sizeof(T) <= size());
		if (pos + sizeof(T) > size())
			return (T)0;
		return *((T*)&_storage[pos]);
	};

	void read(void *dest, size_t len) 
	{
		if (_rpos + len <= size()) 
			memcpy(dest, &_storage[_rpos], len);
		else // throw error();
			memset(dest, 0, len);
		_rpos += len;
	};

	const uint8 *contents() const { return &_storage[0]; };
	INLINE size_t size() const { return _storage.size(); };

	// one should never use resize
	void resize(size_t newsize) 
	{
		_storage.resize(newsize);
		_rpos = 0;
		_wpos = size();
	};

	void reserve(size_t ressize)  { if (ressize > size()) _storage.reserve(ressize); };

	// append to the end of buffer
	void append(const std::string& str) { append((uint8 *)str.c_str(),str.size() + 1); }
	void append(const char *src, size_t cnt) { return append((const uint8 *)src, cnt); }
	void append(const void *src, size_t cnt)
	{
		if (!cnt)
			return;

		// 10MB is far more than you'll ever need.
		ASSERT(size() < 10000000);

		if (_storage.size() < _wpos + cnt)
			_storage.resize(_wpos + cnt);

		memcpy(&_storage[_wpos], src, cnt);
		_wpos += cnt;
	}

	void append(const ByteBuffer& buffer) { if (buffer.size() > 0) append(buffer.contents(), buffer.size()); }
	void append(const ByteBuffer& buffer, size_t len)
	{ 
		ASSERT(buffer._rpos + len <= buffer.size());
		append(buffer.contents() + buffer._rpos, len); 
	}

	void put(size_t pos, const void *src, size_t cnt) 
	{
		ASSERT(pos + cnt <= size());
		memcpy(&_storage[pos], src, cnt);
	}

protected:
	// read and write positions
	size_t _rpos, _wpos;
	std::vector<uint8> _storage;
};
