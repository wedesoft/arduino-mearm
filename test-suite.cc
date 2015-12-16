#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controllerbase.hh"
#include "profile.hh"
#include "path.hh"

TEST(ProfileTest, DefaultProfile)
{
  EXPECT_EQ(0, Profile().value(-1));
  EXPECT_EQ(0, Profile().value( 0));
  EXPECT_EQ(0, Profile().value( 1));
}

TEST(ProfileTest, StartWithZero)
{
  EXPECT_EQ(0, Profile(1, 10).value(0));
}

TEST(ProfileTest, EmptyOrNot)
{
  ASSERT_TRUE(Profile().empty());
  ASSERT_FALSE(Profile(1, 10).empty());
}

TEST(ProfileTest, EndWithDistance)
{
  EXPECT_NEAR(1, Profile(1, 10).value(10), 0.001);
  EXPECT_NEAR(2, Profile(2, 10).value(10), 0.001);
}

TEST(ProfileTest, PassesMiddle)
{
  EXPECT_NEAR(0.5, Profile(1, 10).value(5), 0.001);
}

TEST(ProfileTest, DistanceCubicWithDuration)
{
  EXPECT_NEAR(2 * Profile::timeRequired(1, 0.01), Profile::timeRequired(8, 0.01), 0.001);
}

TEST(ProfileTest, DistanceInverseCubicWithMaxJerk)
{
  EXPECT_NEAR(2 * Profile::timeRequired(1, 0.08), Profile::timeRequired(1, 0.01), 0.001);
}

TEST(ProfileTest, TestTimeRequiredPoint)
{
  EXPECT_NEAR(2, Profile::timeRequired(180, 1350), 0.001);
}

TEST(ProfileTest, Accelerates)
{
  EXPECT_GT(0.2, Profile(1, 4).value(1));
}

TEST(ProfileTest, StartAndEndStationary)
{
  EXPECT_FLOAT_EQ(  0, Profile(123, 10).value(-1));
  EXPECT_FLOAT_EQ(123, Profile(123, 10).value(11));
}

class PathTest: public ::testing::Test {
protected:
  Path m_path;
};

TEST_F(PathTest, StartWithZero)
{
  EXPECT_EQ(0, m_path.pos());
}

TEST_F(PathTest, StayConstantOverTime)
{
  m_path.update(5);
  EXPECT_EQ(0, m_path.pos());
}

TEST_F(PathTest, StopSetsValue)
{
  m_path.stop(100);
  EXPECT_EQ(100, m_path.pos());
}

TEST_F(PathTest, ApproachTarget)
{
  m_path.stop(100);
  m_path.retarget(200, 1);
  m_path.update(0.5);
  EXPECT_NEAR(150, m_path.pos(), 0.001);
  m_path.update(0.5);
  EXPECT_NEAR(200, m_path.pos(), 0.001);
}

TEST_F(PathTest, UseSpecifiedDuration)
{
  m_path.stop(100);
  m_path.retarget(200, 2);
  m_path.update(1);
  EXPECT_NEAR(150, m_path.pos(), 0.001);
  m_path.update(1);
  EXPECT_NEAR(200, m_path.pos(), 0.001);
}

TEST_F(PathTest, UpdateReturnsPosition)
{
  m_path.stop(100);
  m_path.retarget(200, 2);
  EXPECT_NEAR(150, m_path.update(1), 0.001);
  EXPECT_NEAR(200, m_path.update(1), 0.001);
}

TEST_F(PathTest, AbortMotion) {
  m_path.stop(100);
  m_path.retarget(200, 2);
  m_path.update(1);
  m_path.stop(m_path.pos());
  m_path.update(1);
  EXPECT_NEAR(150, m_path.pos(), 0.001);
}

TEST_F(PathTest, ModifyTarget) {
  m_path.stop(100);
  m_path.retarget(200, 2);
  m_path.update(1);
  m_path.retarget(300, 2);
  EXPECT_NEAR(150, m_path.pos(), 0.001);
  m_path.update(1);
  EXPECT_NEAR(250, m_path.pos(), 0.001);
  m_path.update(1);
  EXPECT_NEAR(300, m_path.pos(), 0.001);
}

TEST_F(PathTest, IgnoreThirdTarget) {
  m_path.retarget(1, 1);
  m_path.retarget(2, 1);
  m_path.retarget(4, 1);
  m_path.update(1);
  EXPECT_FLOAT_EQ(2, m_path.pos());
}

TEST_F(PathTest, MakeFirstProfileAvailableWhenFinished) {
  m_path.retarget(1, 1);
  m_path.retarget(2, 2);
  m_path.update(1);
  m_path.retarget(4, 1);
  m_path.update(1);
  EXPECT_FLOAT_EQ(4, m_path.pos());
}

TEST_F(PathTest, MakeSecondProfileAvailableWhenFinished) {
  m_path.retarget(1, 2);
  m_path.retarget(2, 1);
  m_path.update(1);
  m_path.retarget(4, 1);
  m_path.update(1);
  EXPECT_FLOAT_EQ(4, m_path.pos());
}

TEST_F(PathTest, QueryTarget) {
  m_path.stop(100);
  EXPECT_FLOAT_EQ(100, m_path.target());
  m_path.retarget(200, 2);
  m_path.update(1);
  EXPECT_FLOAT_EQ(200, m_path.target());
  m_path.retarget(300, 2);
  m_path.update(1);
  EXPECT_FLOAT_EQ(300, m_path.target());
}

TEST_F(PathTest, QueryTimeRequired) {
  m_path.stop(100);
  EXPECT_FLOAT_EQ(0, m_path.timeRemaining());
  m_path.retarget(200, 3);
  EXPECT_FLOAT_EQ(3, m_path.timeRemaining());
  m_path.update(1);
  EXPECT_FLOAT_EQ(2, m_path.timeRemaining());
  m_path.retarget(300, 3);
  EXPECT_FLOAT_EQ(3, m_path.timeRemaining());
  m_path.update(4);
  EXPECT_FLOAT_EQ(0, m_path.timeRemaining());
}

class MockController: public ControllerBase
{
public:
  int offset(int drive) { return 1500; }
  float resolution(int drive) { return 12.0; }
  int lower(int drive) { return 544; }
  int upper(int drive) { return 2400; }
  MOCK_METHOD0(reportTime, void());
  MOCK_METHOD1(reportAngle, void(float));
  MOCK_METHOD1(reportPWM, void(float));
  MOCK_METHOD2(writePWM, void(int,int));
};

class ControllerTest: public ::testing::Test {
public:
  ControllerTest(void) {
    m_controller.curve(BASE    ).stop(45);
    m_controller.curve(SHOULDER).stop(-10);
    m_controller.curve(ELBOW   ).stop(20);
    m_controller.curve(GRIPPER ).stop(30);
  }
protected:
  MockController m_controller;
};

TEST_F(ControllerTest, CheckTime) {
  EXPECT_CALL(m_controller, reportTime());
  m_controller.parseChar('t');
}

TEST_F(ControllerTest, ReportBase) {
  EXPECT_CALL(m_controller, reportAngle(45));
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, ReportBasePWM) {
  EXPECT_CALL(m_controller, reportPWM(2040.0));
  m_controller.parseChar('B');
}

TEST_F(ControllerTest, ClipAcceptsValues) {
  EXPECT_EQ(1500, m_controller.clip(BASE, 1500));
}

TEST_F(ControllerTest, ClipLowerBound) {
  EXPECT_EQ(544, m_controller.clip(BASE, 500));
}

TEST_F(ControllerTest, ClipUpperBound) {
  EXPECT_EQ(2400, m_controller.clip(BASE, 2500));
}

TEST_F(ControllerTest, ConvertZeroAngleToPWM) {
  EXPECT_EQ(1500, m_controller.angleToPWM(SHOULDER, 0));
}

TEST_F(ControllerTest, ConvertAngleToPWM) {
  EXPECT_EQ(1740, m_controller.angleToPWM(SHOULDER, 20));
}

TEST_F(ControllerTest, ConvertCenterPWMToAngle) {
  EXPECT_EQ(0, m_controller.pwmToAngle(BASE, 1500));
}

TEST_F(ControllerTest, ConvertPWMToAngle) {
  EXPECT_EQ(20, m_controller.pwmToAngle(BASE, 1740));
}

TEST_F(ControllerTest, TargetBaseAngle) {
  m_controller.parseChar('1');
  m_controller.parseChar('2');
  m_controller.parseChar('b');
  EXPECT_EQ(12, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, TargetBasePWM) {
  m_controller.parseChar('1');
  m_controller.parseChar('3');
  m_controller.parseChar('2');
  m_controller.parseChar('0');
  m_controller.parseChar('B');
  EXPECT_EQ(-15.0, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, RetargetBaseFloat) {
  m_controller.parseChar('1');
  m_controller.parseChar('2');
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('b');
  EXPECT_EQ(12.5, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, TargetBasePWMFloat) {
  m_controller.parseChar('1');
  m_controller.parseChar('5');
  m_controller.parseChar('0');
  m_controller.parseChar('1');
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('B');
  EXPECT_EQ(0.125, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, TargetBasePWMLowerLimit) {
  m_controller.parseChar('0');
  m_controller.parseChar('B');
  EXPECT_LT(-80, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, TargetShoulderPWMLimitAgainstElbow) {
  m_controller.parseChar('2');
  m_controller.parseChar('4');
  m_controller.parseChar('0');
  m_controller.parseChar('0');
  m_controller.parseChar('S');
  EXPECT_GE(35, m_controller.curve(SHOULDER).target());
}

TEST_F(ControllerTest, RetargetNegativeNumber) {
  m_controller.parseChar('-');
  m_controller.parseChar('1');
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('b');
  EXPECT_EQ(-1.5, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, DoubleMinus) {
  m_controller.parseChar('-');
  m_controller.parseChar('-');
  m_controller.parseChar('1');
  m_controller.parseChar('b');
  EXPECT_EQ(1, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, MinusClearsNumber) {
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('-');
  m_controller.parseChar('3');
  m_controller.parseChar('b');
  EXPECT_EQ(-3, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, RetargetZero) {
  m_controller.parseChar('0');
  m_controller.parseChar('b');
  EXPECT_EQ(0, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, TargetTwice) {
  m_controller.parseChar('3');
  m_controller.parseChar('b');
  m_controller.parseChar('3');
  m_controller.parseChar('b');
  EXPECT_EQ(3, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, IgnoreInvalidFloat) {
  m_controller.parseChar('3');
  m_controller.parseChar('.');
  m_controller.parseChar('4');
  m_controller.parseChar('.');
  m_controller.parseChar('5');
  m_controller.parseChar('b');
  EXPECT_EQ(5, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, AbortRetargetBase) {
  EXPECT_CALL(m_controller, reportAngle(45));
  m_controller.parseChar('5');
  m_controller.parseChar('x');
  m_controller.parseChar('b');
  EXPECT_EQ(45, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, CorrectTarget) {
  m_controller.parseChar('5');
  m_controller.parseChar('x');
  m_controller.parseChar('3');
  m_controller.parseChar('b');
  EXPECT_EQ(3, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, RetargetBaseOnce) {
  EXPECT_CALL(m_controller, reportAngle(45));
  m_controller.parseChar('3');
  m_controller.parseChar('0');
  m_controller.parseChar('b');
  m_controller.parseChar('b');
  EXPECT_EQ(30, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, ReportElbow) {
  EXPECT_CALL(m_controller, reportAngle(20));
  m_controller.parseChar('e');
}

TEST_F(ControllerTest, ReportElbowPWM) {
  EXPECT_CALL(m_controller, reportPWM(1740.0));
  m_controller.parseChar('E');
}

TEST_F(ControllerTest, RetargetElbow) {
  m_controller.parseChar('1');
  m_controller.parseChar('0');
  m_controller.parseChar('e');
  EXPECT_EQ(10, m_controller.curve(ELBOW).target());
}

TEST_F(ControllerTest, ReportShoulder) {
  EXPECT_CALL(m_controller, reportAngle(-10));
  m_controller.parseChar('s');
}

TEST_F(ControllerTest, ReportShoulderPWM) {
  EXPECT_CALL(m_controller, reportPWM(1380.0));
  m_controller.parseChar('S');
}

TEST_F(ControllerTest, ReportGripper) {
  EXPECT_CALL(m_controller, reportAngle(30));
  m_controller.parseChar('g');
}

TEST_F(ControllerTest, ReportGripperPWM) {
  EXPECT_CALL(m_controller, reportPWM(1860.0));
  m_controller.parseChar('G');
}

TEST_F(ControllerTest, UseBase) {
  EXPECT_EQ(-90, m_controller.limitArm(BASE, -90));
  EXPECT_EQ( 90, m_controller.limitArm(BASE,  90));
}

TEST_F(ControllerTest, RestrictElbow) {
  m_controller.curve(SHOULDER).stop(0);
  EXPECT_EQ( 45, m_controller.limitArm(ELBOW,  70));
  EXPECT_EQ(-45, m_controller.limitArm(ELBOW, -70));
}

TEST_F(ControllerTest, RestrictElbowRelativeToShoulder) {

  EXPECT_EQ( 55, m_controller.limitArm(ELBOW, 70));
  EXPECT_EQ(-35, m_controller.limitArm(ELBOW,-70));
}

TEST_F(ControllerTest, UseElbowRestriction) {
  m_controller.parseChar('7');
  m_controller.parseChar('0');
  m_controller.parseChar('e');
  EXPECT_EQ(55, m_controller.curve(ELBOW).target());
}

TEST_F(ControllerTest, RestrictShoulder) {
  m_controller.curve(ELBOW).stop(0);
  EXPECT_EQ( 45, m_controller.limitArm(SHOULDER, 70));
  EXPECT_EQ(-45, m_controller.limitArm(SHOULDER,-70));
}

TEST_F(ControllerTest, RestrictShoulderRelativeToElbow) {
  EXPECT_EQ( 25, m_controller.limitArm(SHOULDER, 70));
  EXPECT_EQ(-65, m_controller.limitArm(SHOULDER,-70));
}

TEST_F(ControllerTest, UpdateInformsServos) {
  m_controller.targetAngle(BASE, 0);
  EXPECT_CALL(m_controller, writePWM(BASE    ,2040));
  EXPECT_CALL(m_controller, writePWM(SHOULDER,1380));
  EXPECT_CALL(m_controller, writePWM(ELBOW   ,1740));
  EXPECT_CALL(m_controller, writePWM(GRIPPER ,1860));
  m_controller.update(0);
}

TEST_F(ControllerTest, UpdateAppliesTargets) {
  m_controller.targetAngle(BASE, 0);
  EXPECT_CALL(m_controller, writePWM(BASE    ,1500));
  EXPECT_CALL(m_controller, writePWM(SHOULDER,1380));
  EXPECT_CALL(m_controller, writePWM(ELBOW   ,1740));
  EXPECT_CALL(m_controller, writePWM(GRIPPER ,1860));
  m_controller.update(10);
}

TEST_F(ControllerTest, StopDrives) {
  for (int i=0; i<DRIVES; i++)
    m_controller.targetAngle(i, 0);
  EXPECT_CALL(m_controller, writePWM(BASE    ,2040));
  EXPECT_CALL(m_controller, writePWM(SHOULDER,1380));
  EXPECT_CALL(m_controller, writePWM(ELBOW   ,1740));
  EXPECT_CALL(m_controller, writePWM(GRIPPER ,1860));
  m_controller.parseChar('x');
  m_controller.update(10);
}

TEST_F(ControllerTest, AdaptDuration) {
  m_controller.targetAngle(BASE, -35);
  m_controller.targetAngle(SHOULDER, 0);
  EXPECT_NEAR(m_controller.curve(BASE).timeRemaining(), 2 * m_controller.curve(SHOULDER).timeRemaining(), 0.001);
}

TEST_F(ControllerTest, ApproachTeachPoint) {
  m_controller.parseChar('\'');
  m_controller.parseChar('b');
  EXPECT_CALL(m_controller, writePWM(BASE    ,1500));
  EXPECT_CALL(m_controller, writePWM(SHOULDER,1500));
  EXPECT_CALL(m_controller, writePWM(ELBOW   ,1500));
  EXPECT_CALL(m_controller, writePWM(GRIPPER ,1500));
  m_controller.update(10);
}

TEST_F(ControllerTest, FinishTeachPointSelection) {
  m_controller.parseChar('\'');
  m_controller.parseChar('b');
  EXPECT_CALL(m_controller, reportAngle(45));
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, OnlyAlphabeticTeachPoints) {
  m_controller.parseChar('\'');
  m_controller.parseChar('\'');
  EXPECT_EQ(45, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, WrongTeachPointKeyStopsDrives) {
  m_controller.targetAngle(BASE, 0);
  m_controller.parseChar('\'');
  m_controller.parseChar('\'');
  EXPECT_EQ(45, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, TeachPointSavingClearsNumber) {
  m_controller.parseChar('0');
  m_controller.parseChar('\'');
  m_controller.parseChar('b');
  EXPECT_CALL(m_controller, reportAngle(45));
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, SaveTeachPoint) {
  m_controller.parseChar('m');
  m_controller.parseChar('b');
  m_controller.parseChar('\'');
  m_controller.parseChar('b');
  EXPECT_EQ( 45, m_controller.curve(BASE    ).target());
  EXPECT_EQ(-10, m_controller.curve(SHOULDER).target());
  EXPECT_EQ( 20, m_controller.curve(ELBOW   ).target());
  EXPECT_EQ( 30, m_controller.curve(GRIPPER ).target());
}

TEST_F(ControllerTest, FinishTeachPointSaving) {
  m_controller.parseChar('m');
  m_controller.parseChar('b');
  EXPECT_CALL(m_controller, reportAngle(45));
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, TeachPointLoadingClearsNumber) {
  m_controller.parseChar('0');
  m_controller.parseChar('m');
  m_controller.parseChar('b');
  EXPECT_CALL(m_controller, reportAngle(45));
  m_controller.parseChar('b');
}

TEST_F(ControllerTest, WrongTeachPointSelectionStopsDrives) {
  m_controller.targetAngle(BASE, 0);
  m_controller.parseChar('m');
  m_controller.parseChar('\'');
  EXPECT_EQ(45, m_controller.curve(BASE).target());
}

TEST_F(ControllerTest, SaveSecondTeachPoint) {
  m_controller.parseChar('m');
  m_controller.parseChar('b');
  m_controller.parseChar('\'');
  m_controller.parseChar('a');
  EXPECT_EQ(0, m_controller.curve(BASE    ).target());
  EXPECT_EQ(0, m_controller.curve(SHOULDER).target());
  EXPECT_EQ(0, m_controller.curve(ELBOW   ).target());
  EXPECT_EQ(0, m_controller.curve(GRIPPER ).target());
}

TEST_F(ControllerTest, SynchroniseProfilesWithBase) {
  m_controller.parseChar('m');
  m_controller.parseChar('a');
  m_controller.curve(BASE).stop(0);
  m_controller.targetTeachPoint(0);
  float time = m_controller.curve(BASE).timeRemaining();
  EXPECT_LT(0, time);
  EXPECT_FLOAT_EQ(time, m_controller.curve(SHOULDER).timeRemaining());
  EXPECT_FLOAT_EQ(time, m_controller.curve(ELBOW   ).timeRemaining());
  EXPECT_FLOAT_EQ(time, m_controller.curve(GRIPPER ).timeRemaining());
}

TEST_F(ControllerTest, SynchroniseProfilesWithShoulder) {
  m_controller.parseChar('m');
  m_controller.parseChar('a');
  m_controller.curve(SHOULDER).stop(0);
  m_controller.targetTeachPoint(0);
  float time = m_controller.curve(SHOULDER).timeRemaining();
  EXPECT_LT(0, time);
  EXPECT_FLOAT_EQ(time, m_controller.curve(BASE    ).timeRemaining());
  EXPECT_FLOAT_EQ(time, m_controller.curve(ELBOW   ).timeRemaining());
  EXPECT_FLOAT_EQ(time, m_controller.curve(GRIPPER ).timeRemaining());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
