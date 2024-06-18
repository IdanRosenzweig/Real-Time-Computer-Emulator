#ifndef EMULATOR_WINDOW_H
#define EMULATOR_WINDOW_H

#include <GLFW/glfw3.h>
#include "../common.hpp"
#include "../hardware/vga/vga.hpp"
#include "../hardware/ps2_controller/ps2_controller.h"
#include "../hardware/cpu_i386/ram/ram.hpp"

struct window_setting {
    bool enable;
    bool full;
    bool vm;
};

class window {
private:
    window_setting set;
    class vga *vga;
//    Keyboard *keyboard;
    ps2_controller *ps2Controller;

    struct {
        int32_t x, y;
        bool click[2];
    } mouse;

    GLFWwindow *my_window = nullptr;
    bool ui_alive;

    uint16_t image_size_x, image_size_y;
    uint8_t *image_buff = nullptr;

    bool in_focus;

public:
    window(window_setting set, ps2_controller *ps2Controller, class vga *vga);

    ~window();

    bool is_ui_alive(void) const { return ui_alive; };

    void start_ui(void);

    void stop_ui(void);

private:
    void ui_loop(void);

    void ui_close(void);

    static void keyboard_callback(GLFWwindow *, int key, int scancode, int action, int mods);

    static void mouse_callback(GLFWwindow *, int button, int action, int mods);

    static void cursorpos_callback(GLFWwindow *, double xpos, double ypos);

    static void window_size_callback(GLFWwindow *, int width, int height);
    //static void cursorenter_callback(GLFWwindow *, int entered);
};

#endif
