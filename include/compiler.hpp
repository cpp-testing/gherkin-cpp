#pragma once

#include <memory>
#include <string>
#include <vector>

struct GherkinDocument;
struct Compiler;

namespace gherkin {
inline namespace v1 {
class compiler {
 public:
  explicit compiler(const std::string &uri);
  std::vector<std::string> compile(const GherkinDocument *);

  std::vector<std::string> compile(const std::unique_ptr<const GherkinDocument, void(*)(const GherkinDocument*)>&);
  std::string ast(const std::unique_ptr<const GherkinDocument, void(*)(const GherkinDocument*)>&);

 private:
  std::string uri_;
  std::unique_ptr<Compiler, void (*)(Compiler *)> compiler_;
};

}  // v1
}  // gherkin
