#include <cassert>
#include <fstream>
#include <gherkin.hpp>
#include <iostream>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "You need to supply a feature file like: ./gherkin-cpp-test testdata/good/escaped_pipes.feature.out" << std::endl;
    std::cerr << "Or better, run the 'make gherkin-cpp-test.run" << std::endl;
    exit(1);
  }
  std::ifstream file{argv[1]};
  assert(file.good());
  std::wstring content{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

  try {
    gherkin::parser parser{L"en"};
    gherkin::compiler compiler{argv[1]};
    const auto gherkin_document = parser.parse(content);
    const auto pickles = compiler.compile(gherkin_document);
    const auto ast = compiler.ast(gherkin_document);

    std::cout << ast << "\n\n";

    for (const auto& pickle : pickles) {
      std::cout << pickle << '\n';
    }
  } catch (const gherkin::parsing_error& err) {
    std::cout << err.what() << '\n';
  }
}
