#pragma once // like ifndef HEADER_H in c
#include <string>
#include <variant>
#include <cstdint>

using std::string;
using std::int32_t;

// enum class = enum with more type safety
enum class TokenType{
	  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  _EOF

};

// create type aliases for our implementation of nil and literal
using nil = std::monostate;
using Literal = std::variant<nil, bool, double, string>;

// STRUCT DECLARATION
struct Token{
	TokenType type{}; // {} = default value
	string lexeme;
	Literal literal{};
	int32_t line{1};

	Token() = default; // use the {} constructors 
	Token(TokenType tt, string lx, Literal lit, int32_t ln);
};

// FUNCTION DECLARATIONS
string to_string(TokenType tt);
string to_string(const Literal& lit);
string to_string(const Token& t);
