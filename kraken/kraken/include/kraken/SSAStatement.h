#ifndef SSASTATEMENT_H
#define SSASTATEMENT_H

#include <kraken/kraken.h>

#include "SSADefinition.h"
#include "SSAExpression.h"

#include <memory>

namespace kraken
{
  class SSAStatement
  {
    public:
      enum StatementType
      {
        Unknown,
        Assignment,
        Call,
        JmpType,
        Return,
        EndlessLoop
      };

      SSAStatement(std::shared_ptr<SSADefinition> definition, StatementType statementType, SSAExpression expression);

      std::string to_string();

    private:
      std::shared_ptr<SSADefinition> _definition;

      SSAExpression _expression;

      StatementType _operationType;
  };
}

#endif // SSASTATEMENT_H
