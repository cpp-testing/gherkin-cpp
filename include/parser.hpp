#pragma once

#include <iostream>
#include <memory>
#include <string>

struct GherkinDocument;
struct Builder;
struct Parser;
struct TokenMatcher;

namespace gherkin {
inline namespace v1 {
class parsing_error final : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class parser {
 public:
  explicit parser(const std::wstring & = L"en");

  std::unique_ptr<const GherkinDocument, void (*)(const GherkinDocument *)>
  parse(const std::wstring &feature);

 private:
  std::wstring language_;
  std::unique_ptr<TokenMatcher, void (*)(TokenMatcher *)> token_matcher_;
  std::unique_ptr<Builder, void (*)(Builder *)> builder_;
  std::unique_ptr<Parser, void (*)(Parser *)> parser_;
};

}  // v1
}  // gherkin
