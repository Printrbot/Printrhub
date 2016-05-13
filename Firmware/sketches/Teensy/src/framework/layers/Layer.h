//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_LAYER_H
#define TEENSYCMAKE_LAYER_H

#include "../core/UIElement.h"
#include "../core/StackArray.h"

class Layer: public UIElement
{
#pragma mark Constructor
public:
    Layer();
    Layer(Rect frame);
    virtual ~Layer();

#pragma mark Getter/Setter
    void setBackgroundColor(const uint16_t& color);
    uint16_t getBackgroundColor() const;

    void setStrokeColor(const uint16_t& strokeColor);
    uint16_t getStrokeColor() const;

    void setStrokeWidth(const uint8_t strokeWidth);
    uint8_t getStrokeWidth() const;

#pragma mark Layout Management
    void splitWithRect(Rect& rect);
    void splitVertically(int x, Layer** left, Layer** right);
    void splitHorizontally(int y, Layer**top, Layer**bottom);
    void invalidateRect(Rect &invalidationRect);
	uint16_t getOriginX();

#pragma mark Layer Hierarchy
    void addSublayer(Layer* layer);
    StackArray<Layer*>* getSublayers();
    bool isLeaf();
    void removeAllSublayers();
    virtual Layer* subLayerWithRect(Rect frame);

#pragma mark Draw and Display
    virtual void setNeedsDisplay();
    virtual void draw(Rect &invalidationRect);
    void display(Layer* backgroundLayer=NULL);
    Rect getRenderFrame();
    bool isVisible();
	void setVisible(const bool visible);
	virtual Rect prepareRenderFrame(const Rect proposedRenderFrame);

#pragma mark Misc
    void log();

#pragma mark UIElements
    virtual void setFrame(Rect frame) override;

#pragma mark Member Variables
protected:
    uint16_t _backgroundColor;
    uint16_t _strokeColor;
    uint8_t _strokeWidth;
    bool _needsDisplay;
    int uniqueId;
    StackArray<Layer*>* _sublayers;
	bool _visible;
};


#endif //TEENSYCMAKE_LAYER_H
