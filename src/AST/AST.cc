/**
 * Copyright (C) 2024 Yahya Al-Shamali, Kyle Prince, Charles Ancheta
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * \file AST.cc
 */
#include "AST.h"
#include <cassert>
#include <ostream>
#include <sstream>

constexpr const char *str(const fysh::ast::FyshBinary &op) {
  using fysh::ast::FyshBinary;
  switch (op) {
    // clang-format off
  case FyshBinary::Add:        return "+";
  case FyshBinary::Mul:        return "*";
  case FyshBinary::Div:        return "/";
  case FyshBinary::Equal:      return "==";
  case FyshBinary::NotEqual:   return "!=";
  case FyshBinary::GT:         return ">";
  case FyshBinary::LT:         return "<";
  case FyshBinary::GTE:        return ">=";
  case FyshBinary::LTE:        return "<=";
  case FyshBinary::BitwiseAnd: return "&";
  case FyshBinary::BitwiseOr:  return "|";
  case FyshBinary::BitwiseXor: return "^";
  case FyshBinary::ShiftLeft:  return "<<";
  case FyshBinary::ShiftRight: return ">>";
    // clang-format on
  }

  assert(false);
}

constexpr const char *fysh::ast::toStr(const fysh::ast::FyshBinary &op) {
  return str(op);
}

constexpr const char *str(const fysh::ast::FyshUnary &op) {
  using fysh::ast::FyshUnary;
  switch (op) {
  case FyshUnary::Neg:
    return "-";
  }

  assert(false);
}

constexpr const char *fysh::ast::toStr(const fysh::ast::FyshUnary &op) {
  return str(op);
}

std::ostream &fysh::ast::operator<<(std::ostream &os,
                                    const fysh::ast::FyshExpr &f) {
  return std::visit(
      [&os](auto &&arg) -> std::ostream & {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Error>) {
          os << "ERROR(\"" << *arg.t << "\")";
        } else if constexpr (std::is_same_v<T, Box<FyshBinaryExpr>>) {
          os << "(" << arg.t->left << " " << str(arg.t->op) << " "
             << arg.t->right << ")";
        } else if constexpr (std::is_same_v<T, Box<FyshUnaryExpr>>) {
          os << "(" << str(arg.t->op) << " " << arg.t->expr << ")";

        } else if constexpr (std::is_same_v<T, FyshIdentifier>) {
          os << arg.name;

        } else if constexpr (std::is_same_v<T, FyshLiteral>) {
          os << arg.num;

        } else {
          static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
        return os;
      },
      f);
}

std::ostream &fysh::ast::operator<<(std::ostream &os,
                                    const fysh::ast::FyshStmt &f) {
  return std::visit(
      [&os](auto &&arg) -> std::ostream & {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Error>)
          os << "ERROR(\"" << *arg.t << "\");";
        else if constexpr (std::is_same_v<T, FyshExpr>)
          os << arg << ";\n";
        else if constexpr (std::is_same_v<T, FyshAssignmentStmt>)
          os << arg.left << " = " << arg.right << ";\n";
        else if constexpr (std::is_same_v<T, FyshBlock>) {
          os << "{\n";
          for (const ast::FyshStmt &a : arg) {
            os << a;
          }
          os << "}\n";
        } else if constexpr (std::is_same_v<T, FyshIncrementStmt>)
          os << arg.expr << "++;\n";
        else if constexpr (std::is_same_v<T, FyshDecrementStmt>)
          os << arg.expr << "--;\n";
        else if constexpr (std::is_same_v<T, FyshLoopStmt>)
          os << "while (" << arg.condition << ")\n" << arg.body;
        else if constexpr (std::is_same_v<T, FyshIfStmt>) {
          os << "if (" << arg.condition << ")\n" << arg.consequence;
          if (arg.alternative.has_value()) {
            os << "else\n" << arg.alternative.value();
          }
        } else {
          static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
        return os;
      },
      f);
}

bool fysh::ast::operator==(const fysh::ast::FyshExpr &expr, const char *str) {
  std::stringstream ss;
  ss << expr;
  return ss.str() == str;
}

bool fysh::ast::operator!=(const fysh::ast::FyshExpr &expr, const char *str) {
  return !(expr == str);
}