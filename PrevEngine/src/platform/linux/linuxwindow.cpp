#include "pch.h"
#include "linuxwindow.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace prev { 
    
    Window * Window::Create(const WindowProps &props) {
		return new linux::LinuxWindow(props);
	}

    namespace linux {

        Display * display = NULL;
        int screen;
        ::Window win;
        XEvent event;

        LinuxWindow::LinuxWindow(const WindowProps &props) {
            PV_CORE_TRACE("Creating Linux window [Title = %s] [Width = %d] [Height = %d]", props.Title.c_str(), props.Width, props.Height);
            m_Data.title = props.Title;
            m_Data.width = props.Width;
            m_Data.height = props.Height;

            display = XOpenDisplay(NULL);

            if (display == NULL) {
                PV_CORE_ERROR("Unable to create X11 display");
                return;
            }

            screen = DefaultScreen(display);

            win = XCreateSimpleWindow(display, RootWindow(display, screen), 200, 200, 1280, 720, 1, BlackPixel(display, screen), WhitePixel(display, screen));

            XSelectInput(display, win, ExposureMask | KeyPressMask);
            XMapWindow(display, win);

        }

        void LinuxWindow::OnUpdate() {
            XNextEvent(display, &event);
        }

        void LinuxWindow::SetVSync(bool enabled) {
            
        }
        
		bool LinuxWindow::IsVSync() const {
            
        }

		void LinuxWindow::CreateOpenGLContext() {
            
        }

		void LinuxWindow::CreateDirectXContext() {
            
        }
        
		bool LinuxWindow::IsKeyDown(int keyCode) {
            return false;
        }

        LinuxWindow::~LinuxWindow() {

        }

    }   
}