#define BOOST_TEST_MODULE Ringbuffer Test
#include <Magnum\Magnum.h>
#include "../RingBuffer.h"
#include <boost/test/included/unit_test.hpp>
#include <unordered_set>

struct Storage
{

};

static Magnum::Vector3i atZeroSeq[] = {
  { -1,  0, -1 },
  { -1,  0,  0 },
  { -1,  0,  1 },
         
  {  0,  0, -1 },
  {  0,  0,  0 },
  {  0,  0,  1 },
         
  {  1,  0, -1 },
  {  1,  0,  0 },
  {  1,  0,  1 },
};

static std::unordered_set<Magnum::Vector3i> atHundredSeq = {
  { 99, 0,  99 },
  { 99, 0, 100 },
  { 99, 0, 101 },

  { 100, 0,  99 },
  { 100, 0, 100 },
  { 100, 0, 101 },

  { 101, 0,  99 },
  { 101, 0, 100 },
  { 101, 0, 101 },
};


// 1 1 1
// 1 1 1      <- {0, 0}
// 1 1 0 2 2  
//     2 2 2  <- {2, 2}
//     2 2 2
static std::unordered_set<Magnum::Vector3i> removingSet = {
  { -1,  0, -1 },
  { -1,  0,  0 },
  { -1,  0,  1 },

  {  0,  0, -1 },
  {  0,  0,  0 },
  {  0,  0,  1 },
     
  {  1,  0, -1 },
  {  1,  0,  0 },
  //{  1,  0,  1 }, 0
};

static std::unordered_set<Magnum::Vector3i> addingSet = {
  //{ 1, 0, 1 }, 0
  { 1, 0, 2 },
  { 1, 0, 3 },

  { 2, 0, 1 },
  { 2, 0, 2 },
  { 2, 0, 3 },

  { 3, 0, 1 },
  { 3, 0, 2 },
  { 3, 0, 3 },
};

BOOST_AUTO_TEST_CASE(add_remove_test)
{
  int add_c = 0;
  auto add = [&add_c](const SPos &p) -> std::unique_ptr<Storage> {
    add_c++;

    return std::make_unique<Storage>();
  };

  int del_c = 0;
  auto del = [&del_c](std::unique_ptr<Storage> &s, const SPos &p) {
    del_c++;
  };

  RingBuffer<std::unique_ptr<Storage>> rb(Magnum::Vector2i(3, 3), add, del);

  BOOST_TEST(add_c == 49);
  BOOST_TEST(del_c == 0);
  add_c = del_c = 0;

  rb.UpdatePos({ 1, 0, 0 });

  BOOST_TEST(add_c == 7);
  BOOST_TEST(del_c == 7);
  add_c = del_c = 0;

  auto emptySector = rb.Get({ -3, 0, -3 });
  auto goodSector = rb.Get({ 0, 0, 0 });

  BOOST_TEST(!emptySector.is_initialized());
  BOOST_TEST(goodSector.is_initialized());

  rb.SetSize({ 1, 1 });

  BOOST_TEST(add_c == 9);
  BOOST_TEST(del_c == 49);
  add_c = del_c = 0;
}

BOOST_AUTO_TEST_CASE(exact_place_test)
{
  int c = 0;
  bool ok = true;
  auto add = [&c, &ok](const SPos &p) -> std::unique_ptr<Storage> {
    
    if(atZeroSeq[c] != p)
      ok = false;

    c++;
    return std::make_unique<Storage>();
  };

  auto del = [](std::unique_ptr<Storage> &s, const SPos &p) {
  };

  RingBuffer<std::unique_ptr<Storage>> rb(Magnum::Vector2i(1, 1), add, del);

  BOOST_TEST(ok);
}

BOOST_AUTO_TEST_CASE(full_replace_test)
{
  auto seq = atHundredSeq;
  auto add = [&seq](const SPos &p) -> std::unique_ptr<Storage> {
    seq.erase(p);
    return std::make_unique<Storage>();
  };

  auto del = [](std::unique_ptr<Storage> &s, const SPos &p) {
  };

  RingBuffer<std::unique_ptr<Storage>> rb(Magnum::Vector2i(1, 1), add, del);

  seq = atHundredSeq;

  rb.UpdatePos({ 100, 0, 100 });

  BOOST_TEST(seq.empty());
}

BOOST_AUTO_TEST_CASE(almost_full_replace_test)
{
  auto seqAdd = addingSet;
  auto add = [&seqAdd](const SPos &p) -> std::unique_ptr<Storage> {
    seqAdd.erase(p);
    return std::make_unique<Storage>();
  };

  auto seqDel = removingSet;
  auto del = [&seqDel](std::unique_ptr<Storage> &s, const SPos &p) {
    seqDel.erase(p);
  };

  RingBuffer<std::unique_ptr<Storage>> rb(Magnum::Vector2i(1, 1), add, del);

  seqAdd = addingSet;
  seqDel = removingSet;

  rb.UpdatePos({ 2, 0, 2 });

  BOOST_TEST(seqAdd.empty());
  BOOST_TEST(seqDel.empty());
}

static Magnum::Vector3i atZeroSeq5[] = {
  { -2,  0, -2 },
  { -2,  0, -1 },
  { -2,  0,  0 },
  { -2,  0,  1 },
  { -2,  0,  2 },

  { -1,  0, -2 },
  { -1,  0, -1 },
  { -1,  0,  0 },
  { -1,  0,  1 },
  { -1,  0,  2 },

  {  0,  0, -2 },
  {  0,  0, -1 },
  {  0,  0,  0 },
  {  0,  0,  1 },
  {  0,  0,  2 },

  {  1,  0, -2 },
  {  1,  0, -1 },
  {  1,  0,  0 },
  {  1,  0,  1 },
  {  1,  0,  2 },

  {  2,  0, -2 },
  {  2,  0, -1 },
  {  2,  0,  0 },
  {  2,  0,  1 },
  {  2,  0,  2 },
};

BOOST_AUTO_TEST_CASE(resize_test)
{
  size_t c5 = 0;
  bool ok5 = true;
  auto add = [&ok5, &c5](const SPos &p) -> std::unique_ptr<Storage> {
    if (atZeroSeq[c5] != p)
      ok5 = false;
    return std::make_unique<Storage>();
  };


  size_t c = 0;
  bool ok = true;
  auto del = [&c, &ok](std::unique_ptr<Storage> &s, const SPos &p) {
    if (atZeroSeq5[c] != p)
      ok = false;
  };

  RingBuffer<std::unique_ptr<Storage>> rb(Magnum::Vector2i(2, 2), add, del);

  ok = ok5 = true;
  c = c5 = 0;

  rb.SetSize({ 1, 1 });

  BOOST_TEST(ok);
  BOOST_TEST(ok5);
}