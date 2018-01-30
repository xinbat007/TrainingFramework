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

#define PI 3.14159265
#define RADIUS	2.0f
#define M		0.5f
#define H		0.5f

#define TERRAIN_PEAK_VALUE 20.0f;

#define LINE_LENGTH 256
#define MAX_LENGTH	200
#define READ_LINE()	if (fgets(buff, LINE_LENGTH, pFile) == NULL) return false

#ifndef SAFE_DEL
#define SAFE_DEL(a)					{if(a){delete (a);a=NULL;}}
#endif

#ifndef SAFE_DEL_ARRAY
#define SAFE_DEL_ARRAY(a)			{if(a){delete[] (a);a=NULL;}}
#endif