#include <stdbool.h>
#include <system.h>
#include <button.h>

void ButtonDebounce(BUTTON* button, bool pressed)
{
    if(button->pressed != pressed )
    {
        if(button->debounce == 0)
        {
            button->debounce = BUTTON_DEBOUNCE_COUNT;
        }
        else
        {
            button->debounce--;

            if(button->debounce == 0)
            {
                //if it was pressed, it is now not pressed.
                //if it was not pressed, it is now pressed.
                button->pressed = !button->pressed;
                button->toggled = true;
            }
        }
    }
    else
    {
        button->debounce = 0;
    }
}