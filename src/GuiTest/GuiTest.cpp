#define BOOST_TEST_MODULE Gui Test
#include <boost/test/included/unit_test.hpp>
#include <IGui.h>
#include <Magnum\Timeline.h>

class DataBase;

class TestGui : public NoContextGui
{
public:

  // Inherited via NoContextGui
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context) override
  {
  }
};

BOOST_AUTO_TEST_CASE(register_unregister_guictx)
{
  GuiCtx ctx(*static_cast<DataBase*>(nullptr)); //TODO: do it with mock
  {
    BOOST_TEST(ctx.RegisteredCount() == 0);

    TestGui test;
    auto link = ctx.Register(test);
    BOOST_TEST(ctx.RegisteredCount() == 1);

    {
      TestGui test2;
      auto link2 = ctx.Register(test2);
      BOOST_TEST(ctx.RegisteredCount() == 2);

      {
        auto link3 = std::move(link2);
        BOOST_TEST(ctx.RegisteredCount() == 2);
      }
    }

    BOOST_TEST(ctx.RegisteredCount() == 1);
  }

  BOOST_TEST(ctx.RegisteredCount() == 0);
}


constexpr int gTestContextValue = 5;
class TestContext : public IContext
{
public:
  int test = 0;
};

class TestContextWrong : public IContext
{
public:
  std::string test_s;
};

class TestContextGui : public ContextGui<TestContext>
{
public:
  // Inherited via NoContextGui
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context) override
  {
    auto con = static_cast<TestContext &>(context);

    BOOST_TEST(con.test == gTestContextValue);
  }
};

BOOST_AUTO_TEST_CASE(register_context_guictx)
{
  Magnum::Timeline tl;
  GuiCtx ctx(*static_cast<DataBase*>(nullptr)); //TODO: do it with mock
  TestContextGui test;
  auto link = ctx.Register(test);
  BOOST_TEST(ctx.RegisteredCount() == 1);

  link.Context<TestContext>().test = gTestContextValue;
  link.DrawGui(tl);
}