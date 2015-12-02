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
  EXPECT_EQ(0, m_curve.target());
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

TEST_F(MovingForwardTest, RememberTarget) {
  EXPECT_EQ(140, m_curve.target());
}

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

TEST_F(MovingForwardTest, UpdateReturnsPosition) {
  Curve curve(0, 20);
  curve.retarget(200);
  EXPECT_EQ(10, curve.update(1));
  EXPECT_EQ(40, curve.update(1));
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

TEST_F(MovingForwardTest, AbortMotion) {
  m_curve.update(2);
  m_curve.stop();
  m_curve.update(2);
  EXPECT_EQ(120, m_curve.pos());
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
  MOCK_METHOD1(reportPosition, void(int));
  MOCK_METHOD1(reportPWM, void(int));
  MOCK_METHOD2(targetAngle, void(int,float));
  MOCK_METHOD0(stopDrives, void());
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

TEST_F(ControllerTest, ReportBase) {
  EXPECT_CALL(m_controller, reportPosition(BASE));
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, ReportBasePWM) {
  EXPECT_CALL(m_controller, reportPWM(BASE));
  m_controller.parseChar('B');
}

TEST_F(ControllerTest, ReportPWMClearsNumber) {
  EXPECT_CALL(m_controller, reportPosition(BASE));
  EXPECT_CALL(m_controller, reportPWM(BASE));
  m_controller.parseChar('0');
  m_controller.parseChar('B');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, ClipAcceptsValues) {
  EXPECT_EQ(100, m_controller.clip(100, 50, 200));
}

TEST_F(ControllerTest, ClipLowerBound) {
  EXPECT_EQ(50, m_controller.clip(20, 50, 200));
}

TEST_F(ControllerTest, ClipUpperBound) {
  EXPECT_EQ(200, m_controller.clip(500, 50, 200));
}

TEST_F(ControllerTest, ConvertZeroAngleToPWM) {
  EXPECT_EQ(1500, m_controller.angleToPWM(0, 1500, 12, 544, 2400));
}

TEST_F(ControllerTest, ConvertAngleToPWM) {
  EXPECT_EQ(1740, m_controller.angleToPWM(20, 1500, 12, 544, 2400));
}

TEST_F(ControllerTest, CheckPWMLowerBound) {
  EXPECT_EQ(544, m_controller.angleToPWM(-90, 1500, 12, 544, 2400));
}

TEST_F(ControllerTest, CheckPWMUpperBound) {
  EXPECT_EQ(2400, m_controller.angleToPWM(90, 1500, 12, 544, 2400));
}

TEST_F(ControllerTest, ConvertCenterPWMToAngle) {
  EXPECT_EQ(0, m_controller.pwmToAngle(1500, 1500, 12));
}

TEST_F(ControllerTest, ConvertPWMToAngle) {
  EXPECT_EQ(20, m_controller.pwmToAngle(1740, 1500, 12));
}

TEST_F(ControllerTest, RetargetBaseInteger) {
  EXPECT_CALL(m_controller, targetAngle(BASE, 12));
  m_controller.parseChar('1');
  m_controller.parseChar('2');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, RetargetBaseFloat) {
  EXPECT_CALL(m_controller, targetAngle(BASE, 12.5));
  m_controller.parseChar('1');
  m_controller.parseChar('2');
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, RetargetNegativeNumber) {
  EXPECT_CALL(m_controller, targetAngle(BASE, -1.5));
  m_controller.parseChar('-');
  m_controller.parseChar('1');
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, DoubleMinus) {
  EXPECT_CALL(m_controller, targetAngle(BASE, 1));
  m_controller.parseChar('-');
  m_controller.parseChar('-');
  m_controller.parseChar('1');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, RetargetZero) {
  EXPECT_CALL(m_controller, targetAngle(BASE, 0));
  m_controller.parseChar('0');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, TargetTwice) {
  EXPECT_CALL(m_controller, targetAngle(BASE, 3)).Times(2);
  m_controller.parseChar('3');
  m_controller.parseChar('b');
  m_controller.parseChar('3');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, IgnoreInvalidFloat) {
  EXPECT_CALL(m_controller, targetAngle(BASE, 5));
  m_controller.parseChar('3');
  m_controller.parseChar('.');
  m_controller.parseChar('4');
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, AbortRetargetBase) {
  EXPECT_CALL(m_controller, stopDrives());
  EXPECT_CALL(m_controller, reportPosition(BASE));
  m_controller.parseChar('5');
  m_controller.parseChar('x');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, CorrectTarget) {
  EXPECT_CALL(m_controller, stopDrives());
  EXPECT_CALL(m_controller, targetAngle(BASE, 3));
  m_controller.parseChar('5');
  m_controller.parseChar('x');
  m_controller.parseChar('3');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, RetargetBaseOnce) {
  EXPECT_CALL(m_controller, targetAngle(BASE, 30)).Times(1);
  EXPECT_CALL(m_controller, reportPosition(BASE));
  m_controller.parseChar('3');
  m_controller.parseChar('0');
  m_controller.parseChar('b');
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, ReportElbow) {
  EXPECT_CALL(m_controller, reportPosition(ELBOW));
  m_controller.parseChar('e');
}

TEST_F(ControllerTest, ReportElbowPWM) {
  EXPECT_CALL(m_controller, reportPWM(ELBOW));
  m_controller.parseChar('E');
}

TEST_F(ControllerTest, RetargetElbow) {
  EXPECT_CALL(m_controller, targetAngle(ELBOW, 10));
  m_controller.parseChar('1');
  m_controller.parseChar('0');
  m_controller.parseChar('e');
}

TEST_F(ControllerTest, ReportShoulder) {
  EXPECT_CALL(m_controller, reportPosition(SHOULDER));
  m_controller.parseChar('s');
}

TEST_F(ControllerTest, ReportShoulderPWM) {
  EXPECT_CALL(m_controller, reportPWM(SHOULDER));
  m_controller.parseChar('S');
}

TEST_F(ControllerTest, ReportGripper) {
  EXPECT_CALL(m_controller, reportPosition(GRIPPER));
  m_controller.parseChar('g');
}

TEST_F(ControllerTest, ReportGripperPWM) {
  EXPECT_CALL(m_controller, reportPWM(GRIPPER));
  m_controller.parseChar('G');
}

TEST_F(ControllerTest, AbortPathForOtherKey) {
  EXPECT_CALL(m_controller, stopDrives());
  m_controller.parseChar('x');
}

TEST_F(ControllerTest, UseBase) {
  EXPECT_EQ(-90, m_controller.limitArm(BASE, -90, 0, 0));
  EXPECT_EQ( 90, m_controller.limitArm(BASE,  90, 0, 0));
}

TEST_F(ControllerTest, RestrictElbow) {
  EXPECT_EQ( 45, m_controller.limitArm(ELBOW,  70, 0, 0));
  EXPECT_EQ(-45, m_controller.limitArm(ELBOW, -70, 0, 0));
}

TEST_F(ControllerTest, RestrictElbowRelativeToShoulder) {
  EXPECT_EQ( 35, m_controller.limitArm(ELBOW, 70, 10, 0));
  EXPECT_EQ(-55, m_controller.limitArm(ELBOW,-70, 10, 0));
}

TEST_F(ControllerTest, RestrictShoulder) {
  EXPECT_EQ( 45, m_controller.limitArm(SHOULDER, 70, 0, 0));
  EXPECT_EQ(-45, m_controller.limitArm(SHOULDER,-70, 0, 0));
}

TEST_F(ControllerTest, RestrictShoulderRelativeToElbow) {
  EXPECT_EQ( 55, m_controller.limitArm(SHOULDER, 70, 0, -10));
  EXPECT_EQ(-35, m_controller.limitArm(SHOULDER,-70, 0, -10));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
