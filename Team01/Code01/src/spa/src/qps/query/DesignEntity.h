#pragma once

#include <string>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "common/Utils.h"
#include "qps/pkb_integration/StorageReader.h"
#include "qps/exception/QueryProcessorException.h"

class DesignEntity {
protected:
    std::string value;

public:
    explicit DesignEntity(const std::string& value);

    virtual bool checkSyntax() const = 0;
    virtual bool matches(const std::string& literal, std::shared_ptr<EntityType> type) const = 0;

    std::string getValue() const;
};

// Synonym Class
class Synonym : public virtual DesignEntity {
protected:
    std::unordered_set<EntityType> allowedTypes;

    virtual void setAllowedTypes();

public:
    explicit Synonym(const std::string& value);

    bool checkSyntax() const override;
    bool matches(const std::string &literal, std::shared_ptr<EntityType> type) const override;

    virtual std::string getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader);

    bool compareName(std::shared_ptr<Synonym> s);
    std::unordered_set<EntityType> getAllowedTypes();
    std::size_t hash() const;
};

class Literal : public virtual DesignEntity {
public:
    explicit Literal(const std::string& value);

    virtual bool checkSyntax() const override = 0;
    bool matches(const std::string &literal, std::shared_ptr<EntityType> type) const override;
};

// Statement Classes
class StatementRef : public virtual DesignEntity {
public:
    explicit StatementRef(const std::string& value);
};

class StatementLiteral : public StatementRef, public Literal {
public:
    explicit StatementLiteral(const std::string& value);

    bool checkSyntax() const override;
    using Literal::matches;
};

class StatementSynonym : public StatementRef, public Synonym {
protected:
    void setAllowedTypes() override;
public:
    explicit StatementSynonym(const std::string& value);
};

class ReadSynonym : public StatementSynonym {
protected:
    void setAllowedTypes() override;
public:
    explicit ReadSynonym(const std::string& value);
    std::string getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader) override;
};

class PrintSynonym : public StatementSynonym {
protected:
    void setAllowedTypes() override;
public:
    explicit PrintSynonym(const std::string& value);
    std::string getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader) override;
};

class AssignSynonym : public StatementSynonym {
protected:
    void setAllowedTypes() override;
public:
    explicit AssignSynonym(const std::string& value);
};

class CallSynonym : public StatementSynonym {
protected:
    void setAllowedTypes() override;
public:
    explicit CallSynonym(const std::string& value);
    std::string getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader) override;
};

class WhileSynonym : public StatementSynonym {
protected:
    void setAllowedTypes() override;
public:
    explicit WhileSynonym(const std::string& value);
};

class IfSynonym : public StatementSynonym {
protected:
    void setAllowedTypes() override;
public:
    explicit IfSynonym(const std::string& value);
};

// Entity Classes
class EntityRef : public virtual DesignEntity {
public:
    explicit EntityRef(const std::string& value);
};

class EntityLiteral : public EntityRef, public Literal {
public:
    explicit EntityLiteral(const std::string& value);

    bool checkSyntax() const override;
    using Literal::matches;
};

class VariableLiteral : public EntityLiteral {
public:
    explicit VariableLiteral(const std::string& value);

    bool checkSyntax() const override;
    using Literal::matches;
};

class ConstantLiteral : public Literal {
public:
    explicit ConstantLiteral(const std::string& value);

    bool checkSyntax() const override;
    using Literal::matches;
};

class ProcedureLiteral : public EntityLiteral {
public:
    explicit ProcedureLiteral(const std::string& value);

    bool checkSyntax() const override;
    using Literal::matches;
};

class EntitySynonym : public EntityRef, public Synonym {
protected:
    void setAllowedTypes() override;

public:
    explicit EntitySynonym(const std::string& value);
};

class ProcedureSynonym : public EntitySynonym {
protected:
    void setAllowedTypes() override;

public:
    explicit ProcedureSynonym(const std::string& value);
};

class VariableSynonym : public EntitySynonym {
protected:
    void setAllowedTypes() override;

public:
    explicit VariableSynonym(const std::string& value);
};

class ConstantSynonym : public EntitySynonym {
protected:
    void setAllowedTypes() override;

public:
    explicit ConstantSynonym(const std::string& value);
};

// Expressions
class Expression : public virtual DesignEntity {
public:
    explicit Expression(const std::string& value);
};

class ExactExpression : public Expression {
public:
    explicit ExactExpression(const std::string& value);

    bool checkSyntax() const override;
    bool matches(const std::string &literal, std::shared_ptr<EntityType> type) const override;
};

class PartialExpression : public Expression {
public:
    explicit PartialExpression(const std::string& value);

    bool checkSyntax() const override;
    bool matches(const std::string &literal, std::shared_ptr<EntityType> type) const override;
};

class Wildcard : public StatementRef, public EntityRef, public Expression {
public:
    Wildcard();

    bool checkSyntax() const override;
    bool matches(const std::string &literal, std::shared_ptr<EntityType> type) const override;
};
