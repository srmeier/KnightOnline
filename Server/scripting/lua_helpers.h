#pragma once

// Helper macro to stringify an arg (e.g. User -> "User")
#define STR(str) #str
#define STRINGIFY(str) STR(str)

// Define standardised names for exposed Lua class constants.
#define LUA_CLASS_METATABLE __LuaMetatableName
#define LUA_CLASS_METHOD_TABLE __LuaMethodTable

// Provide the in-class declarations for our exposed Lua class constants.
#define DECLARE_LUA_CLASS(Class) \
	static const char * LUA_CLASS_METATABLE; \
	static const struct luaL_Reg LUA_CLASS_METHOD_TABLE[]; \
	typedef Class SelfType;

// Define our Lua class constants (outside of the class)
//#define DEFINE_LUA_CLASS(Class, methods) \
//	const char * Class ## ::LUA_CLASS_METATABLE = "luaL_" STR(Class); \
//	DEFINE_LUA_FUNCTION_TABLE(Class::LUA_CLASS_METHOD_TABLE, methods)
#define CONCAT_(A, B) A ## B
#define CONCAT(A, B) CONCAT_(A, B)

// Define our Lua class constants (outside of the class)
//#define DEFINE_LUA_CLASS(Class, methods) \
//      const char * Class ## ::LUA_CLASS_METATABLE = "luaL_" STR(Class); \
//      DEFINE_LUA_FUNCTION_TABLE(Class::LUA_CLASS_METHOD_TABLE, methods)
#define DEFINE_LUA_CLASS(...) \
	const char * LUA_CLASS::LUA_CLASS_METATABLE = STRINGIFY(CONCAT(luaL_, LUA_CLASS)); \
	DEFINE_LUA_FUNCTION_TABLE(LUA_CLASS::LUA_CLASS_METHOD_TABLE, __VA_ARGS__)

// Helper macro to build a Lua function table.
// Should look something like:
/*
const struct luaL_Reg gFuncs[] =
{
{ "Function", Function },
{ nullptr, nullptr }
};
*/
#define DEFINE_LUA_FUNCTION_TABLE(name, ...) const struct luaL_Reg name[] = \
	 { \
	 __VA_ARGS__ \
		{ nullptr, nullptr } \
	 }

// Helper macros to generate elements in a Lua function table.
#define MAKE_LUA_FUNCTION(name) { #name, Lua_ ## name }, 

// #define MAKE_LUA_METHOD(Class, name) { STR(name), Class::Lua_ ## name }, 
#define MAKE_LUA_METHOD(name) { #name, LUA_CLASS::Lua_ ## name }, 

// Defines the header for a Lua C function (or C++ method prototype, but remember to define as static).
#define LUA_FUNCTION(name) int Lua_ ## name(lua_State *L)
#define DECLARE_LUA_FUNCTION(name) static LUA_FUNCTION(name) 

#define DECLARE_LUA_GETTER(name) DECLARE_LUA_FUNCTION(name) { LUA_RETURN(LUA_GET_INSTANCE()->name()); }

/**
* Start of helper template functions 
**/

// Creates Lua userdata & assigns it to the predefined class's metatable
// so that we can use the object directly within Lua.
template <typename T> INLINE void lua_tpush(lua_State * L, T arg) 
{
	T * udata = (T *)lua_newuserdata(L, sizeof(T *));
	*udata = arg;
	luaL_setmetatable(L, arg->LUA_CLASS_METATABLE);
}

// std::string
template <> INLINE void lua_tpush(lua_State * L, std::string arg) { lua_pushlstring(L, arg.c_str(), arg.length()); }

// char *
template <> INLINE void lua_tpush(lua_State * L, const char * arg) { lua_pushstring(L, arg); }
template <> INLINE void lua_tpush(lua_State * L, char * arg)  { lua_pushstring(L, arg); }

// Retrieves userdata from the start but does NOT enforce type checks. Shouldn't use this if we can help it.
template <typename T> INLINE T lua_to(lua_State * L, int idx) { return *(T *)lua_touserdata(L, idx); }
template <typename T> INLINE T lua_to_default(lua_State * L, int idx, T def) { return *(T *)lua_touserdata(L, idx); } // userdata must NOT be optional.

// Retrieves userdata from the stack and enforces type checks
template <typename T> INLINE T lua_to(lua_State * L, int idx, const char * name) { return *(T *)luaL_checkudata(L, idx, name); }

// 'Numbers' are defined as double by default
template <> INLINE void lua_tpush<double>(lua_State * L, double arg) { lua_pushnumber(L, arg); }
template <> INLINE void lua_tpush<float>(lua_State * L, float arg) { lua_pushnumber(L, arg); }

// Required parameters
template <> INLINE double lua_to<double>(lua_State * L, int idx) { return luaL_checknumber(L, idx); }
template <> INLINE float lua_to<float>(lua_State * L, int idx) { return (float)luaL_checknumber(L, idx); }
template <> INLINE unsigned int lua_to<unsigned int>(lua_State * L, int idx) { return luaL_checkunsigned(L, idx); }
template <> INLINE unsigned short lua_to<unsigned short>(lua_State * L, int idx) { return luaL_checkunsigned(L, idx); }
template <> INLINE unsigned char lua_to<unsigned char>(lua_State * L, int idx) { return luaL_checkunsigned(L, idx); }
template <> INLINE int lua_to<int>(lua_State * L, int idx) { return luaL_checkinteger(L, idx); }
template <> INLINE short lua_to<short>(lua_State * L, int idx) { return luaL_checkinteger(L, idx); }
template <> INLINE char lua_to<char>(lua_State * L, int idx) { return luaL_checkinteger(L, idx); }
template <> INLINE bool lua_to<bool>(lua_State * L, int idx) { return lua_toboolean(L, idx) == 1; }
template <> INLINE const char * lua_to<const char *>(lua_State * L, int idx) { return luaL_checkstring(L, idx); }

// Optional parameters
template <> INLINE double lua_to_default<double>(lua_State * L, int idx, double def) { return luaL_optnumber(L, idx, def); }
template <> INLINE float lua_to_default<float>(lua_State * L, int idx, float def) { return (float)luaL_optnumber(L, idx, def); }
template <> INLINE unsigned int lua_to_default<unsigned int>(lua_State * L, int idx, unsigned int def) { return luaL_optunsigned(L, idx, def); }
template <> INLINE unsigned short lua_to_default<unsigned short>(lua_State * L, int idx, unsigned short def) { return luaL_optunsigned(L, idx, def); }
template <> INLINE unsigned char lua_to_default<unsigned char>(lua_State * L, int idx, unsigned char def) { return luaL_optunsigned(L, idx, def); }
template <> INLINE int lua_to_default<int>(lua_State * L, int idx, int def) { return luaL_optinteger(L, idx, def); }
template <> INLINE short lua_to_default<short>(lua_State * L, int idx, short def) { return luaL_optinteger(L, idx, def); }
template <> INLINE char lua_to_default<char>(lua_State * L, int idx, char def) { return luaL_optinteger(L, idx, def); }

// Handle unsigned types (smaller types will be upcast)
template <> INLINE void lua_tpush<unsigned int>(lua_State * L, unsigned int arg) { lua_pushunsigned(L, arg); }
template <> INLINE void lua_tpush<unsigned short>(lua_State * L, unsigned short arg) { lua_pushunsigned(L, arg); }
template <> INLINE void lua_tpush<unsigned char>(lua_State * L, unsigned char arg) { lua_pushunsigned(L, arg); }

// handle signed types (smaller types will be upcast)
template <> INLINE void lua_tpush<int>(lua_State * L, int arg) { lua_pushinteger(L, arg); }
template <> INLINE void lua_tpush<short>(lua_State * L, short arg) { lua_pushinteger(L, arg); }
template <> INLINE void lua_tpush<char>(lua_State * L, char arg) { lua_pushinteger(L, arg); }
template <> INLINE void lua_tpush<signed char>(lua_State * L, signed char arg) { lua_pushinteger(L, arg); }

// handle booleans
template <> INLINE void lua_tpush<bool>(lua_State * L, bool arg) { lua_pushboolean(L, arg); }

// Shortcut for setting an arbitrary global value.
template <typename T> INLINE void lua_tsetglobal(lua_State * L, const char * szGlobalName, T arg) 
{
	lua_tpush<T>(L, arg);
	lua_setglobal(L, szGlobalName);
}

// Macro to help push a null (nil) arg
//#define LUA_PUSH_nullptr()

// Binds an arbitrary Lua class.
#define lua_bindclass(L, Class) lua_createclass(L, Class::LUA_CLASS_METHOD_TABLE, Class::LUA_CLASS_METATABLE)

// Binds the specified Lua class with the specified methods.
INLINE void lua_createclass(lua_State * L, const luaL_Reg * methods, const char * name)
{
	luaL_newmetatable(L, name);
	luaL_setfuncs(L, methods, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
}

// Macro to help pull args. NOTE: These are checked for type safety.
#define LUA_ARG(type, pos) lua_to<type>(L, pos)
#define LUA_ARG_OPTIONAL(type, def, pos) lua_to_default<type>(L, pos, def)
#define LUA_UARG(type, pos) lua_to<type *>(L, pos, type::LUA_CLASS_METATABLE)

// Shortcut to retrieve the specified class instance (from the first item on the stack, for class bindings)
#define LUA_GET_CLASS_INSTANCE(Class) LUA_UARG(Class, 1)

// Shortcut to retrieve the current class intance (from the first item on the stack, for class bindings)
#define LUA_GET_INSTANCE() LUA_GET_CLASS_INSTANCE(SelfType)

// Shortcut to return the single specified value from a Lua C binding to a Lua function.
#define LUA_RETURN(value) lua_tpush(L, value); \
	return 1 // return 1 result

#define LUA_NO_RESULTS 0

#define LUA_NO_RETURN(value) value; \
	return LUA_NO_RESULTS // return 0 results
