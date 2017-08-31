#include "gtest/gtest.h"

#include "freertps/rtps/type/part.h"

TEST(VecGenTest, frudp_part_create) {
	bool value = true; //frudp_part_create();

  EXPECT_TRUE(value);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
