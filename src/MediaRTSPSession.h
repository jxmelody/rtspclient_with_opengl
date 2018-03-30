#ifndef _MEDIA_RTSP_SESSION_H_
#define _MEDIA_RTSP_SESSION_H_

#include <string>
#include <pthread.h>
#include <vector>		// for std::vector

#if 0
#include "FFMpegDecoder.h"
#else
#include "H264Decoder.h"
#endif

// reference
// https://en.wikipedia.org/wiki/Graphics_display_resolution
typedef enum screen_ratio {
	nHD,
	qHD,
	HD,
	HD_PLUS,
	FHD,
	WQHD,
	QHD_PLUS,
	UHD_4K,
	UHD_5K_PLUS,
	UHD_8K
} SCREEN_RATIO;

//#define WINDOWS_WIDTH	320
//#define WINDOWS_HEIGHT	160

#if defined(USE_GLFW_LIB)
// reference
// https://medium.com/@Plimsky/how-to-install-a-opengl-environment-on-ubuntu-e3918cf5ab6c
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#elif defined(USE_SDL2_LIB)
#include <SDL2/SDL.h>
#endif

class TaskScheduler;
class UsageEnvironment;
class RTSPClient;
#if 0
class MediaRTSPSession : public CDecodeCB
#else
class MediaRTSPSession : public DecodeListener
#endif
{
public:
	MediaRTSPSession();
	virtual ~MediaRTSPSession();
	int startRTSPClient(const char* progName, const char* rtspURL, 
		const char* username = NULL, const char* password = NULL, 
		bool bInterleaved = false, bool bTransportStream = false);
	int stopRTSPClient();
	int openURL(UsageEnvironment& env);
	bool isTransportStream() { return bTransportStream; }
	void setTransportStream(bool flag) { bTransportStream = flag; }
	bool isInterleaved() { return bInterleaved; }
	void setInterleaved(bool flag) { bInterleaved = flag; }
	void setUsername(const char* username) { m_username = username; }
	void setPassword(const char* password) { m_password = password; }
	void setPort(const int port) { m_port = port; }
	void setDebugLevel(int level) { m_debugLevel = level; }

private:
	RTSPClient* m_rtspClient;
	TaskScheduler* taskScheduler;
	UsageEnvironment* usageEnvironment;

	char eventLoopWatchVariable;
	bool bTransportStream;
	bool bInterleaved;

	pthread_t tid;
	bool m_running;
	std::string m_rtspUrl;
	std::string m_progName;
	std::string m_username;
	std::string m_password;
	int m_debugLevel;
	int m_port;

	SCREEN_RATIO screen_radio;
	
	typedef struct buffer {
		int width;
		int height;
		int length;
		uint8_t*	data;
		int pitch;
	} rgb_buffer;

	std::vector<rgb_buffer> myvector;

#if defined(USE_GLFW_LIB)
	GLuint camera_texture;
	// variable declarations for glfw
	GLFWwindow* window;

#elif defined(USE_SDL2_LIB)
	// variable declarations for sdl2
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
#endif

	static void *rtsp_thread_fun(void *param);
	void rtsp_fun();
#if defined(USE_GLFW_LIB)
	static void *glfw3_thread_fun(void *param);
	void glfw3_fun();
#elif defined(USE_SDL2_LIB)
	static void *sdl2_thread_fun(void *param);
	void sdl2_fun();
#endif
protected:
#if 0
	virtual void videoCB(int width, int height, uint8_t* buff, int len, int pitch, RTSPClient* client);
#else
	virtual void onDecoded(void* frame);
#endif
};

#endif // _MEDIA_RTSP_SESSION_H_