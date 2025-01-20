#include <windows.h>
static bool running = true;

struct Render_State {
	void* memory;
	int height = 0;
	int width = 0;
	float render_scale = 0.01f;
	BITMAPINFO bit_map_info;
};

// global for now buffer stuff
static Render_State render_state;
// declared after so we can use render state
#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;
		
		// window size changed
		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int bufferSize = render_state.width * render_state.height * sizeof(u32);
			// instead of malloc we can use windows specific pointers
			// check if buffer already has allocated memory if yes, free it
			if (render_state.memory) { VirtualFree(render_state.memory, 0, MEM_RELEASE); }
			render_state.memory = VirtualAlloc(0,bufferSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
			
			// set the fields of bufferbitmap
			render_state.bit_map_info.bmiHeader.biSize = sizeof(render_state.bit_map_info.bmiHeader);
			render_state.bit_map_info.bmiHeader.biWidth = render_state.width;
			render_state.bit_map_info.bmiHeader.biHeight = render_state.height;
			render_state.bit_map_info.bmiHeader.biPlanes = 1;
			render_state.bit_map_info.bmiHeader.biBitCount = 32;//32 bit u32eger because RBGA is 32
			render_state.bit_map_info.bmiHeader.biCompression = BI_RGB;

		}break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window Class";
	window_class.lpfnWndProc = window_callback;

	// Register the Class
	RegisterClass(&window_class);

	// Call Create Window
	HWND window = CreateWindow(window_class.lpszClassName, "Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	// device context
	HDC hdc = GetDC(window);

	//input struct
	Input input = {};
	
	// delta for frame stuff
	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER performance;
		QueryPerformanceFrequency(&performance);
		performance_frequency = (float)performance.QuadPart;
	}

	// need loop
	// exit loop when window is closed (WINDOW CALLBACK)
	while (running) {
		// Input
		MSG message;

		// reset input changed per frame
		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}


		// PMREMOVE so we only read once
		while (PeekMessage(&message,window,0,0,PM_REMOVE)) {
			switch (message.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)message.wParam;
					bool is_down = ((message.lParam & (1<<31))==0);

// MACRO FOR INPUT KEYS
#define process_button(b,vk)\
case vk:{\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
}break;
					switch (vk_code) {
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
					}


				} break;
				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
					
			}
			
		}

		// Simulate
		simulate_game(&input, delta_time);
		
		// Render
		StretchDIBits(hdc,0,0,render_state.width, render_state.height,0,0,render_state.width,render_state.height, render_state.memory,&render_state.bit_map_info,DIB_RGB_COLORS,SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart)/performance_frequency;// in clock cycles, need seconds so divide by perf freq
		frame_begin_time = frame_end_time;
	}

}