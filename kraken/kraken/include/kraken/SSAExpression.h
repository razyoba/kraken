#ifndef SSAEXPRESSION_H
#define SSAEXPRESSION_H

#include "SSAExpressionArgument.h"

namespace kraken
{
  class SSAExpression
  {
    public:
      enum ExpressionType
      {
        Constant = 0,
        UnconditionalJmp,
        ConditionalJmp,
        Add,

        ET_COUNT
      };

      static const char* ExpressionTypeStrings[ET_COUNT];

      SSAExpression(ExpressionType expressionType, const SSAExpressionArgument* arg1);

      SSAExpression(const SSAExpressionArgument* arg1, ExpressionType expressionType, const SSAExpressionArgument* arg2);

      std::string to_string() const;

    private:
      SSAExpression(){}

      const SSAExpressionArgument* _operand1;
      const SSAExpressionArgument* _operand2;

      ExpressionType _type;
  };
}

#endif // SSAEXPRESSION_H
