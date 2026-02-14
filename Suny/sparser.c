#include "sparser.h"
#include "serror.h"
#include "smem.h"
#include "stok.h"

struct Sparser *
Sparser_new(void) {
    struct Sparser *parser = Smem_Malloc(sizeof(struct Sparser));
    parser->ast = NULL;
    parser->lexer = NULL;
    ptoken(parser) = NULL;
    parser->next_token = NULL;
    return parser;
}

struct Sparser *
Sparser_init
(struct Slexer *lexer) {
    struct Sparser *parser = Sparser_new();
    parser->lexer = lexer;
    return parser;
}

int 
Sparser_free
(struct Sparser *parser) {
    if (parser->lexer) Slexer_free(parser->lexer);
    if (parser->ast) Sast_free(parser->ast);
    if (ptoken(parser)) Stok_free(ptoken(parser));
    if (parser->next_token) Stok_free(parser->next_token);
    parser = NULL;
    return 0;
}

struct Sast *
Sparser_parse_program
(struct Sparser *parser) {
    struct Sast *ast = Sast_init(AST_PROGRAM, 0, NULL);

    pnext(parser);

    while (ptoken(parser)->type != EOF_TOK) {
        struct Sast *statement = Sparser_parse(parser);

        if (!statement) {
            Serror_parser("Expected statement", parser->lexer);
            return NULL;
        }

        Sast_add_child(ast, statement);
    }

    return ast;
}

struct Sast *
Sparser_parse(struct Sparser *parser) {
    if (ptoken(parser)->type == IDENTIFIER) {
        parser->next_token = Slexer_look_ahead(parser->lexer);

        if (parser->next_token->type == ASSIGN) {
            return Sparser_parse_assignment(parser);
        }
    }

    if (ptoken(parser)->type == IMPORT) {
        return Sparser_parse_import(parser);
    }

    if (ptoken(parser)->type == INCLUDE) {
        return Sparser_parse_include(parser);
    }

    if (ptoken(parser)->type == DO) {
        return Sparser_parse_block(parser);
    }

    if (ptoken(parser)->type == CLASS) {
        return Sparser_parse_class(parser);
    }

    if (ptoken(parser)->type == RETURN) {
        return Sparser_parse_return(parser);
    }

    if (ptoken(parser)->type == BREAK) {
        struct Sast *node = AST(AST_BREAK, 0, NULL);
        Sast_set_line(parser->lexer, node);

        pnext(parser);
        return node;
    } 

    if (ptoken(parser)->type == CONTINUE) {
        struct Sast *node = AST(AST_CONTINUE, 0, NULL);
        Sast_set_line(parser->lexer, node);

        pnext(parser);
        return node;
    }

    if (ptoken(parser)->type == FOR) {
        return Sparser_parse_for(parser);
    }

    if (ptoken(parser)->type == FUNCTION) {
        parser->next_token = Slexer_look_ahead(parser->lexer);
        if (parser->next_token->type == LPAREN) {
            return Sparser_parse_logic_expression(parser);
        }

        return Sparser_parse_function(parser);
    }

    if (ptoken(parser)->type == WHILE) {
        return Sparser_parse_while(parser);
    }

    if (ptoken(parser)->type == LET) {
        return Sparser_parse_let(parser);
    }

    if (ptoken(parser)->type == LOOP) {
        return Sparser_parse_loop(parser);
    }

    if (ptoken(parser)->type == IF) {
        return Sparser_parse_if(parser);
    }

    if (ptoken(parser)->type == PRINT_T) {
        return Sparser_parse_print(parser);
    }

    return Sparser_parse_logic_expression(parser);
}

struct Sast* Sparser_parse_second_primary_expression(struct Sparser *parser) {
    struct Sast *node = Sparser_parse_primary_expression(parser);

    parser->next_token = Slexer_look_ahead(parser->lexer);
    
    if (parser->next_token->type == LPAREN) {
        return Sparser_parse_function_call(parser, node);
    } else if (parser->next_token->type == LBRACKET) {
        return Sparser_parse_extract(parser, node);
    } else if (parser->next_token->type == DOT) {
        return Sparser_parse_attribute_expression(parser, node);
    } else if (parser->next_token->type == SEMICOLON) {
        return node;
    }

    return node;
}

struct Sast *
Sparser_parse_primary_expression
(struct Sparser *parser) {
    if (ptoken(parser)->type == LPAREN) {
        struct Sast *node = Sparser_parse_parent_expression(parser);
        Sast_set_line(parser->lexer, node);
        return node;
    }

    if (ptoken(parser)->type == SUB) {
        struct Sast *left = AST(AST_LITERAL, 0, NULL);
        pnext(parser);

        struct Sast *right = Sparser_parse_second_primary_expression(parser);

        struct Sast *node = AST(AST_BINARY_EXPRESSION, 0, NULL);
        node->left = left;
        node->right = right;
        node->op = SUB;
        Sast_set_line(parser->lexer, node);
        return node;
    }

    if (ptoken(parser)->type == NULL_T) {
        struct Sast *node = AST(AST_NULL_EXPRESSION, 0, NULL);
        Sast_set_line(parser->lexer, node);
        return node;
    }

    if (ptoken(parser)->type == TRUE_T) {
        struct Sast *node = AST(AST_TRUE, 1, NULL);
        Sast_set_line(parser->lexer, node);
        return node;
    }

    if (ptoken(parser)->type == FALSE_T) {
        struct Sast *node = AST(AST_FALSE, 0, NULL);
        Sast_set_line(parser->lexer, node);
        return node;
    }

    if (ptoken(parser)->type == IDENTIFIER) {
        parser->next_token = Slexer_look_ahead(parser->lexer);

        struct Sast *node = AST(AST_IDENTIFIER, ptoken(parser)->value, ptoken(parser)->lexeme);

        Sast_set_line(parser->lexer, node);

        parser->next_token = Slexer_look_ahead(parser->lexer);
        if (parser->next_token->type == LBRACKET) {
            return Sparser_parse_extract(parser, node);
        } else if (parser->next_token->type == DOT) {
            return Sparser_parse_attribute_expression(parser, node);
        } else if (parser->next_token->type == LPAREN) {
            return Sparser_parse_function_call(parser, node);
        }

        return node;
    }

    if (ptoken(parser)->type == FUNCTION) {
        parser->next_token = Slexer_look_ahead(parser->lexer);
        if (parser->next_token->type == LPAREN) {
            return Sparser_parse_anonymous_function(parser);
        }
        
        Serror_parser("Expected anonymous function", parser->lexer);
    }

    if (ptoken(parser)->type == LBRACKET) {
        struct Sast *node = Sparser_parse_list(parser);

        Sast_set_line(parser->lexer, node);

        parser->next_token = Slexer_look_ahead(parser->lexer);
        
        if (parser->next_token->type == LBRACKET) {
            return Sparser_parse_extract(parser, node);
        }

        return node;
    }

    if (ptoken(parser)->type == NUMBER) {
        struct Sast *node = AST(AST_LITERAL, ptoken(parser)->value, ptoken(parser)->lexeme);

        Sast_set_line(parser->lexer, node);

        parser->next_token = Slexer_look_ahead(parser->lexer);
        if (parser->next_token->type == LBRACKET) {
            Serror_parser("Numeric expression cannot be extracted", parser->lexer);
        }

        return node;
    }

    if (ptoken(parser)->type == STRING) {
        struct Sast *node = AST(AST_STRING_EXPRESSION, ptoken(parser)->value, ptoken(parser)->lexeme);

        Sast_set_line(parser->lexer, node);

        parser->next_token = Slexer_look_ahead(parser->lexer);
        if (parser->next_token->type == LBRACKET) {
            Serror_parser("String expression cannot be extracted", parser->lexer);
        }

        return node;
    }

    if (ptoken(parser)->type == LBRACE) {
        struct Sast* node = Sparser_parse_map(parser);
        Sast_set_line(parser->lexer, node);

        return node;
    }

    Serror_parser(Sstring("Expected primary expression before '%s'", ptoken(parser)->lexeme), parser->lexer);
    return NULL;
}

struct Sast *
Sparser_parse_additive_expression
(struct Sparser *parser) {
    struct Sast *left = Sparser_parse_multiplicative_expression(parser);

    while (ptoken(parser)->type == ADD || ptoken(parser)->type == SUB) {
        enum Stok_t op = ptoken(parser)->type;

        pnext(parser);
        struct Sast *right = Sparser_parse_multiplicative_expression(parser);

        if (!right) {
            Serror_parser("Expected multiplicative expression", parser->lexer);
            return NULL;
        }

        struct Sast *node = AST(AST_BINARY_EXPRESSION, 0, NULL);

        node->left = left;
        node->right = right;
        node->op = op;
        
        left = node;
    }

    return left;
}

struct Sast *
Sparser_parse_multiplicative_expression
(struct Sparser *parser) {
    struct Sast *left = Sparser_parse_second_primary_expression(parser);

    pnext(parser);
    while (ptoken(parser)->type == MUL || ptoken(parser)->type == DIV || ptoken(parser)->type == MOD) {
        enum Stok_t op = ptoken(parser)->type;

        pnext(parser);
        struct Sast *right = Sparser_parse_second_primary_expression(parser);

        if (!right) {
            Serror_parser("Expected primary expression", parser->lexer);
            return NULL;
        }

        struct Sast *node = AST(AST_BINARY_EXPRESSION, 0, NULL);

        node->left = left;
        node->right = right;
        node->op = op;
        
        left = node;

        pnext(parser);
    }

    return left;
}

struct Sast *
Sparser_parse_let
(struct Sparser *parser) {
    struct Sast *node = AST(AST_ASSIGNMENT, 0, NULL);
    pnext(parser);

    char* lexeme;

    if (ptoken(parser)->type == IDENTIFIER) {
        node->var_name = ptoken(parser)->lexeme;
        lexeme = ptoken(parser)->lexeme;
    } else {
        Serror_parser("Expected identifier", parser->lexer);
        return NULL;
    }

    pnext(parser);

    if (ptoken(parser)->type == ASSIGN) {
        pnext(parser);

        node->var_value = Sparser_parse(parser);

        Sast_set_line(parser->lexer, node->var_value);
        Sast_set_line(parser->lexer, node);

        Serror_expected_expression(node->var_value);

        return node;
    } else if (ptoken(parser)->type == LPAREN) {
        struct Sast *node = AST(AST_FUNCTION_STATEMENT, 0, NULL);

        node->lexeme = lexeme;

        while (ptoken(parser)->type != RPAREN) {
            pnext(parser);
            if (ptoken(parser)->type == IDENTIFIER) {
                Sast_set_para(node, ptoken(parser)->lexeme);
                node->args_count++;
                node->is_having_params = 1;
                pnext(parser);
                if (ptoken(parser)->type == COMMA) {
                    continue;
                } else if (ptoken(parser)->type == RPAREN) {
                    break;
                } else {
                    Serror_parser("Expected comma or closing parenthesis", parser->lexer);
                    return NULL;
                }
            } else if (ptoken(parser)->type == RPAREN) {
                break;
            } else {
                Serror_parser("Expected identifier in function", parser->lexer);
                return NULL;
            }
        }

        pnext(parser);
        if (ptoken(parser)->type == ASSIGN) {
            pnext(parser);
            struct Sast *expr = Sparser_parse(parser);

            Sast_set_line(parser->lexer, expr);
            Serror_expected_expression(expr);

            node->expr = expr;
            node->is_lambda = 1;

            return node;
        } else {
            Serror_parser("Expected assignment", parser->lexer);
            return NULL;
        }
    } else {
        Serror_parser("Expected assignment", parser->lexer);
        return NULL;
    }

    return NULL;
}

struct Sast *
Sparser_parse_print
(struct Sparser *parser) {
    struct Sast *node = AST(AST_PRINT, 0, NULL);

    pnext(parser);

    node->print_value = Sparser_parse(parser);

    Sast_set_line(parser->lexer, node->print_value);
    Serror_expected_expression(node->print_value);

    return node;
}

struct Sast *
Sparser_parse_logic_expression
(struct Sparser *parser) {
    struct Sast *left = Sparser_parse_or(parser);

    if (!left) {
        Serror_parser("Expected comparison expression", parser->lexer);
        return NULL;
    }

    if (ptoken(parser)->type == ASSIGN || ptoken(parser)->type == ADD_ASSIGN || ptoken(parser)->type == SUB_ASSIGN || ptoken(parser)->type == MUL_ASSIGN || ptoken(parser)->type == DIV_ASSIGN) {
        if (left->type == AST_EXTRACT) {
            return Sparser_parse_store_index(parser, left);
        } else if (left->type == AST_ATTRIBUTE_EXPRESSION) {
            return Sparser_parse_store_attribute(parser, left);
        }
    }

    return left;
}

struct Sast *
Sparser_parse_parent_expression
(struct Sparser *parser) {
    if (ptoken(parser)->type == LPAREN) {
        pnext(parser);

        struct Sast* node = Sparser_parse(parser);

        Sast_set_line(parser->lexer, node);
        Serror_expected_expression(node);

        if (ptoken(parser)->type == RPAREN) {
            return node;
        } else {
            Serror_parser("Expected closing parenthesis", parser->lexer);
            return NULL;
        }
    } else {
        Serror_parser("Expected opening parenthesis", parser->lexer);
        return NULL;
    }
    return NULL;
}

struct Sast *
Sparser_parse_comparison_expression
(struct Sparser *parser) {
    struct Sast *node = AST(AST_COMPARE_EXPRESSION, 0, NULL);

    struct Sast *oper = Sparser_parse_additive_expression(parser);

    if (!oper) {
        Serror_parser("Expected additive expression", parser->lexer);
        return NULL;
    }

    Sast_add_operands(node, oper);

    int have_oper = 0;

    while (ptoken(parser)->type == EQUALS 
    || ptoken(parser)->type == NOT_EQUALS 
    || ptoken(parser)->type == BIGGER 
    || ptoken(parser)->type == SMALLER 
    || ptoken(parser)->type == BIGGER_EQUALS 
    || ptoken(parser)->type == SMALLER_EQUALS
    || ptoken(parser)->type == IS) {
        have_oper = 1;
        enum Stok_t op = ptoken(parser)->type;

        Sast_add_ops(node, op);

        pnext(parser);

        oper = Sparser_parse_additive_expression(parser);

        if (!oper) {
            Serror_parser("Expected additive expression", parser->lexer);
            return NULL;
        }

        Sast_add_operands(node, oper);
    }

    if (have_oper) return node;

    return oper;
}

struct Sast *
Sparser_parse_assignment
(struct Sparser *parser) {
    struct Sast *node = AST(AST_ASSIGNMENT, 0, NULL);

    node->var_name = ptoken(parser)->lexeme;

    pnext(parser);

    if (ptoken(parser)->type == ASSIGN) {
        pnext(parser);

        node->var_value = Sparser_parse(parser);

        Sast_set_line(parser->lexer, node->var_value);
        Serror_expected_expression(node->var_value);
        Sast_set_line(parser->lexer, node);

        return node;
    } else {
        Serror_parser("Expected assignment", parser->lexer);
    }   

    return NULL;
}

struct Sast *
Sparser_parse_function
(struct Sparser *parser) {
    struct Sast *node = AST(AST_FUNCTION_STATEMENT, 0, NULL);

    pnext(parser);
    if (ptoken(parser)->type == IDENTIFIER) {
        node->lexeme = ptoken(parser)->lexeme;

        pnext(parser);

        if (ptoken(parser)->type == LPAREN) {
            while (ptoken(parser)->type != RPAREN) {
                pnext(parser);
                if (ptoken(parser)->type == IDENTIFIER) {
                    Sast_set_para(node, ptoken(parser)->lexeme);
                    node->args_count++;
                    node->is_having_params = 1;
                    pnext(parser);
                    if (ptoken(parser)->type == COMMA) {
                        continue;
                    } else if (ptoken(parser)->type == RPAREN) {
                        break;
                    } else {
                        Serror_parser("Expected comma or closing parenthesis", parser->lexer);
                        return NULL;
                    }
                } else if (ptoken(parser)->type == RPAREN) {
                    break;
                } else {
                    Serror_parser("Expected identifier or closing parenthesis", parser->lexer);
                    return NULL;
                }
            }

            pnext(parser);
            if (ptoken(parser)->type == DO) {
                node->body = Sparser_parse_block(parser);;
            } else {
                Serror_parser("Expected 'do'", parser->lexer);
                return NULL;
            }

            Sast_set_line(parser->lexer, node);
            return node;
        } else {
            Serror_parser("Expected opening parenthesis", parser->lexer);
            return NULL;
        }
    } else {
        Serror_parser("Expected function name", parser->lexer);
        return NULL;
    }

    return NULL;
}

struct Sast *
Sparser_parse_function_call_identifier
(struct Sparser *parser) {
    struct Sast *node = AST(AST_FUNCTION_CALL_EXPRESSION, 0, NULL);

    if (ptoken(parser)->type == IDENTIFIER) {
        node->lexeme = ptoken(parser)->lexeme;

        pnext(parser);

        if (ptoken(parser)->type == LPAREN) {
            pnext(parser);

            while (ptoken(parser)->type != RPAREN) {
                struct Sast *expr = Sparser_parse(parser);
                
                Sast_set_line(parser->lexer, expr);
                Serror_expected_expression(expr);
                Sast_add_args(node, expr);

                if (ptoken(parser)->type == COMMA) {
                    pnext(parser);
                } else if (ptoken(parser)->type == RPAREN) {
                    break;
                } else {
                    Serror_parser("Expected comma or closing parenthesis", parser->lexer);
                    return NULL;
                }
            }

            Sast_set_line(parser->lexer, node);
            return node;
        } else {
            Serror_parser("Expected opening parenthesis", parser->lexer);
            return NULL;
        }
    } else {
        Serror_parser("Expected identifier", parser->lexer);
        return NULL;
    }

    Serror_parser("Expected identifier", parser->lexer);
    return NULL;
}   

struct Sast *
Sparser_parse_block
(struct Sparser *parser) {
    struct Sast *node = AST(AST_BLOCK, 0, NULL);
    Sast_set_line(parser->lexer, node);

    if (ptoken(parser)->type != DO) {
        Serror_parser("Expected 'do'", parser->lexer);
        return NULL;
    }

    pnext(parser);

    while (ptoken(parser)->type != END) {
        struct Sast *stmt = Sparser_parse(parser);
        
        Sast_add_block(node, stmt);

        node->block_size++;

        if (!stmt) {
            Serror_parser("Expected statement", parser->lexer);
            return NULL;
        }
    }

    pnext(parser);
    return node;
}


struct Sast *
Sparser_parse_ast_block_expression
(struct Sparser *parser) {
    struct Sast *node = AST(AST_BLOCK, 0, NULL);

    if (ptoken(parser)->type != DO) {
        Serror_parser("Expected 'do'", parser->lexer);
        return NULL;
    }

    pnext(parser);

    while (ptoken(parser)->type != END) {
        struct Sast *stmt = Sparser_parse(parser);
        Sast_set_line(parser->lexer, node);
        Sast_add_block(node, stmt);

        node->block_size++;

        if (!stmt) {
            Serror_parser("Expected statement", parser->lexer);
            return NULL;
        }
    }

    Sast_set_line(parser->lexer, node);
    // pnext(parser);

    return node;
}

struct Sast *
Sparser_parse_return
(struct Sparser *parser) {
    struct Sast *node = AST(AST_RETURN_STATEMENT, 0, NULL);

    pnext(parser);

    struct Sast *expr = Sparser_parse(parser);

    Sast_set_line(parser->lexer, expr);
    Serror_expected_expression(expr);

    node->ret_val = expr;

    return node;
}

struct Sast *
Sparser_parse_if
(struct Sparser *parser) {
    struct Sast *node = AST(AST_IF, 0, NULL);

    pnext(parser);

    struct Sast *expr = Sparser_parse(parser);

    Sast_set_line(parser->lexer, expr);
    Serror_expected_expression(expr);

    node->condition = expr;

    if (ptoken(parser)->type != THEN && ptoken(parser)->type != DO) {
        Serror_parser("Expected 'then' or 'do after expression", parser->lexer);
        return NULL;
    }

    node->if_body = Sparser_parse_if_block(parser);

    struct Sast* cur = node;

    while (ptoken(parser)->type == ELIF) {
        pnext(parser);

        struct Sast *expr = Sparser_parse(parser);

        Sast_set_line(parser->lexer, expr);
        Serror_expected_expression(expr);

        if (ptoken(parser)->type != THEN && ptoken(parser)->type != DO) {
            Serror_parser("Expected 'then' or 'do' after expression", parser->lexer);
            return NULL;
        }

        struct Sast *elif_block = Sparser_parse_if_block(parser);

        struct Sast *elif = AST(AST_IF, 0, NULL);

        elif->condition = expr;
        elif->if_body = elif_block;

        Sast_add_block(cur->else_body, elif);

        cur->else_body->block_size++;

        cur = elif;
    }

    if (ptoken(parser)->type == ELSE) {
        pnext(parser);
        while (ptoken(parser)->type != END) {
            struct Sast *stmt = Sparser_parse(parser);
            Sast_set_line(parser->lexer, cur);
            Sast_add_block(cur->else_body, stmt);

            cur->else_body->block_size++;

            if (!stmt) {
                Serror_parser("Expected statement", parser->lexer);
                return NULL;
            }
        }
    }

    Sast_set_line(parser->lexer, node);

    pnext(parser);

    return node;
}

struct Sast *
Sparser_parse_while
(struct Sparser *parser) {
    struct Sast *node = AST(AST_WHILE, 0, NULL);

    pnext(parser);

    struct Sast *expr = Sparser_parse(parser);

    Sast_set_line(parser->lexer, expr);
    
    Serror_expected_expression(expr);

    node->condition = expr;

    node->body = Sparser_parse_block(parser);;

    return node;
}

struct Sast *
Sparser_parse_if_block
(struct Sparser *parser) {
    struct Sast *node = AST(AST_BLOCK, 0, NULL);

    pnext(parser);

    while (ptoken(parser)->type != END && ptoken(parser)->type != ELSE && ptoken(parser)->type != ELIF) {
        struct Sast *stmt = Sparser_parse(parser);
        Sast_set_line(parser->lexer, node);
        Sast_add_block(node, stmt);

        node->block_size++;

        if (!stmt) {
            Serror_parser("Expected statement", parser->lexer);
            return NULL;
        }
    }

    return node;
}

struct Sast *
Sparser_parse_list
(struct Sparser *parser) {
    struct Sast *node = AST(AST_LIST, 0, NULL);

    pnext(parser);

    while (ptoken(parser)->type != RBRACKET) {
        struct Sast *stmt = Sparser_parse(parser);

        Sast_set_line(parser->lexer, node);
        Serror_expected_expression(stmt);
        Sast_add_element(node, stmt);

        if (ptoken(parser)->type == COMMA) {
            pnext(parser);
        } else if (ptoken(parser)->type == RBRACKET) {
            break;
        } else {
            Serror_parser("Expected comma or closing parenthesis", parser->lexer);
            return NULL;
        }
    }

    return node;
}

struct Sast *
Sparser_parse_extract(struct Sparser *parser, struct Sast *extract_obj) {
    struct Sast *node = AST(AST_EXTRACT, 0, NULL);

    node->extract_obj = extract_obj;

    pnext(parser); // eat the opening bracket

    if (ptoken(parser)->type != LBRACKET) {
        Serror_parser("Expected opening bracket '['", parser->lexer);
        return NULL;
    }

    pnext(parser);
    
    struct Sast *expr = Sparser_parse(parser);
    Sast_set_line(parser->lexer, expr);
    Serror_expected_expression(expr);

    node->extract_value = expr;

    if (ptoken(parser)->type != RBRACKET) {
        Serror_parser("Expected closing bracket ']'", parser->lexer);
        return NULL;
    }

    parser->next_token = Slexer_look_ahead(parser->lexer);

    if (parser->next_token->type == LBRACKET) {
        return Sparser_parse_extract(parser, node);
    } else if (parser->next_token->type == LPAREN) {
        return Sparser_parse_function_call(parser, node);
    } else if (parser->next_token->type == DOT) {
        return Sparser_parse_attribute_expression(parser, node);
    } else if (parser->next_token->type == SEMICOLON) {
        return node;
    } 

    return node;
}

struct Sast *
Sparser_parse_store_index
(struct Sparser *parser, struct Sast* extract_obj) {
    struct Sast *node = AST(AST_STORE_INDEX, 0, NULL);

    node->extract_obj = extract_obj;

    pnext(parser);

    struct Sast *expr = Sparser_parse(parser);
    Sast_set_line(parser->lexer, expr);
    Serror_expected_expression(expr);

    node->extract_value = expr;

    return node;
}

struct Sast *
Sparser_parse_for
(struct Sparser *parser) {
    struct Sast *node = AST(AST_FOR, 0, NULL);

    pnext(parser);

    if (ptoken(parser)->type != IDENTIFIER) {
        Serror_parser("Expected identifier", parser->lexer);
        return NULL;
    }

    node->lexeme = ptoken(parser)->lexeme;

    pnext(parser);

    if (ptoken(parser)->type != IN_T) {
        Serror_parser("Expected 'in'", parser->lexer);
        return NULL;
    }

    pnext(parser);

    struct Sast *expr = Sparser_parse(parser);
    Sast_set_line(parser->lexer, expr);
    Serror_expected_expression(expr);

    node->expr = expr;

    node->body = Sparser_parse_block(parser);

    return node;
}

struct Sast *
Sparser_parse_class
(struct Sparser *parser) {
    struct Sast *node = AST(AST_CLASS, 0, NULL);
    
    pnext(parser);

    if (ptoken(parser)->type != IDENTIFIER) {
        Serror_parser("Expected identifier", parser->lexer);
        return NULL;
    }

    node->lexeme = ptoken(parser)->lexeme;

    pnext(parser);

    if (ptoken(parser)->type == EXTENDS || ptoken(parser)->type == SHARED) {
        
        if (ptoken(parser)->type == EXTENDS) set_true(node->is_extends);
        else set_true(node->is_shared);

        while (ptoken(parser)->type != DO) {
            pnext(parser);
            if (ptoken(parser)->type == IDENTIFIER) {
                node->parent_class_names[node->parent_class_count] = ptoken(parser)->lexeme;
                node->parent_class_count++;
                pnext(parser);
                if (ptoken(parser)->type == COMMA) {
                    continue;
                } else if (ptoken(parser)->type == DO) {
                    break;
                } else {
                    Serror_parser("Expected comma or 'do'", parser->lexer);
                    return NULL;
                }
            } else if (ptoken(parser)->type == DO) {
                break;
            } else {
                Serror_parser("Expected identifier or 'do'", parser->lexer);
                return NULL;
            }
        }
    }

    node->body = Sparser_parse_block(parser);

    Sast_set_line(parser->lexer, node);
    return node;
}

struct Sast *
Sparser_parse_not
(struct Sparser *parser) {
    if (ptoken(parser)->type == NOT) {
        struct Sast *node = AST(AST_NOT_EXPRESSION, 0, NULL);

        pnext(parser);

        struct Sast *expr = Sparser_parse(parser);
        Serror_expected_expression(expr);

        node->expr = expr;

        return node;
    }

    return Sparser_parse_comparison_expression(parser);
}

struct Sast *
Sparser_parse_and
(struct Sparser *parser) {
    struct Sast *node = Sparser_parse_not(parser);

    while (ptoken(parser)->type == AND) {
        struct Sast *and_node = AST(AST_AND_EXPRESSION, 0, NULL);

        pnext(parser);

        struct Sast *expr = Sparser_parse(parser);
        Serror_expected_expression(expr);

        and_node->left = node;
        and_node->right = expr;

        node = and_node;

        return node;
    }

    return node;
}

struct Sast *
Sparser_parse_or
(struct Sparser *parser) {
    struct Sast *node = Sparser_parse_and(parser);

    while (ptoken(parser)->type == OR) {
        struct Sast *or_node = AST(AST_OR_EXPRESSION, 0, NULL);

        pnext(parser);

        struct Sast *expr = Sparser_parse(parser);
        Serror_expected_expression(expr);

        or_node->left = node;
        or_node->right = expr;

        node = or_node;

        return node;
    }

    return node;
}

struct Sast *
Sparser_parse_include
(struct Sparser *parser) {
    struct Sast *node = AST(AST_INCLUDE, 0, NULL);

    pnext(parser);

    if (ptoken(parser)->type != STRING) {
        Serror_parser("Expected file name, file name must be a string", parser->lexer);
        return NULL;
    }

    node->lexeme = ptoken(parser)->lexeme;

    pnext(parser);

    return node;
}

struct Sast *
Sparser_parse_anonymous_function
(struct Sparser *parser) {
    struct Sast *node = AST(AST_ANONYMOUS_FUNCTION, 0, NULL);

    pnext(parser);

    if (ptoken(parser)->type == LPAREN) {
        while (ptoken(parser)->type != RPAREN) {
            pnext(parser);
            if (ptoken(parser)->type == IDENTIFIER) {
                Sast_set_para(node, ptoken(parser)->lexeme);
                node->args_count++;
                node->is_having_params = 1;
                pnext(parser);
                if (ptoken(parser)->type == COMMA) {
                    continue;
                } else if (ptoken(parser)->type == RPAREN) {
                    break;
                } else {
                    Serror_parser("Expected comma or closing parenthesis", parser->lexer);
                    return NULL;
                }
            } else if (ptoken(parser)->type == RPAREN) {
                break;
            } else {
                Serror_parser("Expected identifier or closing parenthesis", parser->lexer);
                return NULL;
            }
        }

        pnext(parser);
        node->body = Sparser_parse_ast_block_expression(parser);
    } else {
        Serror_parser("Expected opening parenthesis", parser->lexer);
        return NULL;
    }

    return node;
}

struct Sast *
Sparser_parse_function_call
(struct Sparser *parser, struct Sast *expr) {
    struct Sast *node = AST(AST_FUNCTION_CALL_PRIMARY_EXPRESSION, 0, NULL);

    node->expr = expr;

    pnext(parser);

    if (ptoken(parser)->type == LPAREN) {
        pnext(parser);

        while (ptoken(parser)->type != RPAREN) {
            struct Sast *expr = Sparser_parse(parser);
            
            Serror_expected_expression(expr);
            Sast_add_args(node, expr);

            if (ptoken(parser)->type == COMMA) {
                pnext(parser);
            } else if (ptoken(parser)->type == RPAREN) {
                break;
            } else {
                Serror_parser("Expected comma or closing parenthesis", parser->lexer);
                return NULL;
            }
        }

        parser->next_token = Slexer_look_ahead(parser->lexer);

        if (parser->next_token->type == LPAREN) {
            return Sparser_parse_function_call(parser, node);  
        } else if (parser->next_token->type == LBRACKET) {
            return Sparser_parse_extract(parser, node);
        } else if (parser->next_token->type == DOT) {
            return Sparser_parse_attribute_expression(parser, node);
        } else if (parser->next_token->type == SEMICOLON) {
            Sast_set_line(parser->lexer, node);
            return node;
        }

        Sast_set_line(parser->lexer, node);
        return node;
    } else {
        Serror_parser("Expected opening parenthesis", parser->lexer);
        return NULL;
    }

    Serror_parser("Expected identifier", parser->lexer);
    return NULL;
}

struct Sast *
Sparser_parse_loop
(struct Sparser *parser) {  
    struct Sast *node = AST(AST_LOOP, 0, NULL);

    pnext(parser);

    if (ptoken(parser)->type == DO) {
        node->body = Sparser_parse_block(parser);
        return node;
    } else {
        struct Sast *times = Sparser_parse(parser);
        
        Sast_set_line(parser->lexer, times);
        Serror_expected_expression(times);

        node->times = times;
        node->is_times = 1;

        if (ptoken(parser)->type == TIMES) {
            pnext(parser);
            node->body = Sparser_parse_block(parser); 
            return node;
        } else {
            Serror_parser("Expected 'times'", parser->lexer);
            return NULL;
        }
    }

    return node;
}

struct Sast *
Sparser_parse_variable_list
(struct Sparser *parser) {
    struct Sast *node = AST(AST_VAR_LIST, 0, NULL);

    while (ptoken(parser)->type != ASSIGN) {
        if (ptoken(parser)->type == IDENTIFIER) {
            Sast_add_var_list_name(node, ptoken(parser)->lexeme);
            pnext(parser);
            if (ptoken(parser)->type == COMMA) {
                pnext(parser);
            } else if (ptoken(parser)->type == ASSIGN) {
                break;
            } else {
                Serror_parser("Expected comma or assignment", parser->lexer);
                return NULL;
            }
        } else {
            Serror_parser("Expected identifier", parser->lexer);
            return NULL;
        }
    }

    pnext(parser);

    while (1) {
        struct Sast *expr = Sparser_parse(parser);

        Sast_set_line(parser->lexer, expr);
        Serror_expected_expression(expr);

        Sast_add_var_list_value(node, expr);

        if (ptoken(parser)->type == COMMA) {
            pnext(parser);
        } else {
            break;
        }
    }

    if (node->var_list_size != node->var_list_values_size) {
        Serror_parser("Expected same number of variables and values", parser->lexer);
        return NULL;
    }

    Sast_assign_var_list_value(node);
    Sast_set_line(parser->lexer, node);

    return node;
}

struct Sast *
Sparser_parse_import
(struct Sparser *parser) {
    struct Sast *node = AST(AST_IMPORT, 0, NULL);

    pnext(parser);

    if (ptoken(parser)->type != STRING) {
        Serror_parser("Expected file name, file name must be a string", parser->lexer);
        return NULL;
    }

    node->lexeme = ptoken(parser)->lexeme;

    pnext(parser);

    return node;
}

struct Sast *
Sparser_parse_attribute
(struct Sparser *parser, struct Sast* object, struct Sast* attribute) {
    struct Sast *node = AST(AST_ATTRIBUTE_EXPRESSION, 0, NULL);
    node->expr = object;
    node->attribute = attribute;
    return node;
}

struct Sast *
Sparser_parse_attribute_expression
(struct Sparser *parser, struct Sast* object) {
    struct Sast *node = AST(AST_ATTRIBUTE_EXPRESSION, 0, NULL);
    node->target = object;

    pnext(parser);

    if (ptoken(parser)->type != DOT) {
        Serror_parser("Expected '.'", parser->lexer);
        return NULL;
    }

    pnext(parser);

    if (ptoken(parser)->type == IDENTIFIER) {
        node->attr_name = ptoken(parser)->lexeme;
    } else {
        Serror_parser("Expected identifier", parser->lexer);
        return NULL;
    }

    parser->next_token = Slexer_look_ahead(parser->lexer);

    if (parser->next_token->type == LPAREN) {
        return Sparser_parse_function_call(parser, node);
    } else if (parser->next_token->type == LBRACKET) {
        return Sparser_parse_extract(parser, node);
    } else if (parser->next_token->type == DOT) {
        return Sparser_parse_attribute_expression(parser, node);
    }

    Sast_set_line(parser->lexer, node);
    return node;
}

struct Sast *
Sparser_parse_store_attribute
(struct Sparser *parser, struct Sast* object) {
    struct Sast *node = AST(AST_STORE_ATTRIBUTE, 0, NULL);
    
    pnext(parser);
    struct Sast *value = Sparser_parse(parser);

    Sast_set_line(parser->lexer, value);
    Serror_expected_expression(value);

    node->expr = object;
    node->attribute = value;

    Sast_set_line(parser->lexer, node);

    return node;
}

struct Sast*
Sast_parse_from_string(char *str) {
    struct Slexer *lexer = Slexer_init(str);
    struct Sparser *parser = Sparser_init(lexer);
    pnext(parser);
    struct Sast *ast = Sparser_parse(parser);
    return ast;
}

struct Sast *
Sparser_parse_map
(struct Sparser *parser) {
    struct Sast* node = Sast_init(AST_MAP_EXPRESSION, 0, NULL);

    pnext(parser);

    while (ptoken(parser)->type != RBRACE) {
        struct Sast* key = Sparser_parse(parser);
        Sast_set_line(parser->lexer, key);
        Serror_expected_expression(key);

        if (ptoken(parser)->type != COLON) {
            Serror_parser("Expected ':'", parser->lexer);
        }

        pnext(parser);

        struct Sast* value = Sparser_parse(parser);
        Sast_set_line(parser->lexer, value);
        Serror_expected_expression(value);
        
        Sast_add_key(node, key);
        Sast_add_value(node, value);

        node->map_size++;

        if (ptoken(parser)->type == COMMA)
            pnext(parser);
        else if (ptoken(parser)->type == RBRACE)
            break;
        else
            Serror_parser("Expected ',' or '}'", parser->lexer);
    }

    return node;
}           
