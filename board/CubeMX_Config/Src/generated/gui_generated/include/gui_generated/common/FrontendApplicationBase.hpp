/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FRONTENDAPPLICATIONBASE_HPP
#define FRONTENDAPPLICATIONBASE_HPP

#include <mvp/MVPApplication.hpp>
#include <gui/model/Model.hpp>

class FrontendHeap;

class FrontendApplicationBase : public touchgfx::MVPApplication
{
public:
    FrontendApplicationBase(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplicationBase() { }

    // screen1
    void gotoscreen1ScreenNoTransition();

    void gotoscreen1ScreenCoverTransitionEast();

    // Screen2
    void gotoScreen2ScreenSlideTransitionEast();

    // Screen3
    void gotoScreen3ScreenSlideTransitionEast();

protected:
    touchgfx::Callback<FrontendApplicationBase> transitionCallback;
    FrontendHeap& frontendHeap;
    Model& model;

    // screen1
    void gotoscreen1ScreenNoTransitionImpl();

    void gotoscreen1ScreenCoverTransitionEastImpl();

    // Screen2
    void gotoScreen2ScreenSlideTransitionEastImpl();

    // Screen3
    void gotoScreen3ScreenSlideTransitionEastImpl();
};

#endif // FRONTENDAPPLICATIONBASE_HPP