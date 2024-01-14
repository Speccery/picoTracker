#ifndef __STRING_TOKENIZER__
#define __STRING_TOKENIZER__

class Token {
private:
  bool is_str_;
  bool is_int_;
  int int_value_;
  char *str_value_;
  Token *next_token_;

public:
  Token(char *string);
  ~Token();

  inline bool IsInt() { return is_int_; };
  inline bool IsStr() { return is_str_; };

  inline bool GetInt(int &int_value);
  inline bool GetStr(char **str_value);

  inline Token *GetNext() { return next_token_; };
  inline void SetNext(Token *next_token) { next_token_ = next_token; };
};

class StringTokenizer {
private:
  Token *first_token_;

public:
  StringTokenizer(char *string, int nb_separators, char *separators);
  ~StringTokenizer();
  Token *GetFirstToken() { return first_token_; };
};

Token::Token(char *string)
    : is_str_(false), is_int_(false), int_value_(0), str_value_(NULL),
      next_token_(NULL) {

  int len = (int)strlen(string);
  bool has_only_numeric_chars = true;
  for (int i = 0; i < len; i++) {
    if (((string[i] < '0') || (string[i] > '9')) && (string[i] != '-')) {
      has_only_numeric_chars = false;
      break;
    }
  }

  if (has_only_numeric_chars) {
    is_int_ = true;
    sscanf(string, "%d", &int_value_);
  } else {
    is_str_ = true;
    str_value_ = (char *)malloc(strlen(string) + 1);
    strcpy(str_value_, string);
  }
}

Token::~Token() {
  if (str_value_)
    free(str_value_);
}

bool Token::GetInt(int &int_value) {
  if (is_int_) {
    int_value = int_value_;
    return true;
  } else
    return false;
}

bool Token::GetStr(char **str_value) {
  if (is_str_) {
    *str_value = str_value_;
    return true;
  } else
    return false;
}

inline bool IsSeparator(char c, int nb_separators, char *separators) {
  for (int i = 0; i < nb_separators; i++)
    if (c == separators[i])
      return true;

  return false;
}

StringTokenizer::StringTokenizer(char *string, int nb_separators,
                                 char *separators)
    : first_token_(NULL) {
  Token *current_token = NULL;

  int len = (int)strlen(string);
  char tmp_str[MAX_TOKEN_SIZE];

  int i = 0;
  while (i < len) {

    while (IsSeparator(string[i], nb_separators, separators) && (i < len))
      i++;

    int token_first_char_i = i;

    // special case: "text 1 a b 32" is one token !
    if ((i > 0) && (string[i - 1] == '"')) {
      while ((string[i] != '"') && (i < len))
        i++;
    }
    // special case: "text 1 a b 32" is one token !
    else if ((i > 0) && (string[i - 1] == '\'')) {
      while ((string[i] != '\'') && (i < len))
        i++;
    } else
      while (!IsSeparator(string[i], nb_separators, separators) && (i < len))
        i++;

    int token_len = i - token_first_char_i;

    strncpy(tmp_str, string + token_first_char_i, token_len);
    tmp_str[token_len] = 0;

    // in case a separator is at the end of the string
    // the string will be ""
    if (strlen(tmp_str) > 0)
      if (current_token == NULL) {
        first_token_ = new Token(tmp_str);
        current_token = first_token_;
      } else {
        current_token->SetNext(new Token(tmp_str));
        current_token = current_token->GetNext();
      }
  }
}

StringTokenizer::~StringTokenizer() {
  Token *token = GetFirstToken();

  if (token) {
    Token *next = token->GetNext();

    if (token != NULL)
      delete token;

    while (next != NULL) {
      token = next;
      next = token->GetNext();
      delete token;
    }
  }
}

#endif