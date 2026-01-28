#ifndef OPCODE_H
#define OPCODE_H

#define PUSH_FLOAT                      0x20
#define PUSH_STRING                     0x21
#define PUSH_TRUE                       0x22
#define PUSH_FALSE                      0x23
#define LOAD_NULL                       0x24
#define BINARY_ADD                      0x25
#define BINARY_SUB                      0x26
#define BINARY_MUL                      0x27
#define BINARY_DIV                      0x28
#define BINARY_BIGGER                   0x29
#define BINARY_SMALLER                  0x2A
#define BINARY_EQUAL                    0x2B
#define BINARY_NOT_EQUAL                0x2C
#define BINARY_BIGGER_EQUAL             0x2D
#define BINARY_SMALLER_EQUAL            0x2E
#define AND_LOG                         0x2F
#define OR_LOG                          0x30
#define NOT_LOG                         0x31
#define POP_TOP                         0x32
#define PRINT                           0x33
#define LEN_OF                          0x34
#define BUILD_LIST                      0x35
#define LOAD_GLOBAL                     0x36
#define STORE_GLOBAL                    0x37
#define LOAD_LOCAL                      0x38
#define STORE_LOCAL                     0x39

#define LOAD_ITEM                       0x3A
#define STORE_ITEM                      0x3B
#define LOAD_ATTR                       0x3C
#define STORE_ATTR                      0x3D
#define LOAD_METHOD                     0x3E
#define MAKE_FUNCTION                   0x3F
#define START_FUNCTION                  0x40
#define END_FUNCTION                    0x41
#define MAKE_ARGS                       0x42
#define FUNCTION_CALL                   0x43
#define FUNCTION_CALL_VARARGS           0x44
#define RETURN_TOP                      0x45
#define MAKE_CLOSURE                    0x46
#define END_CLOSURE                     0x47
#define LOAD_CLOSURE                    0x48
#define STORE_CLOSURE                   0x49
#define JUMP                            0x4A
#define JUMP_FORWARD                    0x4B
#define JUMP_BACKWARD                   0x4C
#define JUMP_IF_TRUE                    0x4D
#define JUMP_IF_FALSE                   0x4E
#define POP_JUMP_IF_TRUE                0x4F
#define POP_JUMP_IF_FALSE               0x50
#define JUMP_TO_N_TIMES                 0x51
#define LOOP_PREP                       0x52
#define LOOP_STEP                       0x53

#define CLASS_BEGIN                     0x54
#define CLASS_END                       0x55
#define SET_SUPER_CLASS                 0x56

#define START_BLOCK                     0x57
#define END_BLOCK                       0x58

#define PROGRAM_START                   0x59
#define PROGRAM_END                     0x5A
#define EXIT_PROGRAM                    0x5B
#define STOP_PROGRAM                    0x5C
#define SKIP                            0x5D

#define LOAD_TRUE                       0x5E
#define LOAD_FALSE                      0x5F
#define JUMP_TO                         0x60
#define ADD_LABEL                       0x61
#define BINARY_MOD                      0x62

#define STORE_MEMBER                    0x63
#define LOAD_MEMBER                     0x64
#define SET_SHARED_CLASS                0x65

#define IS_LOG                          0x66

#endif // OPCODE_H
