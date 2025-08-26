#include "Token.hpp"
#include <sstream>


Token::Token(TokenType tt, string lx, Literal lit, int32_t ln)
: type(tt), lexeme(std::move(lx)), literal(std::move(lit)), line(ln){}


string to_string(const Literal& lit){
	struct {
	string operator()(nil) const		{return "nil";}
	string operator()(bool b) const		{if (b) return "true"; else return "false";}
	string operator()(double d) const	{std::ostringstream os; os<<d; return os.str();}
	string operator()(const string& s) const{return "\"" + s + "\"";}
	} visitor;
	return std::visit(visitor, lit);
}

string to_string(TokenType tt){
	switch (tt){
	case TokenType::LEFT_PAREN   : return "LEFT_PAREN";
	case TokenType::RIGHT_PAREN  : return "RIGHT_PAREN";
	case TokenType::LEFT_BRACE   : return "LEFT_BRACE";
	case TokenType::RIGHT_BRACE  : return "RIGHT_BRACE";
	case TokenType::COMMA        : return "COMMA";
	case TokenType::DOT          : return "DOT";
	case TokenType::MINUS        : return "MINUS";
	case TokenType::PLUS         : return "PLUS";
	case TokenType::SEMICOLON    : return "SEMICOLON";
	case TokenType::SLASH        : return "SLASH";
	case TokenType::STAR         : return "STAR";
	case TokenType::BANG         : return "BANG";
	case TokenType::BANG_EQUAL   : return "BANG_EQUAL";
	case TokenType::EQUAL        : return "EQUAL";
	case TokenType::EQUAL_EQUAL  : return "EQUAL_EQUAL";
	case TokenType::GREATER      : return "GREATER";
	case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
	case TokenType::LESS         : return "LESS";
	case TokenType::LESS_EQUAL   : return "LESS_EQUAL";
	case TokenType::IDENTIFIER   : return "IDENTIFIER";
	case TokenType::STRING       : return "STRING";
	case TokenType::NUMBER       : return "NUMBER";
	case TokenType::AND          : return "AND";
	case TokenType::CLASS        : return "CLASS";
	case TokenType::ELSE         : return "ELSE";
	case TokenType::FALSE        : return "FALSE";
	case TokenType::FUN          : return "FUN";
	case TokenType::FOR          : return "FOR";
	case TokenType::IF           : return "IF";
	case TokenType::NIL          : return "NIL";
	case TokenType::OR           : return "OR";
	case TokenType::PRINT        : return "PRINT";
	case TokenType::RETURN       : return "RETURN";
	case TokenType::SUPER        : return "SUPER";
	case TokenType::THIS         : return "THIS";
	case TokenType::TRUE         : return "TRUE";
	case TokenType::VAR          : return "VAR";
	case TokenType::WHILE        : return "WHILE";
	case TokenType::_EOF         : return "_EOF";

	default: return "UNKNOWN_TOK";
	}
}

string to_string(const Token& t){
	std::ostringstream os;
	os << to_string(t.type) << " " << t.lexeme << " " << to_string(t.literal);
	return os.str();
}
