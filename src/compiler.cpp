#include "compiler.hpp"

#include <compiler.h>
#include <gherkin_document_event.h>
#include <pickle_event.h>
#include <array>
#include <cstdio>
#include "fmem.h"

namespace gherkin {
inline namespace v1 {

compiler::compiler(const std::string &uri) : uri_{uri}, compiler_{Compiler_new(), Compiler_delete} {}

std::vector<std::string> compiler::compile(const std::unique_ptr<const GherkinDocument, void(*)(const GherkinDocument*)> &document) {
  std::vector<std::string> pickles;
  if (!Compiler_compile(compiler_.get(), document.get())) {
    while (Compiler_has_more_pickles(compiler_.get())) {
      fmem fm{};
      fmem_init(&fm);
      const auto buffer = fmem_open(&fm, "w");
      fseek(buffer, 0, SEEK_SET);
      const auto event = reinterpret_cast<const Event *>(PickleEvent_new(uri_.c_str(), Compiler_next_pickle(compiler_.get())));
      Event_print(event, buffer);
      fflush(buffer);
      Event_delete(event);
      void* base{};
      std::size_t size{};
      fmem_mem(&fm, &base, &size);
      std::string pickle{reinterpret_cast<char*>(base), size};
      pickles.emplace_back(pickle);
      fclose(buffer);
      fmem_term(&fm);
    }
  }
  return pickles;
}

std::string compiler::ast(const std::unique_ptr<const GherkinDocument, void(*)(const GherkinDocument*)> &document) {
  fmem fm{};
  fmem_init(&fm);
  const auto buffer = fmem_open(&fm, "w");
  const auto gherkin_document_event = GherkinDocumentEvent_new(uri_.c_str(), document.get());
  Event_print(reinterpret_cast<const Event *>(gherkin_document_event), buffer);
  fflush(buffer);
  Event_delete(reinterpret_cast<const Event *>(gherkin_document_event));
  void* base{};
  std::size_t size{};
  fmem_mem(&fm, &base, &size);
  std::string ast{reinterpret_cast<char*>(base), size};
  fclose(buffer);
  fmem_term(&fm);
  return ast;
}

}  // v1
}  // gherkin
