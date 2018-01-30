/*************************************************************************************************
* THIS FILE IS NOT MANDATORY!!!
**************************************************************************************************
**************************************************************************************************
* This file is used to implement some MACROS and functions that can be used in a game.
* Was added to GLLegacy because contains common code used in games.
* To disable this file, set USE_GLLEGACY_COMMONS=0 in GLLegacyConfig\_General\config.bat
**************************************************************************************************/

#ifndef __I_SINGLETON__H__
#define __I_SINGLETON__H__
/*
#define __DeclareSingleton(type)           \
private:                                   \
	type(const type&);                     \
	type& operator=(const type&);          \
public:                                    \
	static void CreateInstance()	       \
	{                                      \
		GetInstance();                     \
	}                                      \
	                                       \
	inline static type * GetInstance()     \
	{                                      \
		if(!Singleton)                     \
			Singleton = new type();        \
		return type::Singleton;            \
	}                                      \
	                                       \
	static void Release()                  \
	{                                      \
		if(Singleton)                      \
			delete Singleton;              \
		Singleton = 0;                     \
	}                                      \
private:                                   \
	static type * Singleton;               

#define __ImplementSingleton(type)         \
	type * type::Singleton = 0;*/

#define __DeclareSingleton__(type) \
public: \
    static type * Singleton; \
    static type * Instance() { return Singleton; }; \
    static bool HasInstance() { return 0 != Singleton; };

#define __ImplementSingleton__(type) \
    type * type::Singleton = 0;

#define __ConstructSingleton__ \
    Singleton = this;

#define __DestructSingleton__ \
    Singleton = 0;

#endif //__I_SINGLETON__H__