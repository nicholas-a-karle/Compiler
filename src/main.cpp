#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

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

#define SEG_ARG		"argument"
#define SEG_LOC		"local"
#define SEG_STAT	"static"
#define SEG_THIS	"this"	
#define SEG_THAT	"that"
#define SEG_POINT	"pointer"
#define SEG_TEMP	"temp"
#define SPACE		" "

#define PUSH_CONST	"push constant "

#define PUSH		"push "
#define POP			"pop "

#define MATH_ADD	"add"
#define MATH_SUB	"sub"
#define MATH_NEG	"neg"
#define MATH_EQ 	"eq"
#define MATH_GT 	"gt"
#define MATH_LT 	"lt"
#define MATH_AND	"and"
#define MATH_OR 	"or"
#define MATH_NOT	"not"

#define LABEL		"label "
#define GOTO		"goto "
#define IFGOTO		"if-goto "
#define FUNC		"function "
#define CALL		"call "
#define RETURN		"return"
#define KIND_STATIC	"KSTATIC"
#define KIND_FIELD	"KFIELD"
#define KIND_ARG	"KARG"
#define KIND_VAR	"KVAR"
#define NO_KIND		"KNONE"

#define IN_STATIC	0
#define IN_FIELD	1
#define IN_ARG		2
#define IN_VAR		3
#define R_INS_SZ	4

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
	streampos spot;

	//constructor
    Tokenizer() {
        //fin remains undefined
    }

    //constructor
    Tokenizer(ifstream &fin) {
        this->fin = &fin;
		save_spot();
    }

    void setFile(ifstream &fin) { this->fin = &fin; }

	void reset() { fin->seekg(ios::beg); }

	void save_spot() {
		spot = fin->tellg();
	}
	void load_spot() {
		fin->seekg(spot);
	}
    
    Token tokenize() {
        string token = "";
        string type = INVALID_TYPE;
        char c = 'a';

        //read until a valid token
        while (true) {
			if(!fin->get(c)) 
				return END_TOKEN_BUILD;

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

};

struct AnalyzerEngine {

	ofstream *fout;
	Tokenizer tokenizer;
	string prepend;
	Token token;

	AnalyzerEngine() {}
	
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

	void to(string tag) { 
		(*fout) << (prepend + O1 + tag + C0) << endl; 
		indent(); 
	}
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

//name: type, kind, offset
struct SymbolTable {

	string table_name;
	map<string, tuple<string, string, int>> table;
	vector<int> rins;

	SymbolTable() {
		table_name = "";
		rins = vector<int>(R_INS_SZ, 0);
		table = map<string, tuple<string, string, int>>();
	}

	SymbolTable(string name) {
		table_name = name;
		rins = vector<int>(R_INS_SZ, 0);
		table = map<string, tuple<string, string, int>>();
	}

	void reset() {
		rins = vector<int>(R_INS_SZ, 0);
		table = map<string, tuple<string, string, int>>();
	}

	bool define(string name, string type, string kind) {
		if (table.find(name) == table.end()) {
			table[name] = {type, kind, var_count(kind)};
			if (kind == KIND_STATIC) {
				++rins[IN_STATIC];
			} else if (kind == KIND_FIELD) {
				++rins[IN_FIELD];
			} else if (kind == KIND_ARG) {
				++rins[IN_ARG];
			} else if (kind == KIND_VAR) {
				++rins[IN_VAR];
			} else {
				return false;
			}
			return true;
		} //if symbol exists, ignore, need not edit
		return false;
	}

	int var_count(string kind) {
		if (kind == KIND_STATIC) {
			return rins[IN_STATIC];
		} else if (kind == KIND_FIELD) {
			return rins[IN_FIELD];
		} else if (kind == KIND_ARG) {
			return rins[IN_ARG];
		} else if (kind == KIND_VAR) {
			return rins[IN_VAR];
		} else {
			return -1;
		}
	}

	string kind(string name) {
		if (get<1>(table[name]) != KIND_STATIC && get<1>(table[name]) != KIND_FIELD 
		&& get<1>(table[name]) != KIND_ARG && get<1>(table[name]) != KIND_VAR) {
			return get<1>(table[name]);
		} else {
			return NO_KIND;
		}
	}

	string type(string name) {
		return get<0>(table[name]);
	}

	int index(string name) {
		return get<2>(table[name]);
	}

	bool declared(string name) {
		return table.find(name) != table.end();
	}

};

struct VMWriter{

	ofstream* fout;

	VMWriter() {}

	VMWriter(ofstream &fout) {
		this->fout = &fout;
	}

	void _set_file(ofstream &fout) { this->fout = &fout; }

	void out(string str) { (*fout) << str << endl; }

	void _push(string segment, int index) {
		out(PUSH + segment + SPACE + to_string(index));
	}

	void _pop(string segment, int index) {
		out(POP + segment + SPACE + to_string(index));
	}

	void _push(tuple<string, string, int> variable) {
		_push(ktos(get<1>(variable)), get<2>(variable));
	}

	void _pop(tuple<string, string, int> variable) {
		_pop(ktos(get<1>(variable)), get<2>(variable));
	}

	void _push_constant(int constant) {
		out(PUSH_CONST + to_string(constant));
	}

	void _push_int(string integer) {
		out(PUSH_CONST + integer);
	}

	void _push_string(string str) {
		_push_constant(str.length());
		_call("String.new", 1);
		for (int i = 0; i < str.length(); ++i) {
			_push_constant(str[i]);
			_call("String.appendChar", 2);
		}
	}

	void _add() {out(MATH_ADD); }
	void _sub() {out(MATH_SUB); }
	void _neg() {out(MATH_NEG); }
	void _eq()  {out(MATH_EQ); }
	void _gt()  {out(MATH_GT); }
	void _lt()  {out(MATH_LT); }
	void _and() {out(MATH_AND); }
	void _or()  {out(MATH_OR); }
	void _not() {out(MATH_NOT); }

	void _label(string label) { out(LABEL + label); }

	void __goto(string label) { out(GOTO + label); }

	void __ifgoto(string label) { out(IFGOTO + label); }

	void _call(string label, int nArgs) {
		out(CALL + label + SPACE + to_string(nArgs));
	}

	void _func(string label, int nVars) {
		out(FUNC + label + SPACE + to_string(nVars));
	}

	void _ret() { out(RETURN); }

	string ktos(string kind) {
		if (kind == KIND_STATIC) return SEG_STAT;
		if (kind == KIND_FIELD) return SEG_THIS;
		if (kind == KIND_ARG) return SEG_ARG;
		if (kind == KIND_VAR) return SEG_LOC;
		return "";
	}

};

struct CompilationEngine {
	Tokenizer tokenizer;
	VMWriter writer;
	Token token;
	int num_labels;

	CompilationEngine() {}

	CompilationEngine(ofstream &fout, ifstream &fin) {
		tokenizer = Tokenizer(fin);
		writer = VMWriter(fout);
		token = Token("", "");
	}

	string label() {
		++num_labels;
		return "ARBITRARY_LABEL_" + to_string(num_labels);
	}

	string label(string pre) {
		++num_labels;
		return pre + "_LABEL_" + to_string(num_labels);
	}

	void advance() { token = tokenizer.tokenize(); }

	void compile() { compile_class(); }

	void compile_class() {
		advance(); 
		advance();

		SymbolTable class_vars(token.token);

		advance();

		compile_cvars(class_vars);
		compile_subrs(class_vars);


	}
	void compile_cvars(SymbolTable &class_vars) {
		//compiles all class vars
		string kind, type;
		tokenizer.reset();

		while (token.token != END) {
			if (token.token == KW_FIELD || token.token == KW_STATIC) {
				//varType dataType varName ... , varName, varName ... ;
				kind = token.token;
				advance();
				type = token.token;
				advance();
				while (token.token != SYM_SMC) {
					class_vars.define(token.token, type, kind);
					advance();
					if (token.token == SYM_SMC) break;
					advance();
				}
			}
			advance();
		}

		tokenizer.reset();
	}
	void compile_subrs(SymbolTable &class_vars) {
		string subr_type, ret_type, subr_name;
		tokenizer.reset();

		while (token.token != END) {
			if (token.token == KW_CONSTRUCTOR || token.token == KW_FUNCTION || token.token == KW_METHOD) {
				SymbolTable local_vars;
				subr_type = token.token;
				advance();
				ret_type = token.token;
				advance();
				subr_name = token.token;
				local_vars.table_name = subr_type + "_" + ret_type + "_" + subr_name;
				advance(); //pass (
				advance();
				compile_param_list(class_vars, local_vars);
				advance(); //pass )
				advance(); //pass {
				compile_subr_body(class_vars, local_vars, subr_type);
				advance(); //pass }
			}
			advance();
		}

		tokenizer.reset();
	}
	void compile_param_list(SymbolTable &class_vars, SymbolTable &local_vars) {
		//pattern: (type name, type name, ...)
		//should start at first type

		while (token.token != SYM_PAB) {
			string param_type, param_name;
			
			param_type = token.token;
			advance();
			param_name = token.token;

			local_vars.define(param_name, param_type, KIND_ARG);

			advance(); //pass by comma
			advance();
		}

	}
	void compile_subr_body(SymbolTable &class_vars, SymbolTable &local_vars, string subr_type) {
		//already at first token, { advanced past
		compile_subr_vars(class_vars, local_vars);

		//
		// WRITE FUNCTION
		//

		if (subr_type == KW_CONSTRUCTOR) {
			int field_count = class_vars.var_count(KIND_FIELD);
			//push constant field_count
			//call "Memory" "alloc" 1
		} else if (subr_type == KW_METHOD) {
			//push argument 0
			writer._push(SEG_ARG, 0);
			//pop point 0
			writer._pop(SEG_POINT, 0);
		}

		compile_statements(class_vars, local_vars);
		advance();
	}
	void compile_subr_vars(SymbolTable &class_vars, SymbolTable &local_vars) {
		tokenizer.save_spot();
		int cbi = 1;
		while (cbi > 0) {
			if (token.token == KW_VAR) {
				//should be: var data_type name;  or var data_type name, name
				string type, name;
				advance(); //pass var
				type = token.token;
				advance();
				name = token.token;
				local_vars.define(name, type, KIND_VAR);
				advance(); //check if ,
				while (token.token == SYM_COM) {
					advance();
					name = token.token;
					local_vars.define(name, type, KIND_VAR);
					advance();
				}
				advance(); //pass ;
			} else if (token.token == SYM_CBB) --cbi;
			else if (token.token == SYM_CBF) ++cbi;
		}

		tokenizer.load_spot();
	}
	void compile_statements(SymbolTable &class_vars, SymbolTable local_vars) {
		bool check_statements = true;
		while (check_statements) {
			if (token.token == KW_IF) {
				compile_if(class_vars, local_vars);
			} else if (token.token == KW_WHILE) {
				compile_while(class_vars, local_vars);
			} else if (token.token == KW_LET) {
				compile_let(class_vars, local_vars);
			} else if (token.token == KW_DO) {
				compile_do(class_vars, local_vars);
			} else if (token.token == KW_RETURN) {
				compile_return(class_vars, local_vars);
			} else {
				check_statements = false;
			}
		}
	}
	void compile_let(SymbolTable &class_vars, SymbolTable local_vars) {

		advance(); //pass let keyword
		string identifier = token.token;
		advance(); //pass var name

		if (token.token == SYM_SBF) {
			//array
			advance(); //pass [
			compile_expression(class_vars, local_vars);
			advance(); //pass ]
			advance(); //pass =
			//push symbol identifier
			if (local_vars.declared(identifier)) writer._push(local_vars.table[identifier]);
			if (class_vars.declared(identifier)) writer._push(class_vars.table[identifier]);
			//write add
			compile_expression(class_vars, local_vars);
			//pop temp 0
			writer._pop(SEG_TEMP, 0);
			//pop point 1
			writer._pop(SEG_POINT, 1);
			//push temp 0
			writer._push(SEG_TEMP, 0);
			//pop that 0
			writer._pop(SEG_THAT, 0);

		} else {
			//not array
			advance(); //pass =
			compile_expression(class_vars, local_vars);
			//pop symbol identifier
			if (local_vars.declared(identifier)) writer._pop(local_vars.table[identifier]);
			if (class_vars.declared(identifier)) writer._pop(class_vars.table[identifier]);
		}
		advance(); //pass ;
	}
	void compile_if(SymbolTable &class_vars, SymbolTable local_vars) {
		advance(); advance(); // pass if and (

		compile_expression(class_vars, local_vars);

		advance(); advance(); // pass ) and {

		string label_false = label("IF_CASE_FALSE");
		string label_end_if = label("IF_END");

		//write if label_false
		writer.__ifgoto(label_false);

		compile_statements(class_vars, local_vars);

		//in case of else
		if (token.token == KW_ELSE) {
			advance(); advance(); //pass else and {
			compile_statements(class_vars, local_vars);
			advance(); //pass }
		}
		
		//write label label_end_if
		writer._label(label_end_if);
		
	}
	void compile_while(SymbolTable &class_vars, SymbolTable local_vars) {

		advance(); advance(); //pass while and {
		
		string label_begin = label("WHILE_BEGIN");
		string label_false = label("WHILE_CASE_FALSE");

		// write label label_begin
		writer._label(label_begin);
		compile_expression(class_vars, local_vars);

		advance(); advance(); //pass ) and {
		
		//write if label_false
		writer.__ifgoto(label_false);

		compile_statements(class_vars, local_vars);

		//write goto label_begin
		writer.__goto(label_begin);
		//write label label_false
		writer._label(label_false);
	}
	void compile_do(SymbolTable &class_vars, SymbolTable local_vars) {
		advance(); //pass keyword do
		compile_term(class_vars, local_vars);
		//pop temp 0
		writer._pop(SEG_TEMP, 0);
		advance(); //pass ;
	}
	void compile_return(SymbolTable &class_vars, SymbolTable local_vars) {
		advance(); //pass keyword return
		if (token.token != SYM_SMC) {
			compile_expression(class_vars, local_vars);
		} else {
			//write int 0
			writer._push_constant(0);
		}
		//write return
		writer._ret();
		advance(); //pass ;
	}
	void compile_expression(SymbolTable &class_vars, SymbolTable local_vars) {

		while (true) {
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
			compile_term(class_vars, local_vars);
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
			
			if (token.token == SYM_PAF) {
				advance();
				compile_expression(class_vars, local_vars);
			}
			advance();
			if (token.token == SYM_PAB || token.token == SYM_SMC || token.token == SYM_COM || token.token == SYM_SBB) break;
		}

	}
	void compile_term(SymbolTable &class_vars, SymbolTable local_vars) {
		bool not_var = true;
		if (token.token == SYM_MIN) {
			//write neg
			writer._neg();
		} else if (token.token == SYM_TIL) {
			//wrtie not
			writer._not();
		} else if (token.token == SYM_PAF) {
			compile_expression(class_vars, local_vars);
			advance();
		} else if (token.type == TYPE_IC) {
			//write int (token.token)
			writer._push_int(token.token);
		} else if (token.type == TYPE_SC) {
			//write string (token.token)
			writer._push_string(token.token);
		} else if (token.token == KW_THIS) {
			//push pointer 0
			writer._push(SEG_POINT, 0);
		} else if (token.token == KW_FALSE || token.token == KW_NULL) {
			//push constant 0
			writer._push_constant(0);
		} else if (token.token == KW_TRUE) {
			//push constant 0, not
			writer._push_constant(0);
			writer._not();
		} else if (token.type == TYPE_ID) {
			//variable, array, and call handling
			
			//save identifier
			string identifier = token.token;
			advance(); //next token after identifier

			if (token.token == SYM_SBF) {

				advance(); //pass [
				compile_expression(class_vars, local_vars);
				//push symbol identifier
				if (local_vars.declared(identifier)) writer._push(local_vars.table[identifier]);
				if (class_vars.declared(identifier)) writer._push(class_vars.table[identifier]);
				//write add
				writer._add();
				//pop pointer 1
				writer._pop(SEG_POINT, 1);
				//push that 0
				writer._push(SEG_THAT, 0);
				advance(); //pass ]

			} else {
				//token_value = token_var = identifier
				//names of function and function's class
				string func_name = identifier;
				string func_class = class_vars.table_name;
				//whether or not this is a method call
				bool method_call = true;
				//number of arguments in paremeters
				int num_args = 0;

				if (token.token == SYM_PER) {
					method_call = false;
					advance(); //pass .
					//func_obj = identifier;
					func_name = token.token;
					advance();
					if (class_vars.declared(identifier)) {
						func_class = class_vars.type(identifier);
						goto class_func_setup;
					} else if (local_vars.declared(identifier)) {
						func_class = local_vars.type(identifier);
						class_func_setup:
						num_args = 1;
						//push symbol identifier
						if (local_vars.declared(identifier)) writer._push(local_vars.table[identifier]);
						if (class_vars.declared(identifier)) writer._push(class_vars.table[identifier]);
					} else {
						//function call from class
						func_class = identifier;
					}
				}

				if (token.token == SYM_PAF) {
					if (method_call) {
						num_args = 1;
						//push pointer 0
						writer._push(SEG_POINT, 0);
					}
					advance(); //pass (
					num_args += compile_exp_list(class_vars, local_vars);
					//write call func_class.func_name, num_args
					writer._call(func_class + "." + func_name, num_args);
					advance(); //pass )
				} else {
					//push symbol identifier
					if (local_vars.declared(identifier)) writer._push(local_vars.table[identifier]);
					if (class_vars.declared(identifier)) writer._push(class_vars.table[identifier]);
				}
			}
		}
	}
	int compile_exp_list(SymbolTable &class_vars, SymbolTable local_vars) {
		int i = 0;
		while (token.token != SYM_PAB) {
			if (token.token == SYM_SMC) advance(); //pass ,
			++i;
			compile_expression(class_vars, local_vars);
		}
		return i;
	}
};

struct Compiler {
	CompilationEngine *engine_c;
	AnalyzerEngine *engine_a;

	Compiler() {}

	Compiler(string inputFileName) {
		string fn = inputFileName.substr(0, inputFileName.find_last_of("."));
		ifstream *fin = new ifstream(inputFileName);
		ofstream *xmlFout = new ofstream(fn + ".xml");
		ofstream *vmFout = new ofstream(fn + ".vm");

		cout << inputFileName << " : \t" << fin->is_open() << endl;
		cout << fn << ".xml : \t" << xmlFout->is_open() << endl;
		cout << fn << ".vm : \t" << vmFout->is_open() << endl;

		engine_c = new CompilationEngine(*vmFout, *fin);
		engine_a = new AnalyzerEngine(*xmlFout, *fin);
	}

	void compile() { engine_c->compile(); }

	void analyze() { engine_a->compile(); }
};

int main(int argc, char* argv[]) {
	string testFileName = "test.jack";

	Compiler compiler(testFileName);

	compiler.analyze();

	compiler.compile();
}