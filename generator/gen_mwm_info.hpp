#pragma once

#include "coding/read_write_utils.hpp"

#include "base/assert.hpp"

#include "std/algorithm.hpp"
#include "std/fstream.hpp"
#include "std/iostream.hpp"
#include "std/string.hpp"
#include "std/utility.hpp"

namespace gen
{
template <class T> class Accumulator
{
protected:
  vector<T> m_data;

public:
  typedef T ValueT;

  void Add(T const & t) { m_data.push_back(t); }

  template <class TSink> void Flush(TSink & sink) const
  {
    rw::WriteVectorOfPOD(sink, m_data);
  }

  template <class TSource> void Read(TSource & src)
  {
    rw::ReadVectorOfPOD(src, m_data);
  }

  vector<T> const & GetData() const { return m_data; }
};

class OsmID2FeatureID : public Accumulator<pair<uint64_t, uint32_t>>
{
  typedef Accumulator<ValueT> BaseT;

  struct LessID
  {
    bool operator() (ValueT const & r1, ValueT const & r2) const { return r1.first < r2.first; }
    bool operator() (uint64_t const & r1, ValueT const & r2) const { return r1 < r2.first; }
    bool operator() (ValueT const & r1, uint64_t const & r2) const { return r1.first < r2; }
  };

public:
  template <class TSink> void Flush(TSink & sink)
  {
    sort(m_data.begin(), m_data.end());

    for (size_t i = 1; i < m_data.size(); ++i)
      CHECK_NOT_EQUAL(m_data[i-1].first, m_data[i].first, ());

    BaseT::Flush(sink);
  }

  uint32_t GetFeatureID(uint64_t osmID) const
  {
    vector<ValueT>::const_iterator i = lower_bound(m_data.begin(), m_data.end(), osmID, LessID());
    if (i != m_data.end() && i->first == osmID)
      return i->second;
    else
      return 0;
  }
};
}  // namespace gen
