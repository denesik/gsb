#pragma once
#include <map>
#include <algorithm>
#include <assert.h>
#include <type_traits>

namespace gsb
{
  template<typename Int, typename Val>
  using range_value = std::pair<std::pair<Int, Val>, Int>;

  template<typename Int, typename Val>
  class interval_set : public std::map<std::pair<Int, Val>, Int>
  {
  public:
    using container = std::map<std::pair<Int, Val>, Int>;
    using range_value = range_value<Int, Val>;

    std::pair<interval_set::iterator, bool> insert(const range& range) {
      assert(range.first <= range.second);

      interval_set::iterator after = upper_bound(range.first.first), insert_range;

      if (after == begin() || std::prev(after)->second < range.first.first) {
        insert_range = container::insert(after, range);
      }
      else {
        insert_range = std::prev(after);
        if (insert_range->second >= range.second) {
          return std::pair<interval_set::iterator, bool>(insert_range, false);
        }
        else {
          insert_range = range.second;
        }
      }

      while (after != end() && range.second >= after->first.first) {
        insert_range->second = std::max(after->second, insert_range->second);
        after = erase(after);
      }

      return std::pair<interval_set::iterator, bool>(insert_range, true);
    }

  };
}