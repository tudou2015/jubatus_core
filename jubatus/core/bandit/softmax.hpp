// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2015 Preferred Networks and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef JUBATUS_CORE_BANDIT_SOFTMAX_HPP_
#define JUBATUS_CORE_BANDIT_SOFTMAX_HPP_

#include <string>

#include "jubatus/util/data/serialization.h"
#include "jubatus/util/data/optional.h"
#include "jubatus/util/math/random.h"
#include "bandit_base.hpp"
#include "summation_storage.hpp"

namespace jubatus {
namespace core {
namespace bandit {

class softmax : public bandit_base {
 public:
  struct config {
    bool assume_unrewarded;
    double tau;
    jubatus::util::data::optional<int64_t> seed;

    template<class Ar>
    void serialize(Ar& ar) {
      ar & JUBA_MEMBER(assume_unrewarded)
        & JUBA_MEMBER(tau)
        & JUBA_MEMBER(seed);
    }
  };

  explicit softmax(const config& conf);

  std::string select_arm(const std::string& player_id);

  bool register_arm(const std::string& arm_id);
  bool delete_arm(const std::string& arm_id);

  bool register_reward(const std::string& player_id,
                       const std::string& arm_id,
                       double reward);

  arm_info_map get_arm_info(const std::string& player_id) const;

  bool reset(const std::string& player_id);
  void clear();

  std::string name() const {
    return "softmax";
  }

  void pack(framework::packer& pk) const;
  void unpack(msgpack::object o);

  void get_diff(diff_t& diff) const;
  bool put_diff(const diff_t& diff);
  void mix(const diff_t& lhs, diff_t& rhs) const;
  storage::version get_version() const;

 private:
  double tau_;
  jubatus::util::math::random::mtrand rand_;
  summation_storage s_;
};

}  // namespace bandit
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_BANDIT_SOFTMAX_HPP_
