#include "utils/interval.h"

#include <gtest/gtest.h>

bool operator==(Point f, Point s) { return f.x == s.x && f.y == s.y; }

TEST(interval_suite, normal_test_0) {
  std::vector<Point> points{{0, 1}, {1, 1}, {2, 4}, {3, -4}, {4, 0}};
  Intervals intervals(points);
  auto result = intervals.GetInterval(0.0);
  ASSERT_EQ(result, 0);
}

TEST(interval_suite, normal_test_1) {
  std::vector<Point> points{{0, 1}, {1, 1}, {2, 4}, {3, -4}, {4, 0}};
  Intervals intervals(points);
  auto result = intervals.GetInterval(4.0);
  ASSERT_EQ(result, 3);
}

TEST(interval_suite, normal_test_2) {
  std::vector<Point> points{{0, 1}, {1, 1}, {2, 4}, {3, -4}, {4, 0}};
  Intervals intervals(points);
  auto result = intervals.GetInterval(1.0);
  ASSERT_EQ(result, 1);
}

TEST(interval_suite, invalid_range_overflow_1) {
  std::vector<Point> points{{0, 1}, {1, 1}, {2, 4}, {3, -4}, {4, 0}};
  Intervals intervals(points);
  ASSERT_EQ(intervals.GetInterval(4.0 + 1e-4, 1e-8), 4);
}

TEST(interval_suite, invalid_range_underflow_1) {
  std::vector<Point> points{{0, 1}, {1, 1}, {2, 4}, {3, -4}, {4, 0}};
  Intervals intervals(points);
  ASSERT_EQ(intervals.GetInterval(-1e-4, 1e-8), -1);
}

TEST(interval_suite, invalid_range_overflow_2) {
  std::vector<Point> points{{0, 1}, {1, 1}, {2, 4}, {3, -4}, {4, 0}};
  Intervals intervals(points);
  ASSERT_EQ(intervals.GetInterval(4.0 + 1e-8, 1e-4), 3);
}

TEST(interval_suite, invalid_range_underflow_2) {
  std::vector<Point> points{{0, 1}, {1, 1}, {2, 4}, {3, -4}, {4, 0}};
  Intervals intervals(points);
  ASSERT_EQ(intervals.GetInterval(1e-8, 1e-4), 0);
}
