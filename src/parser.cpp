#include <ast_builder.h>
#include <attachment_event.h>
#include <builder.h>
#include <parser.h>
#include <string_token_scanner.h>
#include <array>
#include <cstdio>
#include <stdexcept>

#include "fmem.h"
#include "parser.hpp"

namespace gherkin {
inline namespace v1 {

parser::parser(const std::wstring &language)
    : language_{language}, token_matcher_{TokenMatcher_new(language_.c_str()), TokenMatcher_delete},
      builder_{AstBuilder_new(), AstBuilder_delete},
      parser_{Parser_new(builder_.get()), Parser_delete} {}

std::unique_ptr<const GherkinDocument, void (*)(const GherkinDocument *)>
parser::parse(const std::wstring &feature) {
  std::unique_ptr<TokenScanner, void (*)(TokenScanner *)>
    token_scanner{StringTokenScanner_new(feature.c_str()), StringTokenScanner_delete};

  if (Parser_parse(parser_.get(), token_matcher_.get(), token_scanner.get())) {
    const auto name = std::string{feature.begin(), feature.end()};
    fmem fm{};
    fmem_init(&fm);
    const auto buffer = fmem_open(&fm, "w");
    while (Parser_has_more_errors(parser_.get())) {
      fseek(buffer, 0, SEEK_SET);
      const auto error = Parser_next_error(parser_.get());
      const auto attachment_event = AttachmentEvent_new(name.c_str(), error->location);
      AttacnmentEvent_transfer_error_text(attachment_event, error);
      Event_print(reinterpret_cast<Event *>(attachment_event), buffer);
      fflush(buffer);
      Event_delete(reinterpret_cast<Event *>(attachment_event));
      Error_delete(error);
    }
    void* base{};
    std::size_t size{};
    fmem_mem(&fm, &base, &size);
    std::string err{reinterpret_cast<char*>(base), size};
    fclose(buffer);
    fmem_term(&fm);
    throw parsing_error{err};
  }
  return {AstBuilder_get_result(builder_.get()), GherkinDocument_delete};
}

}  // v1
}  // gherkin
