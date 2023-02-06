#include <service.hpp>

#include <gtest/gtest.h>

TEST(service, empty)
{
  EXPECT_TRUE(service("").empty());
}

TEST(service, params)
{
  const std::string in = R"(params:${param},${tag},${id},${line_count}
params:p=${param},t=${tag},id=${id},l=${line_count}
params:t=${tag},l=${line_count},id=${id},p=${param}
params:foo,l=${line_count},bar,p=${param}"
)";
  const std::string expected = R"(params:0,123,foobar,0
params:p=0,t=123,id=foobar,l=1
params:t=123,l=2,id=foobar,p=0
params:foo,l=3,bar,p=0"
)";

  EXPECT_EQ(expected, service(in));
}

TEST(service, key_value)
{
  const std::string in = R"(value:key-24
set:key-24:value-42
value:key-24
)";
  const std::string expected = R"(value:
value:value-42
)";

  EXPECT_EQ(expected, service(in));
}

TEST(service, unknown)
{
  const std::string in = R"(unknown:a
unknown:a:bc:d
)";

  EXPECT_EQ(in, service(in));
}

TEST(service, mixed)
{
  const std::string in = R"(params:${param},${tag},${id},${line_count}
value:key-24
unknown:a
set:key-24:value-42
params:p=${param},t=${tag},id=${id},l=${line_count}
unknown:a:bc:d
params:t=${tag},l=${line_count},id=${id},p=${param}
value:key-24
params:foo,l=${line_count},bar,p=${param}
)";
  const std::string expected = R"(params:0,123,foobar,0
value:
unknown:a
params:p=0,t=123,id=foobar,l=3
unknown:a:bc:d
params:t=123,l=5,id=foobar,p=0
value:value-42
params:foo,l=7,bar,p=0
)";

  EXPECT_EQ(expected, service(in));
}
