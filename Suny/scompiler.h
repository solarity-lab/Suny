#ifndef SCOMPILER_H
#define SCOMPILER_H

#include "stable.h"
#include "scode.h"
#include "sast.h"
#include "sparser.h"
#include "sobj.h"
#include "score.h"

SUNY_API struct Scode* 
Scompiler_compile
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_program
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_loop
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_block
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_local_block
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_function
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_binary_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_literal
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_identifier
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_closure_identifier
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_assignment
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_print
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_function_call
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_return
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_comparison
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_if
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_while
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_function
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_string
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_break
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_continue
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_list
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_for
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_class
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_or
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_and
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_not
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_anonymous_function
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_function_call_primary
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_include
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_import
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_attribute
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_store_attribute
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_null_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_extract_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_ast_store_index
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table);

SUNY_API struct Scode* 
Scompiler_compile_block
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size);

SUNY_API struct Scode* 
Scompiler_compile_body
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size);

SUNY_API struct Scode* 
Scompiler_compile_local_body
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size);

SUNY_API struct Scode* 
Scompiler_compile_function_body
(struct ScompilerUnit *compiler, struct Sast **block, struct Stable *table, int block_size, char **args, int args_size);

SUNY_API struct Scode* 
Scompiler_compile_function_body_expression
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table, int args_count, char **args);

SUNY_API struct Scode* 
Scompiler_compile_ast_function_body
(struct ScompilerUnit *compiler, struct Sast *ast, struct Stable *table, int args_count, char **args);

#endif // SCOMPILER_H