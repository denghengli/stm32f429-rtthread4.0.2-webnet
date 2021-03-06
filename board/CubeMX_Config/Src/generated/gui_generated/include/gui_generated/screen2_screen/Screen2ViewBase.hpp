/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN2VIEWBASE_HPP
#define SCREEN2VIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class Screen2ViewBase : public touchgfx::View<Screen2Presenter>
{
public:
    Screen2ViewBase();
    virtual ~Screen2ViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box box1;
    touchgfx::Button button1;
    touchgfx::Line line1;
    touchgfx::PainterRGB565 line1Painter;
    touchgfx::Line line1_1;
    touchgfx::PainterRGB565 line1_1Painter;
    touchgfx::Line line1_2;
    touchgfx::PainterRGB565 line1_2Painter;
    touchgfx::Line line1_3;
    touchgfx::PainterRGB565 line1_3Painter;
    touchgfx::Line line1_4;
    touchgfx::PainterRGB565 line1_4Painter;
    touchgfx::Line line1_1_1;
    touchgfx::PainterRGB565 line1_1_1Painter;
    touchgfx::Line line1_2_1;
    touchgfx::PainterRGB565 line1_2_1Painter;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Screen2ViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 12000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // SCREEN2VIEWBASE_HPP
