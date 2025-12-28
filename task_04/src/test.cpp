#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "solution.hpp"

namespace {

namespace fs = std::filesystem;

bool EndsWith(const std::string& s, const std::string& suf) {
  if (s.size() < suf.size()) return false;
  return s.compare(s.size() - suf.size(), suf.size(), suf) == 0;
}

fs::path FindTestsDir() {
  const std::vector<fs::path> candidates = {
      fs::path("tests"),
      fs::path("../tests"),
      fs::path("../../tests"),
      fs::path("../../../tests"),
      fs::path("../../../../tests"),
  };
  for (const auto& p : candidates) {
    if (fs::exists(p) && fs::is_directory(p)) return p;
  }
  return fs::path();
}

std::vector<fs::path> ListInFiles(const fs::path& dir) {
  std::vector<fs::path> files;
  for (const auto& entry : fs::directory_iterator(dir)) {
    if (!entry.is_regular_file()) continue;
    const std::string name = entry.path().filename().string();
    if (EndsWith(name, ".in")) files.push_back(entry.path());
  }
  std::sort(files.begin(), files.end());
  return files;
}

std::string ReadFileStrict(const fs::path& path) {
  std::ifstream f(path);
  if (!f.is_open()) return std::string();
  std::ostringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

std::string RunSolve(const std::string& input) {
  std::istringstream in(input);
  std::ostringstream out;
  SolveTask04(in, out);
  return out.str();
}

}  // namespace

TEST(Task04FileCases, All) {
  const fs::path tests_dir = FindTestsDir();
  ASSERT_FALSE(tests_dir.empty());

  const std::vector<fs::path> in_files = ListInFiles(tests_dir);
  ASSERT_FALSE(in_files.empty());

  for (const auto& in_path : in_files) {
    fs::path out_path = in_path;
    out_path.replace_extension(".out");

    SCOPED_TRACE(in_path.filename().string());

    ASSERT_TRUE(fs::exists(out_path)) << out_path.string();
    ASSERT_GT(fs::file_size(out_path), 0u) << out_path.string();

    const std::string input = ReadFileStrict(in_path);
    ASSERT_GT(input.size(), 0u) << in_path.string();

    const std::string expected = ReadFileStrict(out_path);
    ASSERT_GT(expected.size(), 0u) << out_path.string();

    ASSERT_EQ(RunSolve(input), expected);
  }
}
