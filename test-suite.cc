#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller.hh"
#include "curve.hh"

class StationaryTest: public ::testing::Test {
public:
  StationaryTest(void): m_curve(0, 10) {}
protected:
  Curve m_curve;
};

TEST_F(StationaryTest, StartWithGivenValue) {
  EXPECT_EQ(0, m_curve.pos());
}

TEST_F(StationaryTest, StayConstantOverTime) {
  m_curve.update(5);
  EXPECT_EQ(0, m_curve.pos());
}

class MovingForwardTest: public ::testing::Test {
public:
  MovingForwardTest(void):
    m_curve(100, 10)
  {
    m_curve.retarget(140);
  }
protected:
  Curve m_curve;
};

TEST_F(MovingForwardTest, AdvanceWithTime) {
  m_curve.update(1);
  EXPECT_EQ(105, m_curve.pos());
}

TEST_F(MovingForwardTest, AccelerateOverTime) {
  m_curve.update(1);
  m_curve.update(1);
  EXPECT_EQ(120, m_curve.pos());
}

TEST_F(MovingForwardTest, UseSpecifiedAcceleration) {
  Curve curve(0, 20);
  curve.retarget(200);
  curve.update(1);
  EXPECT_EQ(10, curve.pos());
}

TEST_F(MovingForwardTest, DetermineTimeOfReversal) {
  EXPECT_EQ(2, m_curve.reverseTime());
  m_curve.update(1);
  EXPECT_EQ(1, m_curve.reverseTime());
  m_curve.update(1);
  EXPECT_EQ(0, m_curve.reverseTime());
}

TEST_F(MovingForwardTest, DecelerateBeforeTarget) {
  m_curve.update(3);
  EXPECT_EQ(135, m_curve.pos());
}

TEST_F(MovingForwardTest, StopAtTarget) {
  m_curve.update(4);
  EXPECT_EQ(140, m_curve.pos());
  m_curve.update(1);
  EXPECT_EQ(140, m_curve.pos());
}

TEST_F(MovingForwardTest, AccomodateReducedTarget) {
  m_curve.update(2);
  m_curve.retarget(120);
  m_curve.update(0);
  EXPECT_EQ(120, m_curve.pos());
}

TEST_F(MovingForwardTest, RampDownForReducedTarget) {
  m_curve.update(2);
  m_curve.retarget(120);
  m_curve.update(2);
  EXPECT_EQ(140, m_curve.pos());
}

TEST_F(MovingForwardTest, ContinueIfIdentical) {
  m_curve.update(2);
  m_curve.retarget(140);
  m_curve.update(2);
  EXPECT_EQ(140, m_curve.pos());
}

class MovingBackwardTest: public ::testing::Test {
public:
  MovingBackwardTest(void):
    m_curve(100, 10)
  {
    m_curve.retarget(60);
  }
protected:
  Curve m_curve;
};

TEST_F(MovingBackwardTest, DetermineTimeOfReversal) {
  EXPECT_EQ(2, m_curve.reverseTime());
  m_curve.update(1);
  EXPECT_EQ(1, m_curve.reverseTime());
  m_curve.update(1);
  EXPECT_EQ(0, m_curve.reverseTime());
}

TEST_F(MovingBackwardTest, AdvanceWithTime) {
  m_curve.update(1);
  EXPECT_EQ(95, m_curve.pos());
}

class MockController: public ControllerBase
{
public:
  MOCK_METHOD0(reportTime, void());
  MOCK_METHOD1(reportPosition, void(ControllerBase::Drive));
  MOCK_METHOD2(retargetDrive, void(ControllerBase::Drive,int));
};

class ControllerTest: public ::testing::Test {
public:
  ControllerTest(void) {}
protected:
  MockController m_controller;
};

TEST_F(ControllerTest, CheckTime) {
  EXPECT_CALL(m_controller, reportTime());
  m_controller.parseChar('t');
}

TEST_F(ControllerTest, ReportMiddle) {
  EXPECT_CALL(m_controller, reportPosition(MockController::Middle));
  m_controller.parseChar('m');
}

TEST_F(ControllerTest, RetargetMiddle) {
  EXPECT_CALL(m_controller, retargetDrive(MockController::Middle, 567));
  m_controller.parseChar('5');
  m_controller.parseChar('6');
  m_controller.parseChar('7');
  m_controller.parseChar('m');
}

TEST_F(ControllerTest, AbortRetargetMiddle) {
  EXPECT_CALL(m_controller, reportPosition(MockController::Middle));
  m_controller.parseChar('5');
  m_controller.parseChar('x');
  m_controller.parseChar('m');
}

TEST_F(ControllerTest, RetargetMiddleOnce) {
  EXPECT_CALL(m_controller, retargetDrive(MockController::Middle, 567)).Times(1);
  EXPECT_CALL(m_controller, reportPosition(MockController::Middle));
  m_controller.parseChar('5');
  m_controller.parseChar('6');
  m_controller.parseChar('7');
  m_controller.parseChar('m');
  m_controller.parseChar('m');
}

TEST_F(ControllerTest, ReportLeft) {
  EXPECT_CALL(m_controller, reportPosition(MockController::Left));
  m_controller.parseChar('l');
}

TEST_F(ControllerTest, RetargetLeft) {
  EXPECT_CALL(m_controller, retargetDrive(MockController::Left, 789));
  m_controller.parseChar('7');
  m_controller.parseChar('8');
  m_controller.parseChar('9');
  m_controller.parseChar('l');
}

TEST_F(ControllerTest, ReportRight) {
  EXPECT_CALL(m_controller, reportPosition(MockController::Right));
  m_controller.parseChar('r');
}

TEST_F(ControllerTest, ReportClaw) {
  EXPECT_CALL(m_controller, reportPosition(MockController::Claw));
  m_controller.parseChar('c');
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
