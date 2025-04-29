#pragma once
// #include "gui.hpp"

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void framebuffer_resize_callback(GLFWwindow *window, int width, int height);

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
