#include "../QueryClause.h"

#include <regex>

DeclareClause::DeclareClause(std::shared_ptr<Synonym> syn) : syn(syn) {}

std::shared_ptr<Synonym> DeclareClause::getSynonym() {
    return syn;
}
