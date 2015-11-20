#include <gtest/gtest.h>
#include "curve.hh"

class StationaryTest: public ::testing::Test {
public:
  StationaryTest(void): m_curve(0) {}
protected:
  Curve m_curve;
};

TEST_F(StationaryTest, ShouldStartWithGivenValue) {
  EXPECT_EQ(0, m_curve.pos());
}

TEST_F(StationaryTest, ShouldStayConstantOverTime) {
  m_curve.advance(5);
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

TEST_F(MovingForwardTest, ShouldAdvanceWithTime) {
  m_curve.advance(1);
  EXPECT_EQ(105, m_curve.pos());
}

TEST_F(MovingForwardTest, ShouldAccelerateOverTime) {
  m_curve.advance(1);
  m_curve.advance(1);
  EXPECT_EQ(120, m_curve.pos());
}

TEST_F(MovingForwardTest, ShouldUseSpecifiedAcceleration) {
  Curve curve(0, 20);
  curve.retarget(200);
  curve.advance(1);
  EXPECT_EQ(10, curve.pos());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
