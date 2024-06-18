#include "window.hpp"

#include <GLFW/glfw3.h>

#include <thread>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;

window::window(window_setting set, ps2_controller *ps2Controller, class vga *vga) {
    this->set = set;
    this->ps2Controller = ps2Controller;
    this->vga = vga;

    glfwInit();

    // start a new thread that calls start_ui()
    std::thread th(&window::start_ui, this);
    th.detach();

    while (!ui_alive) { // wait for the initial emulator to start up
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
}


window::~window(void) {
    glfwTerminate();
}

void window::start_ui(void) {

    image_size_x = 320;
    image_size_y = 200;

    image_buff = new uint8_t[image_size_x * image_size_y * 3];

    mouse.x = image_size_x / 2;
    mouse.y = image_size_y / 2;
    mouse.click[0] = mouse.click[1] = false;

    in_focus = false;

    my_window = glfwCreateWindow(image_size_x, image_size_y, "x86emu", set.full ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    glfwSetWindowUserPointer(my_window, this);
    glfwMakeContextCurrent(my_window);

    // handler when keyboard is pressed
    glfwSetKeyCallback(my_window, keyboard_callback);

    // handlers when mouse moves or clicks
    glfwSetMouseButtonCallback(my_window, mouse_callback);
    glfwSetCursorPosCallback(my_window, cursorpos_callback);

    // handler when the emulator size changes
    glfwSetWindowSizeCallback(my_window, window_size_callback);
    //glfwSetCursorEnterCallback(my_window, cursorenter_callback);

    glfwSetWindowAspectRatio(my_window, image_size_x, image_size_y);
    glOrtho(0, image_size_x, 0, image_size_y, -1, 1);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    ui_alive = true;
    ui_loop();

}

void window::stop_ui(void) {
    ui_alive = false;
}

void window::ui_loop(void) {
    GLfloat vtx[] = {0, 0, (float) image_size_x, 0, (float) image_size_x, (float) image_size_y, 0,
                     (float) image_size_y};
    const GLfloat texuv[] = {0, 1, 1, 1, 1, 0, 0, 0};
    while (!glfwWindowShouldClose(my_window) && ui_alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        glfwPollEvents();

        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (vga->need_refresh()) {

            // get size of vga
            uint16_t new_x, new_y;
            vga->get_windowsize(&new_x, &new_y);

            // check if size has changed
            if (new_x && new_y && ((image_size_x != new_x) || (image_size_y != new_y))) {
                image_size_x = vtx[2] = vtx[4] = new_x;
                image_size_y = vtx[5] = vtx[7] = new_y;
                printf("new emulator size by vga: x (%d), y (%d)\n", image_size_x, image_size_y);

                glfwSetWindowSize(my_window, image_size_x, image_size_y);
                glfwSetWindowAspectRatio(my_window, image_size_x, image_size_y);
                glOrtho(0, image_size_x, 0, image_size_y, -1, 1);

                delete[] image_buff;
                image_buff = new uint8_t[image_size_x * image_size_y * 3];
            }

            // get image to display from vga
            vga->display_rgb_image(image_buff, image_size_x * image_size_y);

            // display the image in glfw emulator
            // (size of glfw emulator may be larger than the image provided by vga)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_size_x, image_size_y, 0, GL_RGB, GL_UNSIGNED_BYTE, image_buff);

            glVertexPointer(2, GL_FLOAT, 0, vtx);
            glTexCoordPointer(2, GL_FLOAT, 0, texuv);
        }
        glDrawArrays(GL_QUADS, 0, 4);

        glfwSwapBuffers(my_window);
    }

    ui_close();
}

void window::ui_close(void) {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);

    glfwDestroyWindow(my_window);
    my_window = nullptr;

    if (image_buff != nullptr)
        delete[] image_buff;

    ui_alive = false;
}

void window::keyboard_callback(GLFWwindow *win, int key, int scancode, int action, int mods) {
    window *ui = static_cast<window *>(glfwGetWindowUserPointer(win));
    if (!ui->in_focus)
        return;

    switch (key) {
        case 0x159: // right CTRL
            ui->in_focus = false;
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
    }

//    Keyboard *kb = ui->keyboard;
    ps2_controller *controller = ui->ps2Controller;
    switch (action) {
        case GLFW_RELEASE:
            controller->keyboard.send_scancode(scancode - (ui->set.vm ? 8 : 0) + 0x80);
//            kb->send_scancode(scancode - (ui->set.vm ? 8 : 0) + 0x80);
            break;
        case GLFW_PRESS:
        case GLFW_REPEAT:
            controller->keyboard.send_scancode(scancode - (ui->set.vm ? 8 : 0));
//            kb->send_scancode(scancode - (ui->set.vm ? 8 : 0));
            break;
    }
}

void window::mouse_callback(GLFWwindow *win, int button, int action, int mods) {
    window *ui = static_cast<window *>(glfwGetWindowUserPointer(win));
    if (!ui->in_focus) {
        ui->in_focus = true;
        glfwSetInputMode(win, GLFW_CURSOR, !ui->set.vm ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_HIDDEN);
        puts("to cancle the emulator focus, press the right control key.");
        return;
    }

//    Mouse *mouse = ui->keyboard->get_mouse();
    ps2_controller *controller = ui->ps2Controller;

    ui->mouse.click[button % 2] = action;

    mouse::scancode_packet pckt{};
    memset(&pckt, '\x00', sizeof(pckt));
    pckt.bl = ui->mouse.click[0];
    pckt.br = ui->mouse.click[1];

    controller->mouse.send_packet_scancode(pckt);

}

void window::cursorpos_callback(GLFWwindow *win, double xpos, double ypos) {
    window *ui = static_cast<window *>(glfwGetWindowUserPointer(win));
    if (!ui->in_focus) return;

    static int count = 0;
    if (count++ % 10) // for skiping lags
        return;

    ps2_controller *controller = ui->ps2Controller;

    int32_t x_pos = xpos;
    int32_t y_pos = ypos;

    bool sx = x_pos < ui->mouse.x;
    bool sy = y_pos > ui->mouse.y;

    mouse::scancode_packet pckt{};
    memset(&pckt, '\x00', sizeof(pckt));
    pckt.bl = ui->mouse.click[0];
    pckt.br = ui->mouse.click[1];
    pckt.xs = sx;
    pckt.ys = sy;
    pckt.x_axis_val = (x_pos - ui->mouse.x) / 20;
    pckt.y_axis_val = (ui->mouse.y - y_pos) / 20;

    controller->mouse.send_packet_scancode(pckt);

    ui->mouse.x = x_pos;
    ui->mouse.y = y_pos;
}

void window::window_size_callback(GLFWwindow *win, int width, int height) {
    glViewport(0, 0, width, height);
}

/*
void emulator::cursorenter_callback(GLFWwindow *win, int entered){
}
*/
