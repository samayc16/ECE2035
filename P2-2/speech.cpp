#include "speech.h"

#include "globals.h"
#include "hardware.h"
GameInputs inputs;
Timer waitTime;
/**
 * Draw the speech bubble background.
 * Use this file to draw speech bubbles, look at the uLCD libraries for printing
 * text to the uLCD (see graphics.cpp note for link)
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{
    uLCD.rectangle(0, 102, 127, 127, 0xAAAAAA);
    uLCD.filled_rectangle(1, 103 , 126, 126, 0x000000); //White rectangle  
}
void erase_speech_bubble()
{
    uLCD.rectangle(0, 102, 127, 127, TRANSPARENT);
    uLCD.filled_rectangle(1, 103, 126, 126, TRANSPARENT); //White rectangle
    
}

void draw_speech_line(const char* line, int which)
{
    uLCD.textbackground_color(TRANSPARENT);
    uLCD.color(0xFFFFFF);
    int i;
    while (*line)
    {
        uLCD.locate(1, 13);
        i = 0;
        draw_speech_bubble();
        while (i < 17 && *line) 
        {
            inputs = read_inputs();
            uLCD.printf("%c", *line);
            if(inputs.b1) wait_ms(50);
            else wait_ms(25);
            i++;
            line++;
        }
        uLCD.locate(1, 14);
        while (i < 34 && *line)
        {
            inputs = read_inputs();
            uLCD.printf("%c", *line);
            if(inputs.b1) wait_ms(50);
            i++;
            line++;   
        }
        speech_bubble_wait();
    }
}

void speech_bubble_wait()
{
    waitTime.start();
    int time_passed = 0;
    while (inputs.b1 || time_passed < 200) 
    {
        inputs = read_inputs();
        uLCD.filled_circle(120, 123, 2, waitTime.read_ms() * 127 + 0x888888 % 0x1000000);
        time_passed = waitTime.read_ms();
        inputs = read_inputs();
    }
}

void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    erase_speech_bubble();
}

void long_speech(const char* lines[], int n)
{
    int count = 0;
    while (count < n)
    {
        if (count == n - 1 && n % 2 == 1)   draw_speech_line(lines[count], TOP);
        else                                speech(lines[count], lines[count + 1]);
        count += 2;
    }
    erase_speech_bubble();   
}
