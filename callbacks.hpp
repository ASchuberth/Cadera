#pragma once

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);