#pragma once

#ifdef WIN32
	#define LOGI(...) printf(__VA_ARGS__);printf("\n")
	#define LOGE(...) printf(__VA_ARGS__);printf("\n")
#elif OS_ANDROID
	#include "android/log.h"
	#include "..\GameSpecific\jni\config_Android.h"
	#ifdef LOGI
	#undef LOGI
	#endif
	#ifdef LOGE
	#undef LOGE
	#endif
	#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
	#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#endif

char * LoadTGA(const char * szFileName, int * width, int * height, int * bpp );