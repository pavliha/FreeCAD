// SPDX-License-Identifier: LGPL-2.1-or-later

#include <gtest/gtest.h>

#include <Gui/Inventor/SoMouseWheelEvent.h>

TEST(SoMouseWheelEvent, defaultConstructedIsNotPrecise)
{
    SoMouseWheelEvent event;

    EXPECT_EQ(event.getDelta(), 0);
    EXPECT_EQ(event.getPixelDelta(), SbVec2f(0.0F, 0.0F));
    EXPECT_FALSE(event.isPrecise());
}

TEST(SoMouseWheelEvent, precisionIsIndependentOfThePixelDeltaValue)
{
    SoMouseWheelEvent touchpadMidGesture;
    SoMouseWheelEvent wheelMouse;

    touchpadMidGesture.setPixelDelta(SbVec2f(0.0F, 0.0F), true);
    wheelMouse.setDelta(120);
    wheelMouse.setPixelDelta(SbVec2f(0.0F, 0.0F), false);

    EXPECT_TRUE(touchpadMidGesture.isPrecise());
    EXPECT_FALSE(wheelMouse.isPrecise());
}

TEST(SoMouseWheelEvent, setPixelDeltaStoresTheDeltaUnchanged)
{
    SoMouseWheelEvent event;

    event.setPixelDelta(SbVec2f(6.0F, -20.0F), true);

    EXPECT_EQ(event.getPixelDelta(), SbVec2f(6.0F, -20.0F));
}

TEST(SoMouseWheelEvent, toGlPixelDeltaScalesByDevicePixelRatioAndFlipsY)
{
    EXPECT_EQ(SoMouseWheelEvent::toGlPixelDelta(SbVec2f(3.0F, -10.0F), 2.0F), SbVec2f(6.0F, 20.0F));
}
