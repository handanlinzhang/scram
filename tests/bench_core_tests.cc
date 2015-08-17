/*
 * Copyright (C) 2014-2015 Olzhas Rakhimov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

#include <boost/assign/list_inserter.hpp>

#include "risk_analysis_tests.h"

namespace scram {
namespace test {

// Benchmark Tests for [A or B or C] fault tree.
// Test Minimal cut sets and total probabilty.
TEST_F(RiskAnalysisTest, ABC) {
  std::string tree_input = "./share/scram/input/core/abc.xml";
  std::string A = "a";  // 0.1
  std::string B = "b";  // 0.2
  std::string C = "c";  // 0.3
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0.496, p_total());  // Total prob check.
  // Minimal cut set check.
  cut_set.insert(A);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert(B);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert(C);
  mcs.insert(cut_set);
  EXPECT_EQ(3, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Benchmark Tests for [AB or BC] fault tree.
// Test Minimal cut sets and total probability.
TEST_F(RiskAnalysisTest, AB_BC) {
  std::string tree_input = "./share/scram/input/core/ab_bc.xml";
  std::string A = "a";  // 0.1
  std::string B = "b";  // 0.2
  std::string C = "c";  // 0.3
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0.074, p_total());  // Total prob check.
  // Minimal cut set check.
  cut_set.insert(A);
  cut_set.insert(B);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert(B);
  cut_set.insert(C);
  mcs.insert(cut_set);
  EXPECT_EQ(2, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Simple verification tests for Atleast gate fault tree.
// Test Minimal cut sets and total probabilty.
TEST_F(RiskAnalysisTest, ATLEAST) {
  std::string tree_input = "./share/scram/input/core/atleast.xml";
  std::string A = "a";  // 0.1
  std::string B = "b";  // 0.2
  std::string C = "c";  // 0.3
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0.098, p_total());  // Total prob check.
  // Minimal cut set check.
  cut_set.insert(A);
  cut_set.insert(C);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert(A);
  cut_set.insert(B);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert(B);
  cut_set.insert(C);
  mcs.insert(cut_set);
  EXPECT_EQ(3, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Benchmark tests for NOT gate.
// [A OR NOT A]
// This produces UNITY top gate.
TEST_F(RiskAnalysisTest, A_OR_NOT_A) {
  std::string tree_input = "./share/scram/input/core/a_or_not_a.xml";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(1, p_total());  // Total prob check.
  // Minimal cut set check.
  // Special case of one empty cut set in a container.
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// [A OR NOT B]
TEST_F(RiskAnalysisTest, A_OR_NOT_B) {
  std::string tree_input = "./share/scram/input/core/a_or_not_b.xml";
  std::string A = "a";  // 0.1
  std::string B = "b";  // 0.2
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0.82, p_total());  // Total prob check.
  // Minimal cut set check.
  cut_set.insert(A);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert("not " + B);
  mcs.insert(cut_set);
  EXPECT_EQ(2, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// [A AND NOT A]
TEST_F(RiskAnalysisTest, A_AND_NOT_A) {
  std::string tree_input = "./share/scram/input/core/a_and_not_a.xml";
  std::string A = "a";  // 0.1
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0, p_total());  // Total prob check.
  // Minimal cut set check.
  EXPECT_EQ(0, min_cut_sets().size());
}

// [A AND NOT B]
TEST_F(RiskAnalysisTest, A_AND_NOT_B) {
  std::string tree_input = "./share/scram/input/core/a_and_not_b.xml";
  std::string A = "a";  // 0.1
  std::string B = "b";  // 0.2
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0.08, p_total());  // Total prob check.
  // Minimal cut set check.
  cut_set.insert(A);
  cut_set.insert("not " + B);
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// [A OR (B, NOT A)]
TEST_F(RiskAnalysisTest, A_OR_NOT_AB) {
  std::string tree_input = "./share/scram/input/core/a_or_not_ab.xml";
  std::string A = "a";  // 0.1
  std::string B = "b";  // 0.2
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0.28, p_total());  // Total prob check.
  // Minimal cut set check.
  cut_set.insert(A);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert(B);
  mcs.insert(cut_set);
  EXPECT_EQ(2, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Uncertainty report for Unity case.
TEST_F(RiskAnalysisTest, MC_A_OR_NOT_A) {
  std::string tree_input = "./share/scram/input/core/a_or_not_a.xml";
  settings.uncertainty_analysis(true);
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
}

// [A OR NOT B] FTA MC
TEST_F(RiskAnalysisTest, MC_A_OR_NOT_B) {
  settings.uncertainty_analysis(true);
  std::string tree_input = "./share/scram/input/core/a_or_not_b.xml";
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
}

// Repeated negative gate expansion.
TEST_F(RiskAnalysisTest, MultipleParentNegativeGate) {
  std::string tree_input = "./share/scram/input/core/"
                           "multiple_parent_negative_gate.xml";
  std::string A = "a";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  // Minimal cut set check.
  cut_set.insert("not " + A);
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Checks for NAND UNITY top gate cases.
TEST_F(RiskAnalysisTest, NAND_UNITY) {
  std::string tree_input = "./share/scram/input/core/nand_or_equality.xml";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(1, p_total());  // Total prob check.
  // Minimal cut set check.
  // Special case of one empty cut set in a container.
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Checks for OR UNITY top gate cases.
TEST_F(RiskAnalysisTest, OR_UNITY) {
  std::string tree_input =
      "./share/scram/input/core/not_and_or_equality.xml";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(1, p_total());  // Total prob check.
  // Minimal cut set check.
  // Special case of one empty cut set in a container.
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Checks for UNITY due to house event.
TEST_F(RiskAnalysisTest, HOUSE_UNITY) {
  std::string tree_input = "./share/scram/input/core/unity.xml";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(1, p_total());  // Total prob check.
  // Minimal cut set check.
  // Special case of one empty cut set in a container.
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Checks for NULL due to house event.
TEST_F(RiskAnalysisTest, HOUSE_NULL) {
  std::string tree_input = "./share/scram/input/core/null.xml";
  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0, p_total());  // Total prob check.
  // Minimal cut set check.
  // Special case of one empty cut set in a container.
  EXPECT_EQ(0, min_cut_sets().size());
}

// Benchmark Tests for [A xor B xor C] fault tree.
// Test Minimal cut sets and total probabilty.
TEST_F(RiskAnalysisTest, XOR_ABC) {
  std::string tree_input = "./share/scram/input/core/xor.xml";
  std::string A = "a";  // 0.1
  std::string B = "b";  // 0.2
  std::string C = "c";  // 0.3
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_DOUBLE_EQ(0.404, p_total());  // Total prob check.
  // Minimal cut set check.
  cut_set.insert(A);
  cut_set.insert(B);
  cut_set.insert(C);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert(A);
  cut_set.insert("not " + B);
  cut_set.insert("not " + C);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert("not " + A);
  cut_set.insert(B);
  cut_set.insert("not " + C);
  mcs.insert(cut_set);
  cut_set.clear();
  cut_set.insert("not " + A);
  cut_set.insert("not " + B);
  cut_set.insert(C);
  mcs.insert(cut_set);
  EXPECT_EQ(4, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Checks for top gate of NOT with a single basic event child.
TEST_F(RiskAnalysisTest, NOT_A) {
  std::string tree_input = "./share/scram/input/core/not_a.xml";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  // Minimal cut set check.
  cut_set.insert("not onlychild");
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Checks for top gate of NULL with a single basic event child.
TEST_F(RiskAnalysisTest, NULL_A) {
  std::string tree_input = "./share/scram/input/core/null_a.xml";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  // Minimal cut set check.
  cut_set.insert("onlychild");
  mcs.insert(cut_set);
  EXPECT_EQ(1, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Benchmark Tests for Beta factor common cause failure model.
// Test Minimal cut sets and total probabilty.
TEST_F(RiskAnalysisTest, BetaFactorCCF) {
  std::string tree_input = "./share/scram/input/core/beta_factor_ccf.xml";
  std::string p1 = "[pumpone]";
  std::string p2 = "[pumptwo]";
  std::string p3 = "[pumpthree]";
  std::string v1 = "[valveone]";
  std::string v2 = "[valvetwo]";
  std::string v3 = "[valvethree]";
  std::string pumps = "[pumpone pumpthree pumptwo]";
  std::string valves = "[valveone valvethree valvetwo]";
  std::set<std::string> cut_set;
  std::set< std::set<std::string> > mcs;  // For expected min cut sets.

  settings.ccf_analysis(true).probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_NEAR(0.04308, p_total(), 1e-5);  // Total prob check.
  // Minimal cut set check.
  using namespace boost::assign;
  insert(cut_set) (pumps);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (valves);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (v1) (v2) (v3);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (p1) (v2) (v3);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (p2) (v1) (v3);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (p3) (v1) (v2);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (p3) (p2) (v1);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (p1) (p2) (v3);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (p1) (p3) (v2);
  mcs.insert(cut_set);
  cut_set.clear();
  insert(cut_set) (p1) (p2) (p3);
  mcs.insert(cut_set);
  EXPECT_EQ(10, min_cut_sets().size());
  EXPECT_EQ(mcs, min_cut_sets());
}

// Benchmark Tests for Phi factor common cause failure calculations.
// Test Minimal cut sets and total probabilty.
TEST_F(RiskAnalysisTest, PhiFactorCCF) {
  std::string tree_input = "./share/scram/input/core/phi_factor_ccf.xml";
  settings.ccf_analysis(true).num_sums(3).probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_NEAR(0.04109, p_total(), 1e-5);  // Total prob check.
  EXPECT_EQ(34, min_cut_sets().size());
  std::vector<int> distr(4, 0);
  distr[1] = 2;
  distr[2] = 24;
  distr[3] = 8;
  EXPECT_EQ(distr, McsDistribution());
}

// Benchmark Tests for MGL factor common cause failure calculations.
// Test Minimal cut sets and total probability.
TEST_F(RiskAnalysisTest, MGLFactorCCF) {
  std::string tree_input = "./share/scram/input/core/mgl_ccf.xml";
  settings.ccf_analysis(true).num_sums(3).probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_NEAR(0.01631, p_total(), 1e-5);  // Total prob check.
  EXPECT_EQ(34, min_cut_sets().size());
  std::vector<int> distr(4, 0);
  distr[1] = 2;
  distr[2] = 24;
  distr[3] = 8;
  EXPECT_EQ(distr, McsDistribution());
}

// Benchmark Tests for Alpha factor common cause failure calculations.
// Test Minimal cut sets and total probability.
TEST_F(RiskAnalysisTest, AlphaFactorCCF) {
  std::string tree_input = "./share/scram/input/core/alpha_factor_ccf.xml";
  settings.ccf_analysis(true).num_sums(3).probability_analysis(true);
  ASSERT_NO_THROW(ProcessInputFile(tree_input));
  ASSERT_NO_THROW(ran->Analyze());
  EXPECT_NEAR(0.03093, p_total(), 1e-5);  // Total prob check.
  EXPECT_EQ(34, min_cut_sets().size());
  std::vector<int> distr(4, 0);
  distr[1] = 2;
  distr[2] = 24;
  distr[3] = 8;
  EXPECT_EQ(distr, McsDistribution());
}

}  // namespace test
}  // namespace scram