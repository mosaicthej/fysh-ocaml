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
 * \file Program.h
 */
#ifndef FYSH_PROGRAM_H_
#define FYSH_PROGRAM_H_

#include <llvm/IR/Function.h>
#include <string>
#include <vector>

namespace fysh {
class Program : std::vector<llvm::Function *> {
public:
  void print(const std::string &outputFile);
  void add(llvm::Function *fn);
  bool empty() const;

private:
  using std::vector<llvm::Function *>::vector;
};
}; // namespace fysh

#endif // !FYSH_PROGRAM_H_