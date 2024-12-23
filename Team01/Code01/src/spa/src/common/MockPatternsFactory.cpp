#include "MockPatternsFactory.h"

MockPatternsFactory::MockPatternsFactory() {}

const std::shared_ptr<ExprNode> MockPatternsFactory::createFirstExpression() const {
    // v + x * y + z * t
    auto varX = std::make_shared<VariableNode>("x", 1);
    auto varV = std::make_shared<VariableNode>("v", 1);
    auto varY = std::make_shared<VariableNode>("y", 1);
    auto varZ = std::make_shared<VariableNode>("z", 1);
    auto varT = std::make_shared<VariableNode>("t", 1);
    auto multiplyXY = std::make_shared<ExprNode>(varX, varY, ExprOperator::Times, 1);
    auto multiplyZT = std::make_shared<ExprNode>(varZ, varT, ExprOperator::Times, 1);
    auto addVXY = std::make_shared<ExprNode>(varV, multiplyXY, ExprOperator::Plus, 1);
    auto firstExpression = std::make_shared<ExprNode>(addVXY, multiplyZT, ExprOperator::Plus, 1);
    auto stringRepresentation = firstExpression->generateExpressionString();
    return firstExpression;
};

const std::shared_ptr<ExprNode> MockPatternsFactory::createSecondExpression() const {
    // (b - c) / (d % e)
    auto varB = std::make_shared<VariableNode>("b", 2);
    auto varC = std::make_shared<VariableNode>("c", 2);
    auto varD = std::make_shared<VariableNode>("d", 2);
    auto varE = std::make_shared<VariableNode>("e", 2);
    auto subtractBC = std::make_shared<ExprNode>(varB, varC, ExprOperator::Minus, 2);
    auto modulusDE = std::make_shared<ExprNode>(varD, varE, ExprOperator::Modulo, 2);
    auto secondExpression = std::make_shared<ExprNode>(subtractBC, modulusDE, ExprOperator::Divided, 2);
    return secondExpression;
};

const std::shared_ptr<ExprNode> MockPatternsFactory::createThirdExpression() const {
    // (g * (h + i)) - j
    auto varG = std::make_shared<VariableNode>("g", 3);
    auto varH = std::make_shared<VariableNode>("h", 3);
    auto varI = std::make_shared<VariableNode>("i", 3);
    auto varJ = std::make_shared<VariableNode>("j", 3);
    auto addHI = std::make_shared<ExprNode>(varH, varI, ExprOperator::Plus, 3);
    auto multiplyGHI = std::make_shared<ExprNode>(varG, addHI, ExprOperator::Times, 3);
    auto thirdExpression = std::make_shared<ExprNode>(multiplyGHI, varJ, ExprOperator::Minus, 3);
    return thirdExpression;
};

const std::shared_ptr<ExprNode> MockPatternsFactory::createFourthExpression() const {
    // ((g + i) * h) + j
    auto varG = std::make_shared<VariableNode>("g", 4);
    auto varI = std::make_shared<VariableNode>("i", 4);
    auto varH = std::make_shared<VariableNode>("h", 4);
    auto varJ = std::make_shared<VariableNode>("j", 4);
    auto addMO = std::make_shared<ExprNode>(varG, varI, ExprOperator::Plus, 4);
    auto multiplyMOP = std::make_shared<ExprNode>(addMO, varH, ExprOperator::Times, 4);
    auto fourthExpression = std::make_shared<ExprNode>(multiplyMOP, varJ, ExprOperator::Plus, 4);
    return fourthExpression;
};

const std::shared_ptr<ExprNode> MockPatternsFactory::createFifthExpression() const {
    // a*b - c*d - v*x + t*z
    auto varA = std::make_shared<VariableNode>("a", 5);
    auto varB = std::make_shared<VariableNode>("b", 5);
    auto varC = std::make_shared<VariableNode>("c", 5);
    auto varD = std::make_shared<VariableNode>("d", 5);
    auto varV = std::make_shared<VariableNode>("v", 5);
    auto varX = std::make_shared<VariableNode>("x", 5);
    auto varT = std::make_shared<VariableNode>("t", 5);
    auto varZ = std::make_shared<VariableNode>("z", 5);

    auto multiplyAB = std::make_shared<ExprNode>(varA, varB, ExprOperator::Times, 5);
    auto multiplyCD = std::make_shared<ExprNode>(varC, varD, ExprOperator::Times, 5);
    auto multiplyVX = std::make_shared<ExprNode>(varV, varX, ExprOperator::Times, 5);
    auto multiplyTZ = std::make_shared<ExprNode>(varT, varZ, ExprOperator::Times, 5);

    auto subtractABCD = std::make_shared<ExprNode>(multiplyAB, multiplyCD, ExprOperator::Minus, 5);
    auto subtractABCDVX = std::make_shared<ExprNode>(subtractABCD, multiplyVX, ExprOperator::Minus, 5);
    auto fifthExpression = std::make_shared<ExprNode>(subtractABCDVX, multiplyTZ, ExprOperator::Plus, 5);
    return fifthExpression;
};

void MockPatternsFactory::populatePatternsMap() {
    patternsData.insert({{"1", EntityType::ASSIGN}, createFirstExpression()});
    patternsData.insert({{"2", EntityType::ASSIGN}, createSecondExpression()});
    patternsData.insert({{"3", EntityType::ASSIGN}, createThirdExpression()});
    patternsData.insert({{"4", EntityType::ASSIGN}, createFourthExpression()});
    patternsData.insert({{"5", EntityType::ASSIGN}, createFifthExpression()});
};

const std::map<std::pair<std::string, EntityType>, std::shared_ptr<ExprNode>>& MockPatternsFactory::getPatternsData() const {
    return patternsData;
}
