#include <stdio.h>
#include "application.h"
#include <GL/glfw.h>
#include <vsx_avector.h>
#include <vsx_string.h>
#include <vsxfst.h>
#include <vsx_version.h>
#include <stdlib.h>

bool app_ctrl = false;
bool app_alt = false;
bool app_shift = false;
bool dual_monitor = false;
bool disable_randomizer = false;
// implementation of app externals
int app_argc = 0;
char** app_argv;

void set_modifiers()
{
  app_ctrl = (bool)glfwGetKey(GLFW_KEY_LCTRL);
  app_shift = (bool)glfwGetKey(GLFW_KEY_LSHIFT);
  //printf("shift is: %d\n",app_shift);
  app_alt = (bool)glfwGetKey(GLFW_KEY_LALT);
}

long key_pressed = -1;
int key_character = -1;
float key_time;
float key_repeat_time;
float initial_key_delay = 0.04f;

void GLFWCALL key_char_event( int character, int action )
{
#ifdef __APPLE__
  return;
#endif
  if (action == GLFW_PRESS)
  {
    //printf("got key: %d\n",character);
    app_char(character);
    key_character = character;

  }
}


void GLFWCALL key_event(int key, int action)
{
  set_modifiers();
  if (action == GLFW_PRESS)
  {
    //printf("got keydown: %d\n",key);
    app_key_down((long)key);
    key_character = -1;
    key_time = 0.0f;
    key_repeat_time = 0.0f;
    key_pressed = key;
    initial_key_delay = 0.04f;
  }
  if (action == GLFW_RELEASE)
  {
    //printf("got key up: %d\n",key);
    app_key_up((long)key);
    key_pressed = -1;
  }
}

int last_x = 0, last_y = 0;
int mouse_state = 0;

void GLFWCALL mouse_button_event(int button, int action)
{
  glfwGetMousePos(&last_x, &last_y);
  set_modifiers();
  unsigned long i_but = 0;
  switch (button)
  {
    case GLFW_MOUSE_BUTTON_MIDDLE: i_but = 1; break;
    case GLFW_MOUSE_BUTTON_RIGHT: i_but = 2;
  }
  if (action == GLFW_PRESS)
  {
    mouse_state = 1; // pressed
    app_mouse_down(i_but, last_x, last_y);
  }
  else
  {
    mouse_state = 0; // depression
    app_mouse_up(i_but, last_x, last_y);
  }
}

int mouse_pos_type = 0;

void GLFWCALL mouse_pos_event(int x, int y)
{
  set_modifiers();
  //printf("mouse_pos_event %d, %d\n",x,y);
  glfwGetMousePos(&last_x, &last_y);
  //last_x = x;
  //last_y = y;
  if (mouse_state) mouse_pos_type = 1;
  else mouse_pos_type = 2;
}

int mousewheel_prev_pos = 0;

void GLFWCALL mouse_wheel(int pos)
{
  app_mousewheel((float)(pos-mousewheel_prev_pos),last_x,last_y);
  mousewheel_prev_pos = pos;
}
//========================================================================
// main()
//========================================================================

int main(int argc, char* argv[])
{
  app_argc = argc;
  app_argv = argv;
  int     width, height, running, frames, x, y;
  double  t, t0, t1;
  char    titlestr[ 200 ];

  // Initialise GLFW
  glfwInit();

  bool start_fullscreen = false;
  int x_res = 1280;
  int y_res = 720;
  bool manual_resolution_set = false;
  for (int i = 1; i < argc; i++) {
    vsx_string arg1 = argv[i];
    if (arg1 == "--help")
    {
      printf(
             "Usage:\n"
          "  vsxu_player [path_to_vsx_file]\n"
          "\n"
          "Flags: \n"
          "  -dr   Disable randomizer     \n\n"
          "  -p [x,y] window position x,y \n\n"
          "  -s [x,y] window size x,y \n\n"
            );
      exit(0);
    } else
    if (arg1 == "-f") {
      start_fullscreen = true;
    } else
    if (arg1 == "-dr") {
      disable_randomizer = true;
    } else
    if (arg1 == "-s") {
      if (i+1 < argc)
      {
        i++;
        vsx_string arg2 = argv[i];
        vsx_avector<vsx_string> parts;
        vsx_string deli = ",";
        explode(arg2, deli, parts);
        x_res = s2i(parts[0]);
        y_res = s2i(parts[1]);
        manual_resolution_set = true;
      }
    }
  }
  if (start_fullscreen && !manual_resolution_set)
  {
    // try to get the resolution from the desktop for fullscreen
    GLFWvidmode video_mode;
    glfwGetDesktopMode(&video_mode);
    x_res = video_mode.Height;
    y_res = video_mode.Width;
  }
  
    // Open OpenGL window
  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  if( !glfwOpenWindow( x_res, y_res, 0,0,0,0,16,0, start_fullscreen?GLFW_FULLSCREEN:GLFW_WINDOW ) ) // GLFW_FULLSCREEN
  {
    glfwTerminate();
    return 0;
  }
  if (start_fullscreen) glfwEnable( GLFW_MOUSE_CURSOR );
  app_init(0);

  glfwEnable(GLFW_AUTO_POLL_EVENTS);

  for (int i = 1; i < argc; i++) {
    vsx_string arg1 = argv[i];
    if (arg1 == "-p") {
      if (i+1 < argc)
      {
        i++;
        vsx_string arg2 = argv[i];
        vsx_avector<vsx_string> parts;
        vsx_string deli = ",";
        explode(arg2, deli, parts);
        glfwSetWindowPos(s2i(parts[0]), s2i(parts[1]));
      }
    }
  }

    //glfwSetWindowPos(1380,200);

  glfwSetKeyCallback(&key_event);
  glfwSetMouseButtonCallback(&mouse_button_event);
  glfwSetMousePosCallback(&mouse_pos_event);
  glfwSetCharCallback(&key_char_event);
  glfwSetMouseWheelCallback(&mouse_wheel);
    // Enable sticky keys
  glfwEnable( GLFW_STICKY_KEYS );
  glfwSwapInterval(1);
    // Main loop
  running = GL_TRUE;
  frames = 0;
  t0 = glfwGetTime();
    //typedef BOOL (APIENTRY * wglSwapIntervalEXT_Func)(int);
    //wglSwapIntervalEXT_Func wglSwapIntervalEXT = wglSwapIntervalEXT_Func(wglGetProcAddress("wglSwapIntervalEXT"));
    //if (wglSwapIntervalEXT) wglSwapIntervalEXT(1);

  sprintf( titlestr, "Vovoid VSX Ultra %s [GNU/Linux] [Visit us at http://vsxu.com]", vsxu_ver);
  glfwSetWindowTitle( titlestr );


  while( running )
  {
    if (mouse_pos_type)
    {
      if (mouse_pos_type == 1) app_mouse_move(last_x,last_y);
      else app_mouse_move_passive(last_x,last_y);
      mouse_pos_type = 0;
    }

    app_pre_draw();

        // Get time and mouse position
    t = glfwGetTime();
    glfwGetMousePos( &x, &y );
    float delta = t-t1;
    t1 = t;
    if (key_pressed != -1)
    {
          //printf("%f\n", delta);
      key_time += delta;
      if (key_time > 0.3f)
      {
        key_repeat_time += delta;
        if (key_repeat_time > initial_key_delay)
        {
          key_repeat_time = 0.0f;
          if (key_character != -1)
            app_char(key_character);
          app_key_down((long)key_pressed);
          initial_key_delay *= 0.99f;
              //printf("repeating key: %d\n", key_character);
        }
      }
    }
        // Calculate and display FPS (frames per second)
        /*if( (t-t0) > 1.0 || frames == 0 )
    {
    fps = (double)frames / (t-t0);
    sprintf( titlestr, "Vovoid VSX Ultra Artiste :: GNU/Linux version (%.1f FPS)", fps );
    glfwSetWindowTitle( titlestr );
    t0 = t;
    frames = 0;
  }*/
    frames ++;

        // Get window size (may be different than the requested size)
    glfwGetWindowSize( &width, &height );
    height = height > 0 ? height : 1;

        // Set viewport
    glViewport( 0, 0, width, height );

        // Clear color buffer
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                     // Reset The Modelview Matrix

    app_draw(0);

        // Select and setup the projection matrix
        /*glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f,
    100.0f );

        // Select and setup the modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0.0f, 1.0f, 0.0f,    // Eye-position
    0.0f, 20.0f, 0.0f,   // View-point
    0.0f, 0.0f, 1.0f );  // Up-vector

        // Draw a rotating colorful triangle
    glTranslatef( 0.0f, 14.0f, 0.0f );
    glRotatef( 0.3*(GLfloat)x + (GLfloat)t*100.0f, 0.0f, 0.0f, 1.0f );
    glBegin( GL_TRIANGLES );
    glColor3f( 1.0f, 0.0f, 0.0f );
    glVertex3f( -5.0f, 0.0f, -4.0f );
    glColor3f( 0.0f, 1.0f, 0.0f );
    glVertex3f( 5.0f, 0.0f, -4.0f );
    glColor3f( 0.0f, 0.0f, 1.0f );
    glVertex3f( 0.0f, 0.0f, 6.0f );
    glEnd();
        */
        // Swap buffers
    glfwSwapBuffers();

        // Check if the ESC key was pressed or the window was closed
    running = /*!glfwGetKey( GLFW_KEY_ESC ) &&*/
        glfwGetWindowParam( GLFW_OPENED );
  }

    // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}
//endif
