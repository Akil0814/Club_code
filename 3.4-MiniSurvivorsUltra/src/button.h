#pragma once

#include <string>

#include <raylib.h>

class Button
{
public:
    Button(Rectangle bounds, std::string text, int font_size);
    bool update();
    bool is_hovered() const;
    void draw() const;

private:
    Rectangle _bounds;
    std::string _text;
    int _font_size;
    bool _is_hovered = false;
    bool _is_pressed = false;
};

