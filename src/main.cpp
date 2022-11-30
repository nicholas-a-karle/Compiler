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

#define SYM_ALT_LT		"&lt;"
#define SYM_ALT_GT		"&gt;"
#define SYM_ALT_AND		"&amp;"

#define TYPE_SC			"stringConstant"
#define TYPE_IC			"integerConstant"
#define TYPE_ID			"identifier"
#define INVALID_TYPE	"INVALID_TYPE"

#define COM_SYM         "//"
#define M_COM_SYM		"/*"
#define COMMENT         "COMMENT"
#define MULTI_COMM		"MULTILINE COMMENT"
#define COMM_DIV		"TBD DIV OR COMM"

#define THROW_TOKEN(x)		do { cout << x << endl; throw std::invalid_argument(x); } while(0)

#define END_TOKEN_BUILD	Token("END", "END")
#define END				"END"

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
	if (token.size() != 1) return false;
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
	if (token == SYM_DIV) return true;
	if (token == SYM_SBF) return true;
	if (token == SYM_MUL) return true;
	if (token == SYM_MIN) return true;
	if (token == SYM_ADD) return true;
	if (token == SYM_PER) return true;
	if (token == SYM_COM) return true;
	if (token == SYM_SMC) return true;
	return false;
}

bool is_symbol(char token) {
	if (token == SYM_OR[0]) return true;
	if (token == SYM_AND[0]) return true;
	if (token == SYM_EQ[0]) return true;
	if (token == SYM_TIL[0]) return true;
	if (token == SYM_GT[0]) return true;
	if (token == SYM_LT[0]) return true;
	if (token == SYM_CBB[0]) return true;
	if (token == SYM_CBF[0]) return true;
	if (token == SYM_PAB[0]) return true;
	if (token == SYM_DIV[0]) return true;
	if (token == SYM_PAF[0]) return true;
	if (token == SYM_SBB[0]) return true;
	if (token == SYM_SBF[0]) return true;
	if (token == SYM_MUL[0]) return true;
	if (token == SYM_MIN[0]) return true;
	if (token == SYM_ADD[0]) return true;
	if (token == SYM_PER[0]) return true;
	if (token == SYM_COM[0]) return true;
	if (token == SYM_SMC[0]) return true;
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
	return true;
}

// check if valid integer constant
bool is_valid_integer(string token) {
	if (!is_integer(token)) return false;
	int tmp = stoi(token);
	return (0 <= tmp && tmp <= 32767);
}

// check if comment
bool is_comment(string token) { return token == COM_SYM; }
bool is_multicomment(string token) { return token == M_COM_SYM; }

string string_val(string token) {
    return token.substr(1, token.size() - 2);
}

// given a token, returns the token type
string token_type(string token) {
	if (token.size() == 0) return INVALID_TYPE;
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

	Token() {
		token =""; type = "";
	}

    Token(string token, string type) {
        this->token = token;
        this->type = type;
    }
};

struct Tokenizer {

    ifstream *fin;

	//constructor
    Tokenizer() {
        //fin remains undefined
    }

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
        while (true) {
			if(!fin->get(c)) return END_TOKEN_BUILD;

			if (c == '\n' || c == ' ' || c == '\t') break;
			if (c == '/') {
				c = fin->peek();
				if (c == SYM_DIV[0]) {
					//continue till you fine \n
					while (true) {
						if(!fin->get(c)) return END_TOKEN_BUILD;
						if (c == '\n') return tokenize();
					}
				} else if (c == '*') {
					//continue till you find */
					char prev = c;
					while (true) {
						if(!fin->get(c)) return END_TOKEN_BUILD;
						if (prev == '*' && c == '/') return tokenize();
						prev = c;
					}
				} else {
					token += '/';
					return Token(token, TYPE_SYM);
				}
			}
			if (c == '\"') {
				//get string constant, go until you hit " not after escape
				bool escaped = false;
				while (fin->get(c)) {
					if (escaped) {
						if (c == '\\') token += c;
						if (c == '\"') token += c;
						else THROW_TOKEN("ESCAPE CHARACTER FOR NO REASON");
						escaped = false;
					} else {
						if (c == '\\') escaped = true;
						else if (c == '\"') break;
						else token += c;
					}
				}
				return Token(token, TYPE_SC);
			}
            token += c;
			if (is_symbol((char)fin->peek())) break;
			if (is_symbol(token)) break;
        }
		type = token_type(token);

		//cout << token << endl;

        if (type == TYPE_IC) {
            //pass whole token
			return Token(token, type);
        } else if (type == TYPE_KW) {
            //pass whole token
			return Token(token, type);
        } else if (type == TYPE_SYM) {
            //pass whole token
			if (token == SYM_GT) return Token(SYM_ALT_GT, type);
			if (token == SYM_LT) return Token(SYM_ALT_LT, type);
			if (token == SYM_AND) return Token(SYM_ALT_AND, type);
			return Token(token, type);
        } else if (type == TYPE_ID) {
            //pass whole token
			return Token(token, type);
        } else {
            //pass
			return tokenize();
        }
    }

};

#define O1			"<"
#define O2			"</"
#define C0			">"
#define CLASS		"class"

#define SUBR_DEC	"subroutineDec"
#define SUBR_BODY	"subroutineBody"

#define PAR_LIST	"parameterList"
#define STATEMENTS	"statements"

#define WHILE_STAT	"whileStatement"
#define IF_STAT		"ifStatement"

#define LET_STAT	"letStatement"
#define DO_STAT		"doStatement"
#define RET_STAT	"returnStatement"
#define VAR_DEC		"varDec"
#define C_VAR_DEC	"classVarDec"

#define EXPRESSION	"expression"
#define EXP_LIST	"expressionList"
#define TERM		"term"



struct AnalyzerEngine {

	ofstream *fout;
	Tokenizer tokenizer;
	string prepend;
	Token token;
	
	AnalyzerEngine(ofstream &fout, ifstream &fin) {
		this->fout = &fout;
		tokenizer = Tokenizer(fin);
		prepend = "";
		token = Token("", "");
	}

	void compile() { compile_class(); }

	void indent() { prepend += '\t'; }
	void undent() { if (prepend.size() > 0) prepend = prepend.substr(1); }
	void advance() { 
		token = tokenizer.tokenize(); 
		cout << token.token << endl; }

	void set_ifile(ofstream &fout) { this->fout = &fout; }
	void set_ofile(ifstream &fin) { tokenizer = Tokenizer(fin); }

	void to(string tag) { (*fout) << (prepend + O1 + tag + C0) << endl; indent(); }
	void tc(string tag) { undent(); (*fout) << (prepend + O2 + tag + C0) << endl; }
	void t(string tag, string contents) { (*fout) << (prepend + O1 + tag + C0 + contents + O2 + tag + C0) << endl; }
	void t() { t(token.type, token.token); }
	void tna() { t(); advance(); }

	void compile_class() {
		to(CLASS);
		advance();
		if (token.token != KW_CLASS) THROW_TOKEN("TOKEN SHOULD BE \"class\"");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();
		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		t();

		//series like this: class class_id { cvars subrs }

		//while loop
		while (token.token != END) {
			advance();
			if (token.token == KW_FUNCTION || token.token == KW_METHOD || token.token == KW_CONSTRUCTOR) {
				compile_subr();
			} else if (token.token == KW_FIELD || token.token == KW_STATIC) {
				compile_cvar_dec();
			} else if (token.token == SYM_CBB) {
				tna();
			} else {
				THROW_TOKEN("TOKEN SHOULD BE \"}\", CLASS VARIABLE, OR SUBROUTINE TYPE");
			}
		}

 		tc(CLASS);
	}
	void compile_cvar_dec() {
		to(C_VAR_DEC);

		//series like this: static_or_field data_type var_id ... , var_id, var_id; 

		if (token.token != KW_STATIC && token.token != KW_FIELD) THROW_TOKEN("TOKEN SHOULD BE \"static\" OR \"field\"");
		tna();
		if (token.token != KW_BOOL && token.token != KW_VOID && token.token != KW_CHAR && token.token != KW_INT && token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();

		while (token.token == SYM_COM) {
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
		}

		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		t();

		tc(C_VAR_DEC);
	}
	void compile_subr() {
		to(SUBR_DEC);
		//series like subr_type return_type subr_id (param_list) { subr_body }

		if (token.token != KW_FUNCTION && token.token != KW_METHOD && token.token != KW_CONSTRUCTOR) THROW_TOKEN("TOKEN SHOULD BE SUBROUTINE TYPE");
		tna();
		if (token.token != KW_BOOL && token.token != KW_VOID && token.token != KW_CHAR && token.token != KW_INT && token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();
		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_param_list();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();

		compile_subr_body();
		tc(SUBR_DEC);
	}
	void compile_param_list() {
		to(PAR_LIST);

		//series like: KW ID, KW ID, ... KW ID, KW ID
		while(true) {
			if (token.token == SYM_PAB) break;
			if (token.token != KW_BOOL && token.token != KW_VOID && token.token != KW_CHAR && token.token != KW_INT) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
			if (token.token != SYM_COM) break;
			tna();
		}
		tc(PAR_LIST);
	}
	void compile_subr_body() {
		to(SUBR_BODY);
		//series like: { statements }

		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		t();
		advance();

		while (token.token == KW_VAR) compile_var_dec();

		compile_statements();

		if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
		t();

		tc(SUBR_BODY);
	}
	void compile_var_dec() {
		to(VAR_DEC);

		//series like: var type_id_or_prim_data_type var_id;
		if (token.token != KW_VAR) THROW_TOKEN("TOKEN SHOULD BE \"var\"");
		tna();
		if (token.type != TYPE_ID && token.token != KW_BOOL && token.token != KW_CHAR && token.token != KW_INT) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();

		while (token.token == SYM_COM) {
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
		}
		
		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		tna();

		tc(VAR_DEC);
	}
	void compile_statements() {
		to(STATEMENTS);
		
		while (token.token != SYM_CBB) {
			if (token.token == KW_DO) {
				compile_do();
			} else if (token.token == KW_WHILE) {
				compile_while();
			} else if (token.token == KW_IF) {
				compile_if();
			} else if (token.token == KW_LET) {
				compile_let();
			} else if (token.token == KW_RETURN) {
				compile_return();
			} else THROW_TOKEN("TOKEN SHOULD BE A STATEMENT");
		}

		tc(STATEMENTS);
	}
	void compile_let() {
		to(LET_STAT);

		//series: let var_id = expression
		if (token.token != KW_LET) THROW_TOKEN("TOKEN SHOULD BE \"let\"");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();
		if (token.token != SYM_EQ) {
			//sqr brackets, array
			if (token.token != SYM_SBF) THROW_TOKEN("TOKEN SHOULD BE \"[\"");
			tna();
			compile_expression();
			if (token.token != SYM_SBB) THROW_TOKEN("TOKEN SHOULD BE \"]\"");
			tna();
		} //else do nothing
		if (token.token != SYM_EQ) THROW_TOKEN("TOKEN SHOULD BE \"=\"");
		tna();

		compile_expression();
		
		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		tna();

		tc(LET_STAT);
	}
	void compile_if() {
		to(IF_STAT);

		//if (expression) { statements }
		if (token.token != KW_IF) THROW_TOKEN("TOKEN SHOULD BE \"if\"");
		tna();
		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_expression();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();
		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		tna();
		compile_statements();
		if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
		tna();

		//check for trailing else statement
		if (token.token == KW_ELSE) {
			tna();
			if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
			tna();
			compile_statements();
			if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
			tna();
		}

		tc(IF_STAT);
	}
	void compile_while() {
		to(WHILE_STAT);

		//while (expression) { statements }
		if (token.token != KW_WHILE) THROW_TOKEN("TOKEN SHOULD BE \"while\"");
		tna();
		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_expression();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();
		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		tna();
		compile_statements();
		if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
		tna();

		tc(WHILE_STAT);
	}
	void compile_do() {
		to(DO_STAT);

		//series like: do id.id() or do id or do id.id.id ... .id()

		if (token.token != KW_DO) THROW_TOKEN("TOKEN SHOULD BE \"do\"");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();

		while (token.token == SYM_PER) {
			if (token.token != SYM_PER) THROW_TOKEN("TOKEN SHOULD BE \".\"");
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
		}

		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_exp_list();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();
		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		tna();

		tc(DO_STAT);
	}
	void compile_return() {
		to(RET_STAT);
		if (token.token != KW_RETURN) THROW_TOKEN("TOKEN SHOULD BE \"return\"");
		tna();
		if (token.token == SYM_SMC) {
			tna();
		} else {
			compile_expression();
			if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
			tna();
		}
		tc(RET_STAT);
	}
	void compile_expression() {
		to(EXPRESSION);
		/*if (token.token == SYM_PAF) {
			tna();
			compile_expression();
			if(token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
			tna();
		}*/

		while (true) {
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
			compile_term();
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
			//tna();       
			//if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM) break;
			if (token.token == SYM_PAF) {
				tna();
				compile_expression();
				if(token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
			}
			tna();
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
		}

		tc(EXPRESSION);
	}
	void compile_term() {
		to(TERM);
		if (token.token == SYM_MIN || token.token == SYM_TIL) {
			tna();
			compile_term();
		}
		//have to determine if this term is a function call or not
		bool function_call = false;

		if (token.token == SYM_PAF || token.token == SYM_SBF) {
			tna();
			if (function_call) compile_exp_list();
			else compile_expression();
			if(token.token != SYM_PAB && token.token != SYM_SBB) THROW_TOKEN("TOKEN SHOULD BE \")\" OR \"]\"");
			tna();
		}
		while (             
			token.token != SYM_ADD && token.token != SYM_MIN && token.token != SYM_DIV && 
			token.token != SYM_OR && token.token != SYM_MUL && token.token != SYM_ALT_LT && 
			token.token != SYM_ALT_GT && token.token != SYM_ALT_AND && token.token != SYM_COM &&
			token.token != SYM_SMC && token.token != SYM_PAB && token.token != SYM_SBB && token.token != SYM_EQ
		) {
			tna();
			if (token.token == SYM_PER) function_call = true;
			if (token.token == SYM_PAF || token.token == SYM_SBF) {
				tna();
				if (function_call) compile_exp_list();
				else compile_expression();
				if(token.token != SYM_PAB && token.token != SYM_SBB) THROW_TOKEN("TOKEN SHOULD BE \")\" OR \"]\"");
				tna();
			}
		}

		tc(TERM);
	}
	void compile_exp_list() {
		to(EXP_LIST);
		//series like: exp, exp, exp
		while(true) {
			if (token.token == SYM_PAB) break;
			compile_expression();
			if (token.token != SYM_COM) break;
			tna();
		}
		tc(EXP_LIST);
	}
};

struct CompilationEngine {

	ofstream *fout;
	Tokenizer tokenizer;
	string prepend;
	Token token;
	
	CompilationEngine(ofstream &fout, ifstream &fin) {
		this->fout = &fout;
		tokenizer = Tokenizer(fin);
		prepend = "";
		token = Token("", "");
	}

	void compile() { compile_class(); }

	void indent() { prepend += '\t'; }
	void undent() { if (prepend.size() > 0) prepend = prepend.substr(1); }
	void advance() { 
		token = tokenizer.tokenize(); 
		cout << token.token << endl; }

	void set_ifile(ofstream &fout) { this->fout = &fout; }
	void set_ofile(ifstream &fin) { tokenizer = Tokenizer(fin); }

	void to(string tag) { (*fout) << (prepend + O1 + tag + C0) << endl; indent(); }
	void tc(string tag) { undent(); (*fout) << (prepend + O2 + tag + C0) << endl; }
	void t(string tag, string contents) { (*fout) << (prepend + O1 + tag + C0 + contents + O2 + tag + C0) << endl; }
	void t() { t(token.type, token.token); }
	void tna() { t(); advance(); }

	void compile_class() {
		to(CLASS);
		advance();
		if (token.token != KW_CLASS) THROW_TOKEN("TOKEN SHOULD BE \"class\"");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();
		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		t();

		//series like this: class class_id { cvars subrs }

		//while loop
		while (token.token != END) {
			advance();
			if (token.token == KW_FUNCTION || token.token == KW_METHOD || token.token == KW_CONSTRUCTOR) {
				compile_subr();
			} else if (token.token == KW_FIELD || token.token == KW_STATIC) {
				compile_cvar_dec();
			} else if (token.token == SYM_CBB) {
				tna();
			} else {
				THROW_TOKEN("TOKEN SHOULD BE \"}\", CLASS VARIABLE, OR SUBROUTINE TYPE");
			}
		}

 		tc(CLASS);
	}
	void compile_cvar_dec() {
		to(C_VAR_DEC);

		//series like this: static_or_field data_type var_id ... , var_id, var_id; 

		if (token.token != KW_STATIC && token.token != KW_FIELD) THROW_TOKEN("TOKEN SHOULD BE \"static\" OR \"field\"");
		tna();
		if (token.token != KW_BOOL && token.token != KW_VOID && token.token != KW_CHAR && token.token != KW_INT && token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();

		while (token.token == SYM_COM) {
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
		}

		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		t();

		tc(C_VAR_DEC);
	}
	void compile_subr() {
		to(SUBR_DEC);
		//series like subr_type return_type subr_id (param_list) { subr_body }

		if (token.token != KW_FUNCTION && token.token != KW_METHOD && token.token != KW_CONSTRUCTOR) THROW_TOKEN("TOKEN SHOULD BE SUBROUTINE TYPE");
		tna();
		if (token.token != KW_BOOL && token.token != KW_VOID && token.token != KW_CHAR && token.token != KW_INT && token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();
		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_param_list();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();

		compile_subr_body();
		tc(SUBR_DEC);
	}
	void compile_param_list() {
		to(PAR_LIST);

		//series like: KW ID, KW ID, ... KW ID, KW ID
		while(true) {
			if (token.token == SYM_PAB) break;
			if (token.token != KW_BOOL && token.token != KW_VOID && token.token != KW_CHAR && token.token != KW_INT) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
			if (token.token != SYM_COM) break;
			tna();
		}
		tc(PAR_LIST);
	}
	void compile_subr_body() {
		to(SUBR_BODY);
		//series like: { statements }

		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		t();
		advance();

		while (token.token == KW_VAR) compile_var_dec();

		compile_statements();

		if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
		t();

		tc(SUBR_BODY);
	}
	void compile_var_dec() {
		to(VAR_DEC);

		//series like: var type_id_or_prim_data_type var_id;
		if (token.token != KW_VAR) THROW_TOKEN("TOKEN SHOULD BE \"var\"");
		tna();
		if (token.type != TYPE_ID && token.token != KW_BOOL && token.token != KW_CHAR && token.token != KW_INT) THROW_TOKEN("TOKEN SHOULD BE DATA TYPE");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();

		while (token.token == SYM_COM) {
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
		}
		
		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		tna();

		tc(VAR_DEC);
	}
	void compile_statements() {
		to(STATEMENTS);
		
		while (token.token != SYM_CBB) {
			if (token.token == KW_DO) {
				compile_do();
			} else if (token.token == KW_WHILE) {
				compile_while();
			} else if (token.token == KW_IF) {
				compile_if();
			} else if (token.token == KW_LET) {
				compile_let();
			} else if (token.token == KW_RETURN) {
				compile_return();
			} else THROW_TOKEN("TOKEN SHOULD BE A STATEMENT");
		}

		tc(STATEMENTS);
	}
	void compile_let() {
		to(LET_STAT);

		//series: let var_id = expression
		if (token.token != KW_LET) THROW_TOKEN("TOKEN SHOULD BE \"let\"");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();
		if (token.token != SYM_EQ) {
			//sqr brackets, array
			if (token.token != SYM_SBF) THROW_TOKEN("TOKEN SHOULD BE \"[\"");
			tna();
			compile_expression();
			if (token.token != SYM_SBB) THROW_TOKEN("TOKEN SHOULD BE \"]\"");
			tna();
		} //else do nothing
		if (token.token != SYM_EQ) THROW_TOKEN("TOKEN SHOULD BE \"=\"");
		tna();

		compile_expression();
		
		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		tna();

		tc(LET_STAT);
	}
	void compile_if() {
		to(IF_STAT);

		//if (expression) { statements }
		if (token.token != KW_IF) THROW_TOKEN("TOKEN SHOULD BE \"if\"");
		tna();
		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_expression();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();
		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		tna();
		compile_statements();
		if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
		tna();

		//check for trailing else statement
		if (token.token == KW_ELSE) {
			tna();
			if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
			tna();
			compile_statements();
			if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
			tna();
		}

		tc(IF_STAT);
	}
	void compile_while() {
		to(WHILE_STAT);

		//while (expression) { statements }
		if (token.token != KW_WHILE) THROW_TOKEN("TOKEN SHOULD BE \"while\"");
		tna();
		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_expression();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();
		if (token.token != SYM_CBF) THROW_TOKEN("TOKEN SHOULD BE \"{\"");
		tna();
		compile_statements();
		if (token.token != SYM_CBB) THROW_TOKEN("TOKEN SHOULD BE \"}\"");
		tna();

		tc(WHILE_STAT);
	}
	void compile_do() {
		to(DO_STAT);

		//series like: do id.id() or do id or do id.id.id ... .id()

		if (token.token != KW_DO) THROW_TOKEN("TOKEN SHOULD BE \"do\"");
		tna();
		if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
		tna();

		while (token.token == SYM_PER) {
			if (token.token != SYM_PER) THROW_TOKEN("TOKEN SHOULD BE \".\"");
			tna();
			if (token.type != TYPE_ID) THROW_TOKEN("TOKEN SHOULD BE IDENTIFIER");
			tna();
		}

		if (token.token != SYM_PAF) THROW_TOKEN("TOKEN SHOULD BE \"(\"");
		tna();
		compile_exp_list();
		if (token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
		tna();
		if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
		tna();

		tc(DO_STAT);
	}
	void compile_return() {
		to(RET_STAT);
		if (token.token != KW_RETURN) THROW_TOKEN("TOKEN SHOULD BE \"return\"");
		tna();
		if (token.token == SYM_SMC) {
			tna();
		} else {
			compile_expression();
			if (token.token != SYM_SMC) THROW_TOKEN("TOKEN SHOULD BE \";\"");
			tna();
		}
		tc(RET_STAT);
	}
	void compile_expression() {
		to(EXPRESSION);
		/*if (token.token == SYM_PAF) {
			tna();
			compile_expression();
			if(token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
			tna();
		}*/

		while (true) {
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
			compile_term();
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
			//tna();       
			//if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM) break;
			if (token.token == SYM_PAF) {
				tna();
				compile_expression();
				if(token.token != SYM_PAB) THROW_TOKEN("TOKEN SHOULD BE \")\"");
			}
			tna();
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
		}

		tc(EXPRESSION);
	}
	void compile_term() {
		to(TERM);
		if (token.token == SYM_MIN || token.token == SYM_TIL) {
			tna();
			compile_term();
		}
		//have to determine if this term is a function call or not
		bool function_call = false;

		if (token.token == SYM_PAF || token.token == SYM_SBF) {
			tna();
			if (function_call) compile_exp_list();
			else compile_expression();
			if(token.token != SYM_PAB && token.token != SYM_SBB) THROW_TOKEN("TOKEN SHOULD BE \")\" OR \"]\"");
			tna();
		}
		while (             
			token.token != SYM_ADD && token.token != SYM_MIN && token.token != SYM_DIV && 
			token.token != SYM_OR && token.token != SYM_MUL && token.token != SYM_ALT_LT && 
			token.token != SYM_ALT_GT && token.token != SYM_ALT_AND && token.token != SYM_COM &&
			token.token != SYM_SMC && token.token != SYM_PAB && token.token != SYM_SBB && token.token != SYM_EQ
		) {
			tna();
			if (token.token == SYM_PER) function_call = true;
			if (token.token == SYM_PAF || token.token == SYM_SBF) {
				tna();
				if (function_call) compile_exp_list();
				else compile_expression();
				if(token.token != SYM_PAB && token.token != SYM_SBB) THROW_TOKEN("TOKEN SHOULD BE \")\" OR \"]\"");
				tna();
			}
		}

		tc(TERM);
	}
	void compile_exp_list() {
		to(EXP_LIST);
		//series like: exp, exp, exp
		while(true) {
			if (token.token == SYM_PAB) break;
			compile_expression();
			if (token.token != SYM_COM) break;
			tna();
		}
		tc(EXP_LIST);
	}

};

int main(int argc, char* argv[]) {
	ifstream fin("test.jack");
	ofstream fout("test.xml");
	/*if (argc < 2) return -1;
	if (argc < 3) {
		string arg(argv[1]);
		cout << arg << endl;
		cout << (arg.substr(0, arg.find_last_of('.')) + "Output.xml") << endl;
		fin = ifstream(arg);
		fout = ofstream(arg.substr(0, arg.find_last_of('.')) + "Output.xml");
	} else {
		cout << argv[2] << endl << argv[1] << endl;
		fin = ifstream(argv[2]);
		fout = ofstream(argv[1]);
	}*/

	AnalyzerEngine engine(fout, fin);

	engine.compile();

}