/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* Copy the first part of user declarations.  */
#line 1 "gcode_parser.y" /* yacc.c:339  */

// G-code parser implementation
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gcode_parser.h"
#include "gcode_lexer.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static void yyerror(const GCodeLocation* location, GCodeParser* parser,
                    const char* msg);

struct GCodeParser {
    void* context;
    GCodeLexer* lexer;
    bool in_expr;
    struct yypstate* yyps;
    GCodeError* error;
    GCodeLocation location;

    bool (*statement)(void*, GCodeStatementNode*);
};

static inline GCodeNode* newop1(
    gcode_operator_type_t type,
    GCodeNode* a)
{
    if (!a)
        return NULL;
    return gcode_operator_new(type, a);
}

static inline GCodeNode* newop2(
    gcode_operator_type_t type,
    GCodeNode* a,
    GCodeNode* b)
{
    if (!a || !b) {
        gcode_node_delete(a);
        gcode_node_delete(b);
        return NULL;
    }
    GCodeNode* op = gcode_operator_new(type, a);
    gcode_add_next(a, b);
    return op;
}

static inline GCodeNode* newop3(
    gcode_operator_type_t type,
    GCodeNode* a,
    GCodeNode* b,
    GCodeNode* c)
{
    if (!a || !b || !c) {
        gcode_node_delete(a);
        gcode_node_delete(b);
        gcode_node_delete(c);
        return NULL;
    }
    if (!b) {
        gcode_node_delete(a);
        return NULL;
    }
    GCodeNode* op = newop2(type, a, b);
    gcode_add_next(b, c);
    return op;
}

static inline bool add_statement(GCodeParser* parser, const char* command,
                                 GCodeNode* children)
{
    GCodeStatementNode* statement =
        (GCodeStatementNode*)gcode_statement_new(command, children);
    if (!statement)
        return false;
    parser->statement(parser->context, statement);
    return true;
}

static void out_of_memory(GCodeParser* parser) {
    EMIT_ERROR(parser, "Out of memory (allocating parse node)");
}

#define OOM(val) if (!(val)) { \
    out_of_memory(parser); \
    YYERROR; \
}

typedef GCodeLocation YYLTYPE;
#define YYLTYPE_IS_DECLARED

static void yyerror(const GCodeLocation* location, GCodeParser* parser,
                    const char* msg)
{
    // Aw Bison...  Our ERROR token is the only way to push an error into the
    // parser, but then we can only skip it using ghetto string matching
    if (strstr(msg, "syntax error, unexpected ERROR"))
        return;
    gcode_error_set_location(parser->error, location);
    EMIT_ERROR(parser, "G-Code parse error: %s", msg);
}


#line 177 "../gcode_parser.generated.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_IDENTIFIER = 258,
    TOK_STRING = 259,
    TOK_INTEGER = 260,
    TOK_FLOAT = 261,
    TOK_ERROR = 262,
    TOK_SPECIAL = 263,
    TOK_END_OF_STATEMENT = 264,
    TOK_OR = 265,
    TOK_AND = 266,
    TOK_EQUAL = 267,
    TOK_CONCAT = 268,
    TOK_PLUS = 269,
    TOK_MINUS = 270,
    TOK_MODULUS = 271,
    TOK_POWER = 272,
    TOK_TIMES = 273,
    TOK_DIVIDE = 274,
    TOK_LT = 275,
    TOK_GT = 276,
    TOK_LTE = 277,
    TOK_GTE = 278,
    TOK_NOT = 279,
    TOK_IF = 280,
    TOK_ELSE = 281,
    TOK_DOT = 282,
    TOK_COMMA = 283,
    TOK_LPAREN = 284,
    TOK_RPAREN = 285,
    TOK_NAN = 286,
    TOK_INF = 287,
    TOK_TRUE = 288,
    TOK_FALSE = 289,
    TOK_LBRACKET = 290,
    TOK_RBRACKET = 291,
    TOK_LBRACE = 292,
    TOK_RBRACE = 293,
    TOK_STR_CAST = 294,
    TOK_INT_CAST = 295,
    TOK_FLOAT_CAST = 296,
    TOK_BOOL_CAST = 297,
    TOK_UNARY = 298,
    TOK_BRIDGE = 299
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 120 "gcode_parser.y" /* yacc.c:355  */

    int keyword;
    int64_t int_value;
    double float_value;
    char* str;
    GCodeNode* node;

#line 267 "../gcode_parser.generated.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, GCodeParser* parser);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);



/* Copy the second part of user declarations.  */

#line 307 "../gcode_parser.generated.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   462

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  51
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  98

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   206,   206,   207,   211,   213,   217,   218,   222,   223,
     229,   230,   231,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     264,   265,   267,   268,   269,   270,   271,   276,   280,   281,
     285,   286
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING", "INTEGER",
  "FLOAT", "ERROR", "SPECIAL", "END_OF_STATEMENT", "\"or\"", "\"and\"",
  "\"==\"", "\"~\"", "\"+\"", "\"-\"", "\"%\"", "\"**\"", "\"*\"", "\"/\"",
  "\"<\"", "\">\"", "\"<=\"", "\">=\"", "\"not\"", "\"if\"", "\"else\"",
  "\".\"", "\",\"", "\"(\"", "\")\"", "\"nan\"", "\"inf\"", "\"true\"",
  "\"false\"", "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\"str\"", "\"int\"",
  "\"float\"", "\"boolean\"", "UNARY", "BRIDGE", "$accept", "statements",
  "statement", "args", "keyvals", "arg", "expr", "string", "exprs",
  "expr_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299
};
# endif

#define YYPACT_NINF -65

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-65)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -65,    66,   -65,   -65,    15,   -65,   -65,    59,   -65,    -6,
      13,   -65,   -27,   -65,   -65,    59,    59,    59,    59,   -65,
     -65,   -65,   -65,   -15,   -13,   -11,    -8,    92,   -65,   -65,
     -65,    15,    16,    59,   -23,   -23,   -23,   148,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,     3,    59,   -65,
     -65,   -65,    16,   174,    -5,   -65,   -65,   200,   226,   252,
     278,   329,   353,   376,   398,   418,   418,   427,   -23,   427,
     427,   -12,   -12,   -12,   -12,   304,   -65,   121,    59,   -65,
     -65,   -65,   -65,   -65,    59,   -65,   -65,   -12
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     5,     8,     3,    47,     0,     4,     0,
       0,    10,    15,    16,    17,     0,     0,     0,     0,    21,
      20,    18,    19,     0,     0,     0,     0,     0,    14,     7,
       6,     0,     8,    48,    24,    23,    22,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    11,
      12,     9,     0,    50,     0,    49,    13,     0,     0,     0,
       0,    32,    31,    38,    37,    25,    26,    29,    30,    27,
      28,    33,    34,    36,    35,     0,    39,     0,     0,    46,
      42,    43,    45,    44,     0,    40,    51,    41
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -65,   -65,   -65,   -65,    -2,    23,    -7,    -3,   -65,   -64
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     5,     8,     9,    32,    63,    28,    64,    65
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      27,    11,    33,    29,    57,    49,    86,    11,    34,    35,
      36,    37,    58,    56,    38,    57,    39,     6,    40,     6,
       6,    41,    30,    58,    96,    89,     0,    10,    11,    11,
      61,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
       7,    87,     7,     7,    60,    62,     0,    31,     0,    11,
      31,     0,    12,     6,    13,    14,     2,     3,     0,     4,
       0,     0,     0,    15,    16,     0,     0,     0,     0,     0,
       0,     0,     0,    17,     0,     0,     0,    97,    18,     0,
      19,    20,    21,    22,     0,     0,     0,     0,    23,    24,
      25,    26,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,     0,    56,     0,    57,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
      59,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,     0,    57,     0,
       0,     0,     0,     0,     0,     0,    58,    95,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,     0,    56,     0,    57,     0,     0,    66,     0,
       0,     0,     0,    58,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,     0,    56,
       0,    57,    88,     0,     0,     0,     0,     0,     0,    58,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,     0,    56,     0,    57,     0,     0,
      90,     0,     0,     0,     0,    58,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
       0,    56,     0,    57,     0,     0,    91,     0,     0,     0,
       0,    58,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,     0,    56,     0,    57,
       0,     0,    92,     0,     0,     0,     0,    58,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,     0,    56,     0,    57,     0,     0,    93,     0,
       0,     0,     0,    58,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,     0,    56,
      94,    57,     0,     0,     0,     0,     0,     0,     0,    58,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,     0,    56,     0,    57,     0,     0,     0,
       0,     0,     0,     0,    58,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,     0,    56,     0,
      57,     0,     0,     0,     0,     0,     0,     0,    58,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
       0,    56,     0,    57,     0,     0,     0,     0,     0,     0,
       0,    58,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,     0,    56,     0,    57,     0,     0,     0,     0,
       0,     0,     0,    58,    48,    49,    50,    51,    52,    53,
      54,    55,     0,    56,    49,    57,     0,    52,    53,    54,
      55,     0,    56,    58,    57,     0,     0,     0,     0,     0,
       0,     0,    58
};

static const yytype_int8 yycheck[] =
{
       7,     4,    29,     9,    27,    17,     3,    10,    15,    16,
      17,    18,    35,    25,    29,    27,    29,     4,    29,     4,
       4,    29,     9,    35,    88,    30,    -1,     4,    31,    32,
      32,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      37,    58,    37,    37,    31,    32,    -1,    44,    -1,    62,
      44,    -1,     3,     4,     5,     6,     0,     1,    -1,     3,
      -1,    -1,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    94,    29,    -1,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    39,    40,
      41,    42,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      38,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,    25,    -1,    27,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    -1,    25,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    -1,    25,    -1,    27,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    35,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      -1,    25,    -1,    27,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    -1,    25,    -1,    27,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,    25,    -1,    27,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,    25,    17,    27,    -1,    20,    21,    22,
      23,    -1,    25,    35,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    46,     0,     1,     3,    47,     4,    37,    48,    49,
      50,    52,     3,     5,     6,    14,    15,    24,    29,    31,
      32,    33,    34,    39,    40,    41,    42,    51,    52,     9,
       9,    44,    50,    29,    51,    51,    51,    51,    29,    29,
      29,    29,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    25,    27,    35,    38,
      50,    49,    50,    51,    53,    54,    30,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,     3,    51,    28,    30,
      30,    30,    30,    30,    26,    36,    54,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    46,    47,    47,    48,    48,    49,    49,
      50,    50,    50,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    52,    53,    53,
      54,    54
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     2,     2,     0,     3,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     5,     4,     4,     4,     4,     4,     1,     0,     1,
       1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, parser, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, parser); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, GCodeParser* parser)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (parser);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, GCodeParser* parser)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, parser);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, GCodeParser* parser)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , parser);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, parser); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, GCodeParser* parser)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (parser);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 3: /* IDENTIFIER  */
#line 128 "gcode_parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).str)); }
#line 1317 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 4: /* STRING  */
#line 128 "gcode_parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).str)); }
#line 1323 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 48: /* args  */
#line 129 "gcode_parser.y" /* yacc.c:1257  */
      { gcode_node_delete(((*yyvaluep).node)); }
#line 1329 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 49: /* keyvals  */
#line 129 "gcode_parser.y" /* yacc.c:1257  */
      { gcode_node_delete(((*yyvaluep).node)); }
#line 1335 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 50: /* arg  */
#line 129 "gcode_parser.y" /* yacc.c:1257  */
      { gcode_node_delete(((*yyvaluep).node)); }
#line 1341 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 51: /* expr  */
#line 129 "gcode_parser.y" /* yacc.c:1257  */
      { gcode_node_delete(((*yyvaluep).node)); }
#line 1347 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 52: /* string  */
#line 129 "gcode_parser.y" /* yacc.c:1257  */
      { gcode_node_delete(((*yyvaluep).node)); }
#line 1353 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 53: /* exprs  */
#line 129 "gcode_parser.y" /* yacc.c:1257  */
      { gcode_node_delete(((*yyvaluep).node)); }
#line 1359 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;

    case 54: /* expr_list  */
#line 129 "gcode_parser.y" /* yacc.c:1257  */
      { gcode_node_delete(((*yyvaluep).node)); }
#line 1365 "../gcode_parser.generated.c" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}



struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;
    /* Used to determine if this is the first time this instance has
       been used.  */
    int yynew;
  };

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = (yypstate *) malloc (sizeof *yyps);
  if (!yyps)
    return YY_NULLPTR;
  yyps->yynew = 1;
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
#ifndef yyoverflow
  /* If the stack was reallocated but the parse did not complete, then the
     stack still needs to be freed.  */
  if (!yyps->yynew && yyps->yyss != yyps->yyssa)
    YYSTACK_FREE (yyps->yyss);
#endif
  free (yyps);
}

#define yynerrs yyps->yynerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yylsa yyps->yylsa
#define yyls yyps->yyls
#define yylsp yyps->yylsp
#define yyerror_range yyps->yyerror_range
#define yystacksize yyps->yystacksize


/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, GCodeParser* parser)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  if (!yyps->yynew)
    {
      yyn = yypact[yystate];
      goto yyread_pushed_token;
    }

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = *yypushed_loc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
      if (yypushed_loc)
        yylloc = *yypushed_loc;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 211 "gcode_parser.y" /* yacc.c:1646  */
    { OOM(add_statement(parser, (yyvsp[-1].str), (yyvsp[0].node)));
                              free((yyvsp[-1].str)); }
#line 1723 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 6:
#line 217 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = gcode_add_next(gcode_str_new("*"), (yyvsp[-1].node)); }
#line 1729 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 7:
#line 218 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[-1].node); }
#line 1735 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 8:
#line 222 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 1741 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 9:
#line 224 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = gcode_add_next((yyvsp[-2].node), (yyvsp[-1].node));
                              gcode_add_next((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1748 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 11:
#line 230 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[-1].node); }
#line 1754 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 12:
#line 231 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_CONCAT, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1760 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 13:
#line 235 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[-1].node); }
#line 1766 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 15:
#line 237 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = gcode_parameter_new((yyvsp[0].str)); free((yyvsp[0].str)); OOM((yyval.node)) }
#line 1772 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 16:
#line 238 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = gcode_int_new((yyvsp[0].int_value))); }
#line 1778 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 17:
#line 239 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = gcode_float_new((yyvsp[0].float_value))); }
#line 1784 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 18:
#line 240 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = gcode_bool_new(true)); }
#line 1790 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 19:
#line 241 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = gcode_bool_new(false)); }
#line 1796 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 20:
#line 242 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = gcode_float_new(INFINITY)); }
#line 1802 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 21:
#line 243 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = gcode_float_new(NAN)); }
#line 1808 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 22:
#line 244 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop1(GCODE_NOT, (yyvsp[0].node))); }
#line 1814 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 23:
#line 245 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop1(GCODE_NEGATE, (yyvsp[0].node))); }
#line 1820 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 24:
#line 246 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = (yyvsp[0].node)); }
#line 1826 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 25:
#line 247 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_ADD, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1832 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 26:
#line 248 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_SUBTRACT, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1838 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 27:
#line 249 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_MULTIPLY, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1844 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 28:
#line 250 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_DIVIDE, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1850 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 29:
#line 251 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_MODULUS, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1856 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 30:
#line 252 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_POWER, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1862 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 31:
#line 253 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_AND, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1868 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 32:
#line 254 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_OR, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1874 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 33:
#line 255 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_LT, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1880 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 34:
#line 256 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_GT, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1886 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 35:
#line 257 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_GTE, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1892 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 36:
#line 258 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_LTE, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1898 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 37:
#line 259 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_CONCAT, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1904 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 38:
#line 260 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_EQUALS, (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1910 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 39:
#line 261 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = newop2(GCODE_LOOKUP, (yyvsp[-2].node), gcode_str_new((yyvsp[0].str)));
                              free((yyvsp[0].str));
                              OOM((yyval.node)); }
#line 1918 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 40:
#line 264 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop2(GCODE_LOOKUP, (yyvsp[-3].node), (yyvsp[-1].node))); }
#line 1924 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 41:
#line 266 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop3(GCODE_IFELSE, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node))); }
#line 1930 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 42:
#line 267 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop1(GCODE_STR_CAST, (yyvsp[-1].node)))}
#line 1936 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 43:
#line 268 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop1(GCODE_INT_CAST, (yyvsp[-1].node)))}
#line 1942 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 44:
#line 269 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop1(GCODE_BOOL_CAST, (yyvsp[-1].node)))}
#line 1948 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 45:
#line 270 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = newop1(GCODE_FLOAT_CAST, (yyvsp[-1].node)))}
#line 1954 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 46:
#line 272 "gcode_parser.y" /* yacc.c:1646  */
    { OOM((yyval.node) = gcode_function_new((yyvsp[-3].str), (yyvsp[-1].node))); }
#line 1960 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 47:
#line 276 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = gcode_str_new((yyvsp[0].str)); free((yyvsp[0].str)); OOM((yyval.node)); }
#line 1966 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 48:
#line 280 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 1972 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;

  case 51:
#line 286 "gcode_parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[-2].node); gcode_add_next((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1978 "../gcode_parser.generated.c" /* yacc.c:1646  */
    break;


#line 1982 "../gcode_parser.generated.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, parser, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, parser, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, parser);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, parser);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, parser, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, parser);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, parser);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  yyps->yynew = 1;

yypushreturn:
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 289 "gcode_parser.y" /* yacc.c:1906  */


static void push_error(GCodeParser* parser) {
    // Our lexer generates errors.  With a push parser, the only way to convey
    // the error to Bison is to give it a token with no semantic meaning.  This
    // in turn triggers our error state which skips the remainder of the
    // statement.
    yypush_parse(parser->yyps, TOK_ERROR, NULL, &parser->location, parser);
}

static bool lex_keyword(void* context, gcode_keyword_t id) {
    GCodeParser* parser = context;

    yypush_parse(parser->yyps, id, NULL, &parser->location, parser);

    return true;
}

static bool lex_bridge(void* context) {
    GCodeParser* parser = context;
    yypush_parse(parser->yyps, TOK_BRIDGE, NULL, &parser->location, parser);
    return true;
}

static bool lex_end_of_statement(void* context) {
    GCodeParser* parser = context;
    yypush_parse(parser->yyps, TOK_END_OF_STATEMENT, NULL, &parser->location, parser);
    return true;
}

static inline bool push_string(void* context, int id, const char* value) {
    GCodeParser* parser = context;

    YYSTYPE yys = { .str = strdup(value) };
    if (!yys.str) {
        EMIT_ERROR(parser, "Out of memory (push_string)");
        push_error(parser);
        return false;
    }

    yypush_parse(parser->yyps, id, &yys, &parser->location, parser);

    return true;
}

static bool lex_identifier(void* context, const char* name) {
    push_string(context, TOK_IDENTIFIER, name);
    return true;
}

static bool lex_string_literal(void* context, const char* value) {
    push_string(context, TOK_STRING, value);
    return true;
}

static bool lex_int_literal(void* context, int64_t value) {
    GCodeParser* parser = context;

    YYSTYPE yys = { .int_value = value };
    yypush_parse(parser->yyps, TOK_INTEGER, &yys, &parser->location, parser);

    return true;
}

static bool lex_float_literal(void* context, double value) {
    GCodeParser* parser = context;

    YYSTYPE yys = { .float_value = value };
    yypush_parse(parser->yyps, TOK_FLOAT, &yys, &parser->location, parser);

    return true;
}

void lex_error(void* context, const GCodeError* error) {
    GCodeParser* parser = (GCodeParser*)context;
    gcode_error_forward(parser->error, error);
    push_error(parser);
}

GCodeParser* gcode_parser_new(
    void* context,
    void (*error)(void*, const GCodeError* error),
    bool (*statement)(void*, GCodeStatementNode*))
{
    GCodeParser* parser = malloc(sizeof(GCodeParser));
    if (!parser)
        return NULL;

    parser->error = gcode_error_new(context, error);
    if (!error) {
        free(parser);
        return NULL;
    }

    parser->context = context;
    parser->in_expr = false;

    parser->statement = statement;

    parser->lexer = gcode_lexer_new(
        parser,
        &parser->location,
        lex_error,
        lex_keyword,
        lex_identifier,
        lex_string_literal,
        lex_int_literal,
        lex_float_literal,
        lex_bridge,
        lex_end_of_statement
    );
    if (!parser->lexer) {
        gcode_error_delete(parser->error);
        free(parser);
        return NULL;
    }

    parser->yyps = yypstate_new();
    if (!parser->yyps) {
        gcode_error_delete(parser->error);
        gcode_lexer_delete(parser->lexer);
        free(parser);
        return NULL;
    }

    return parser;
}

void gcode_parser_parse(GCodeParser* parser, const char* buffer,
                        size_t length)
{
    gcode_lexer_scan(parser->lexer, buffer, length);
}

void gcode_parser_finish(GCodeParser* parser) {
    gcode_lexer_finish(parser->lexer);
}

void gcode_parser_delete(GCodeParser* parser) {
    if (!parser)
        return;
    gcode_lexer_delete(parser->lexer);
    gcode_error_delete(parser->error);
    if (parser->yyps)
        yypstate_delete(parser->yyps);
    free(parser);
}
