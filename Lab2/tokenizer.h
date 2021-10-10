bool delim_character(char c);
bool non_delim_character(char c);
char *word_start(char* str);
char *end_word(char* str);
int count_tokens(char* str);
char *copy_str(char *inStr, short len);
char** tokenize(char** tokenizer, char* start, char* end, int idx, int tokens);
void print_all_tokens(char** tokens, int limit);
