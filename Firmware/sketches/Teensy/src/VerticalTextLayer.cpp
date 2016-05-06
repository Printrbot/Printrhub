//
// Created by Phillip Schuster on 04.05.16.
//

#include "VerticalTextLayer.h"
#include "Application.h"

void VerticalTextLayer::draw(Rect &dirtyRect, Rect &invalidationRect)
{
	Rect renderFrame = Rect::Intersect(_frame,invalidationRect);

//    Display.fillRect(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,ILI9341_BLUE);
//    return;

/*    int xs = renderFrame.left() - _frame.left();
    int ys = renderFrame.top() - _frame.top();
    int width = renderFrame.width;
    int height = renderFrame.height;

    //Transform to screen space
    renderFrame.x = renderFrame.x % 320;

    if (height > 0 && width > 0)
    {

    }*/

	//Align the text layer
	Rect frame = _frame;
	if (getVerticalTextAlign() == TEXTALIGN_TOP)
	{
		frame.x += 1;
	}
	else if (getVerticalTextAlign() == TEXTALIGN_BOTTOM)
	{
		frame.x = frame.right()-getFont()->cap_height - 1;
	}
	else if (getVerticalTextAlign() == TEXTALIGN_CENTERED)
	{
		frame.x += (frame.width - getFont()->cap_height) / 2;
	}
	frame.width = getFont()->cap_height;

	//TODO: Add code for horizontal alignment

	//TODO: Fix this bug. If the inner layers left and right ly on the outer layers boundaries
	//there is a strange render bug. Inseting by one pixels helps though and doesn't matter here
	frame.y += 1;
	frame.height-= 2;

	//Assign padding
	frame.y += getPadding();
	frame.height -= getPadding()*2;

	if (getTextAlign() == TEXTALIGN_CENTERED)
	{
		uint32_t width = Display.textWidth(getFont(), getText());
		frame.y += (frame.height - width)/2;
		frame.height = width;
	}
	else if (getTextAlign() == TEXTALIGN_LEFT)
	{
		uint32_t width = Display.textWidth(getFont(), getText());
		frame.y += (frame.height - width);
		frame.height= width;
	}
	else if (getTextAlign() == TEXTALIGN_RIGHT)
	{
		uint32_t width = Display.textWidth(getFont(), getText());
		frame.height = width;
	}

	frame.x = frame.x % Display.getLayoutWidth();
	frame.x += Display.getLayoutStart();
	renderFrame.x = renderFrame.x % Display.getLayoutWidth();
	renderFrame.x += Display.getLayoutStart();

	//Fill left
	if (renderFrame.left() < frame.left())
	{
		Display.fillRect(renderFrame.x,renderFrame.y,frame.x - renderFrame.x,renderFrame.height,getBackgroundColor());
	}
	//Fill right
	if (renderFrame.right() > frame.right())
	{
		Display.fillRect(frame.right(),renderFrame.y,renderFrame.right()-frame.right(),renderFrame.height,getBackgroundColor());
	}
	//Fill above
	if (renderFrame.top() < frame.top())
	{
		Display.fillRect(renderFrame.x,renderFrame.y,renderFrame.width,frame.top()-renderFrame.top(),getBackgroundColor());
	}
	//Fill below
	if (renderFrame.bottom() > frame.bottom())
	{
		Display.fillRect(renderFrame.x,frame.bottom(),renderFrame.width,renderFrame.bottom() - frame.bottom(),getBackgroundColor());
	}

	Display.fillRect(frame.left(),frame.top(),frame.width,frame.height,getBackgroundColor());
	//return;

	Display.setClippingRect(&renderFrame);

	Display.setFont(*getFont());
	Display.setTextColor(getForegroundColor(),getBackgroundColor());
	Display.setCursor(frame.x,frame.y + frame.height);
	Display.setTextRotation(270);
	Display.print(getText());

	Display.setTextRotation(0);

	//Fill the rest of the text layer
	//Display.fillRect(Display.getCursorX(),frame.top(),frame.right()-Display.getCursorX(),frame.height,getBackgroundColor());

	Display.resetClippingRect();
}

