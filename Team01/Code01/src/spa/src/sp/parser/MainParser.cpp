#include "MainParser.h"

MainParser::MainParser(std::vector<Token>& tokens) : ctx(tokens) {
    ParserFactory::registerParsers();
}

std::shared_ptr<ProgramNode> MainParser::parse() {
    auto primer = ProgramParser();
    std::shared_ptr<ProgramNode> root = primer.parse(ctx);
    return root;
}