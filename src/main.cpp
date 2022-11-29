#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#define TYPE_KW			"keyword"
#define KW_CLASS 		"class"
#define KW_CONSTRUCTOR 	"constructor"
#define KW_FUNCTION 	"function"
#define KW_METHOD 		"method"
#define KW_FIELD 		"field"
#define KW_STATIC 		"static"
#define KW_VAR 			"var"
#define KW_INT 			"int"
#define KW_CHAR 		"char"
#define KW_BOOL 		"boolean"
#define KW_VOID 		"void"
#define KW_TRUE 		"true"
#define KW_FALSE	 	"false"
#define KW_NULL 		"null"
#define KW_THIS 		"this"
#define KW_LET 			"let"
#define KW_DO 			"do"
#define KW_IF 			"if"
#define KW_ELSE 		"else"
#define KW_WHILE 		"while"
#define KW_RETURN 		"return"

#define TYPE_SYM		"symbol"
#define SYM_CBF			"{"
#define SYM_CBB			"}"
#define SYM_PAF			"("
#define SYM_PAB			")"
#define SYM_SBF			"["
#define SYM_SBB			"]"
#define SYM_PER			"."
#define SYM_COM			","
#define SYM_SMC			";"
#define SYM_ADD			"+"
#define SYM_MIN			"-"
#define SYM_MUL			"*"
#define SYM_DIV			"/"
#define SYM_AND			"&"
#define SYM_OR			"|"
#define SYM_LT			"<"
#define SYM_GT			">"
#define SYM_EQ			"="
#define SYM_TIL			"~"

#define TYPE_SC			"string_constant"
#define TYPE_IC			"integer_constant"
#define TYPE_ID			"identifier"
#define INVALID_TYPE	"INVALID_TYPE"

#define COM_SYM         "//"
#define COMMENT         "COMMENT"

// check if letter
bool is_letter(char c) {
	return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}

// check if digit
bool is_digit(char c) {
	return ('0' <= c && c <= '9');
}

// given a token, returns whether or not the token is a keyword
bool is_keyword(string token) {
	if (token == KW_CLASS) return true;
	if (token == KW_CONSTRUCTOR) return true;
	if (token == KW_FUNCTION) return true;
	if (token == KW_METHOD) return true;
	if (token == KW_FIELD) return true;
	if (token == KW_STATIC) return true;
	if (token == KW_VAR) return true;
	if (token == KW_INT) return true;
	if (token == KW_CHAR) return true;
	if (token == KW_BOOL) return true;
	if (token == KW_VOID) return true;
	if (token == KW_TRUE) return true;
	if (token == KW_FALSE) return true;
	if (token == KW_NULL) return true;
	if (token == KW_THIS) return true;
	if (token == KW_LET) return true;
	if (token == KW_DO) return true;
	if (token == KW_IF) return true;
	if (token == KW_ELSE) return true;
	if (token == KW_WHILE) return true;
	if (token == KW_RETURN) return true;
	return false;
}

// given a token, returns whether or not the token is a symbol
bool is_symbol(string token) {
	if (token == SYM_OR) return true;
	if (token == SYM_AND) return true;
	if (token == SYM_EQ) return true;
	if (token == SYM_TIL) return true;
	if (token == SYM_GT) return true;
	if (token == SYM_LT) return true;
	if (token == SYM_CBB) return true;
	if (token == SYM_CBF) return true;
	if (token == SYM_PAB) return true;
	if (token == SYM_PAF) return true;
	if (token == SYM_SBB) return true;
	if (token == SYM_SBF) return true;
	if (token == SYM_DIV) return true;
	if (token == SYM_MUL) return true;
	if (token == SYM_MIN) return true;
	if (token == SYM_ADD) return true;
	if (token == SYM_PER) return true;
	if (token == SYM_COM) return true;
	if (token == SYM_SMC) return true;
	return false;
}

// check if integral
bool is_integer(string token) {
	//minus sign is considered symbol
	for (auto iter = token.begin(); iter < token.end(); ++iter) {
		if (!is_digit(*iter)) return false;
	}
	return true;
}

// check if valid identifier
bool is_valid_id(string token) {
	//we know the entire thing will not be digits, as is_integer is called prior
	auto iter = token.begin();
	if (is_digit(*iter)) return false;
	for (; iter < token.end(); ++iter) {
		if (!(is_digit(*iter) || is_letter(*iter) || *iter == '_')) return false;
	}
	return true;
}

// check if valid string constant
bool is_valid_str(string token) {
	//beginning and end must be " or '
	if (token.size() < 2) return false;
	if (token.front() != '\"' && token.front() != '\'') return false;
	if (token.back() != '\"' && token.back() != '\'') return false;
}

// check if valid integer constant
bool is_valid_integer(string token) {
	if (!is_integer(token)) return false;
	int tmp = stoi(token);
	return (0 <= tmp && tmp <= 32767);
}

// check if comment
bool is_comment(string token) {



}

string string_val(string token) {
    return token.substr(1, token.size() - 2);
}

// given a token, returns the token type
string token_type(string token) {
	if (token.size() == 0) return INVALID_TYPE;
    if (is_comment(token)) return COMMENT;
	if (is_keyword(token)) return TYPE_KW;
	if (is_symbol(token)) return TYPE_SYM;
	if (is_valid_integer(token)) return TYPE_IC;
	if (is_valid_str(token)) return TYPE_SC;
	if (is_valid_id(token)) return TYPE_ID;
	return INVALID_TYPE;
}

struct Token {
    string token;
    string type;

    Token(string token, string type) {
        this->token = token;
        this->type = type;
    }
};

struct Tokenizer {

    ifstream *fin;

    //constructor
    Tokenizer(ifstream &fin) {
        this->fin = &fin;
    }

    void setFile(ifstream &fin) { this->fin = &fin; }
    
    Token tokenize() {
        string token = "";
        string type = INVALID_TYPE;
        char c = 'a';

        //read until a valid token
        while (type == INVALID_TYPE) {
            if (c == '\n') break;
            fin->get(c);
            token += c;
            type = token_type(token);
        }

        if (type == COMMENT) {
            //skip line
			while (c != '\n') fin->get(c);
        } else if (type == TYPE_IC) {
            //pass whole token
			return Token(token, type);
        } else if (type == TYPE_SC) {
            //remove qoutes
			return Token(string_val(token), type);
        } else if (type == TYPE_KW) {
            //pass whole token
			return Token(token, type);
        } else if (type == TYPE_SYM) {
            //pass whole token
			return Token(token, type);
        } else if (type == TYPE_ID) {
            //pass whole token
			return Token(token, type);
        } else {
            //pass
        }

    }

};

#define CLASS_O		"<class>"
#define CLASS_C		"</class>"



struct Engine {

	ofstream *fout;
	Tokenizer tokenizer;
	string prepend = "";
	Token token;

	Engine(ofstream &fout, ifstream &fin) {
		this->fout = &fout;
		tokenizer = Tokenizer(fin);
	}

	void indent() { prepend += '\t'; }
	void undent() { prepend += '\t'; }

	void set_ifile(ofstream &fout) { this->fout = &fout; }
	void set_ofile(ifstream &fin) { tokenizer = Tokenizer(fin); }
	
	void compile_class() {
		fout << prepend << CLASS_O << endl;
		indent();
		
		undent();
		fout << prepend << CLASS_C << endl;
	}

	void compile_class_var() {

	}

	void compile_subr() {

	}

	void compile_param_list() {
		
	}

	void compile_subr_body() {
		
	}

	void compile_var_dec() {
		
	}

	void compile_statements() {

	}

	void compile_let_dec() {
		
	}

	void compile_if() {
		
	}

	void compile_while() {
		
	}

	void compile_do() {

	}

	void compile_return() {
		
	}

	void compile_exp() {
		
	}

	void compile_term() {
		
	}

	int compile_exp_list() {
		
	}

};

int main(int argc, char *argv[]) {

	

}