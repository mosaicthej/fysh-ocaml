#include "../src/Lexer.h"
#include "../src/Parser.h"

#include "doctest.h"
#include <cstdint>
#include <variant>

using namespace fysh::ast;

template <typename T> T get_stmt(FyshStmt stmt) {
  REQUIRE(std::holds_alternative<T>(stmt));
  return std::get<T>(stmt);
}

template <typename T> T get_expr(FyshStmt stmt) {
  auto expr{get_stmt<FyshExpr>(stmt)};
  REQUIRE(std::holds_alternative<T>(expr));
  return std::get<T>(expr);
}

template <typename T> T get_expr(FyshExpr expr) {
  REQUIRE(std::holds_alternative<T>(expr));
  return std::get<T>(expr);
}

template <typename T> void check_ident(T expr, const char *name) {
  auto ident{get_expr<FyshIdentifier>(expr)};
  CHECK(ident.name == name);
}

template <typename T> void check_num(T expr, std::uint64_t value) {
  auto literal{get_expr<FyshLiteral>(expr)};
  CHECK(literal.num == value);
}

TEST_CASE("Assignment Statement") {
  fysh::FyshParser p{fysh::FyshLexer{"><fysh> = ><(({o> ~"}};
  std::vector<FyshStmt> program{p.parseProgram()};
  REQUIRE(program.size() == 1);
  auto stmt{get_stmt<FyshAssignmentStmt>(program[0])};
  check_ident(stmt.left, "fysh");
  check_num(stmt.right, 1);
}

TEST_CASE("Expression Statements") {
  fysh::FyshParser p{fysh::FyshLexer{R"(
><(({o> ~
><fysh> ~
><fysh> <3 ><{({o> ~
)"}};
  std::vector<FyshStmt> program{p.parseProgram()};
  REQUIRE(program.size() == 3);

  check_num(program[0], 1);
  check_ident(program[1], "fysh");

  auto binaryExpr{get_expr<Box<FyshBinaryExpr>>(program[2])};
  check_ident(binaryExpr.t->left, "fysh");
  check_num(binaryExpr.t->right, 5);
  CHECK(binaryExpr.t->op == FyshBinary::Mul);
}

TEST_CASE("Increment Statement") {
  fysh::FyshParser p{fysh::FyshLexer{">><fysh> ~"}};
  std::vector<FyshStmt> program{p.parseProgram()};
  REQUIRE(program.size() == 1);
  auto stmt{get_stmt<FyshIncrementStmt>(program[0])};
  check_ident(stmt.expr, "fysh");
}

TEST_CASE("Decrement Statement") {
  fysh::FyshParser p{fysh::FyshLexer{"<fysh><< ~"}};
  std::vector<FyshStmt> program{p.parseProgram()};
  REQUIRE(program.size() == 1);
  auto stmt{get_stmt<FyshDecrementStmt>(program[0])};
  check_ident(stmt.expr, "fysh");
}
