// SPDX-License-Identifier: LGPL-2.1-or-later

#include <gtest/gtest.h>

#include <numbers>

#include <Gui/SoTouchEvents.h>

TEST(NativeGesturePinch, zoomWithoutBeginIsRejected)
{
    NativeGesturePinch tracker;

    EXPECT_FALSE(tracker.update(Qt::ZoomNativeGesture, 0.5, SbVec2f(10.0F, 20.0F)));
    EXPECT_FALSE(tracker.isActive());
}

TEST(NativeGesturePinch, endWithoutBeginIsRejected)
{
    NativeGesturePinch tracker;

    EXPECT_FALSE(tracker.update(Qt::EndNativeGesture, 0.0, SbVec2f(10.0F, 20.0F)));
}

TEST(NativeGesturePinch, beginActivatesAndRecordsCenter)
{
    NativeGesturePinch tracker;

    EXPECT_TRUE(tracker.update(Qt::BeginNativeGesture, 0.0, SbVec2f(10.0F, 20.0F)));

    EXPECT_TRUE(tracker.isActive());
    EXPECT_EQ(tracker.event().state, SoGestureEvent::SbGSStart);
    EXPECT_EQ(tracker.event().startCenter, SbVec2f(10.0F, 20.0F));
    EXPECT_EQ(tracker.event().curCenter, SbVec2f(10.0F, 20.0F));
}

TEST(NativeGesturePinch, zoomKeepsTheStartCenterFromBegin)
{
    NativeGesturePinch tracker;
    tracker.update(Qt::BeginNativeGesture, 0.0, SbVec2f(10.0F, 20.0F));

    EXPECT_TRUE(tracker.update(Qt::ZoomNativeGesture, 0.25, SbVec2f(30.0F, 40.0F)));

    EXPECT_EQ(tracker.event().state, SoGestureEvent::SbGSUpdate);
    EXPECT_DOUBLE_EQ(tracker.event().deltaZoom, 1.25);
    EXPECT_DOUBLE_EQ(tracker.event().deltaAngle, 0.0);
    EXPECT_EQ(tracker.event().startCenter, SbVec2f(10.0F, 20.0F));
    EXPECT_EQ(tracker.event().curCenter, SbVec2f(30.0F, 40.0F));
}

TEST(NativeGesturePinch, rotateConvertsDegreesToRadiansAndInvertsDirection)
{
    NativeGesturePinch tracker;
    tracker.update(Qt::BeginNativeGesture, 0.0, SbVec2f(0.0F, 0.0F));

    EXPECT_TRUE(tracker.update(Qt::RotateNativeGesture, 90.0, SbVec2f(0.0F, 0.0F)));

    EXPECT_EQ(tracker.event().state, SoGestureEvent::SbGSUpdate);
    EXPECT_DOUBLE_EQ(tracker.event().deltaAngle, -std::numbers::pi / 2.0);
    EXPECT_DOUBLE_EQ(tracker.event().deltaZoom, 0.0);
}

TEST(NativeGesturePinch, endDeactivatesAndFurtherUpdatesAreRejected)
{
    NativeGesturePinch tracker;
    tracker.update(Qt::BeginNativeGesture, 0.0, SbVec2f(1.0F, 2.0F));

    EXPECT_TRUE(tracker.update(Qt::EndNativeGesture, 0.0, SbVec2f(3.0F, 4.0F)));

    EXPECT_FALSE(tracker.isActive());
    EXPECT_EQ(tracker.event().state, SoGestureEvent::SbGSEnd);
    EXPECT_FALSE(tracker.update(Qt::ZoomNativeGesture, 0.5, SbVec2f(5.0F, 6.0F)));
}

TEST(NativeGesturePinch, unhandledGestureTypeIsRejectedWithoutDisturbingState)
{
    NativeGesturePinch tracker;
    tracker.update(Qt::BeginNativeGesture, 0.0, SbVec2f(1.0F, 2.0F));

    EXPECT_FALSE(tracker.update(Qt::SmartZoomNativeGesture, 0.0, SbVec2f(9.0F, 9.0F)));

    EXPECT_TRUE(tracker.isActive());
    EXPECT_TRUE(tracker.update(Qt::ZoomNativeGesture, 0.5, SbVec2f(7.0F, 8.0F)));
    EXPECT_EQ(tracker.event().startCenter, SbVec2f(1.0F, 2.0F));
}

TEST(NativeGesturePinch, aSecondGestureResetsTheStartCenter)
{
    NativeGesturePinch tracker;
    tracker.update(Qt::BeginNativeGesture, 0.0, SbVec2f(1.0F, 2.0F));
    tracker.update(Qt::EndNativeGesture, 0.0, SbVec2f(1.0F, 2.0F));

    tracker.update(Qt::BeginNativeGesture, 0.0, SbVec2f(50.0F, 60.0F));
    tracker.update(Qt::ZoomNativeGesture, 0.1, SbVec2f(51.0F, 61.0F));

    EXPECT_EQ(tracker.event().startCenter, SbVec2f(50.0F, 60.0F));
}
