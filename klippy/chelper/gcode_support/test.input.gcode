; This input file attempts to exhaustively cover G-Code C code


; Lines without statements
; Comment
 ; Comment
N1
 N1
N12
 N12
N1 ; Comment
 N1 ; Comment
N12 ; Comment
 N12 ; Comment
n1
 n1
n12
 n12
n1 ; Comment
 n1 ; Comment
n12 ; Comment
 n12 ; Comment


; Basic instructions

M18
G1 X0
G1 X0 Y0
 M18
 G1 X0
 G1 X0 Y0
N1 M18
N1 G1 X0
N1 G1 X0 Y0
 N1 M18
 N1 G1 X0
 N1 G1 X0 Y0
m18
g1 x0
g1 x0 y0
 m18
 g1 x0
 g1 x0 y0
n1 m18
n1 g1 x0
n1 g1 x0 y0
 n1 m18
 n1 g1 x0
 n1 g1 x0 y0
M18 ; Comment
G1 X0 ; Comment
G1 X0 Y0 ; Comment
 M18 ; Comment
 G1 X0 ; Comment
 G1 X0 Y0 ; Comment
N1 M18 ; Comment
N1 G1 X0 ; Comment
N1 G1 X0 Y0 ; Comment
 N1 M18 ; Comment
 N1 G1 X0 ; Comment
 N1 G1 X0 Y0 ; Comment
✈
🍺


; Argument formats

; Traditional
x1 b={3+4}
x1 a12 bcd e=123
X1 A12 Bcd E=123
X1 a{1 + 2} b={3 + 4} {"c"}5 {"d"}{6}
x1 a
x1 a; comment
x1 a ; comment
x1 {"a"}
x1 {"a"}; comment
x1 {"a"} ; comment

; Extended
extended foo=bar biz= baz dee =dum wee = wum
extended "foo"=bar "biz"= baz "dee" =dum "wee" = wum
extended "f""oo"=bar "b""iz"= baz "d""ee" =dum "w""ee" = wum
extended foo="b""ar" biz= "b""az" dee ="d""um" wee = "w""um"
extended {"foo"}=bar {"biz"}= baz {"dee"} =dum {"wee"} = wum
extended {"f"}{"oo"}=bar {"b"}{"iz"}= baz {"d"}{"ee"} =dum {"w"}{"ee"} = wum
extended foo={"b"}{"ar"} biz= {"b"}{"az"} dee ={"d"}{"um"} wee = {"w"}{"um"}
extended "f"{"oo"}=bar "b"{"iz"}= baz "d"{"ee"} =dum "w"{"ee"} = wum
extended foo="b"{"ar"} biz= "b"{"az"} dee ="d"{"um"} wee = "w"{"um"}
extended {"f"}"oo"=bar {"b"}"iz"= baz {"d"}"ee" =dum {"w"}"ee" = wum
extended foo={"b"}"ar" biz= {"b"}"az" dee ={"d"}"um" wee = {"w"}"um"

a foo = bar

; Raw
m117 Anything should be allowed here; there aren't even comments
echo Although {"expressions"} are OK


; Single literal expressions

; ints
int a={0} b={0b0} c={00} d={0x0}
int a={10} b={0b10} c={0x10} d={010}
int a={1} b={+1} c={-1} d={++1} e={--1} f={10} g={+10} h={-10}

; Decimal floats
dec_f a={0.0} b={1.0} c={1.1} d={12.34} e={+1.0} f={-1.0} g={.0} h={.1} i={.12}
dec_f a={1e2} b={1e10} c={1E-10} d={12.34e56} e={12.34e-56} f={.1e10} g={.12e10}
dec_f   h={.12e-10}

; Hex floats (who knew?)
hex_f a={0x0.0} b={0X1.0} c={0x1.1} d={0x12.34} e={0xab.cd} f={+0x1.0}
hex_f   h={-0x1.0}
hex_f a={0x1p10} b={0x1P-10} c={0x12.34p56} d={0x12.34p-56}
hex_f a={0xab.cdp10} b={0xab.cdp-56} c={0xAb.CdP10}

; Special floats
spec_f a={nan} b={NAN} c={NaN}
spec_f a={inf} b={INF} c={InF}
spec_f a={-inf} b={-INF} c={-InF}
spec_f a={+inf} b={+INF} d={+InF}

; Booleans
bool a={true} b={TRUE} c={tRuE}
bool a={false} b={FALSE} c={fAlSe}

; Strings
str a={"x"} b={"xy"}
str a={"foo\tbar"} b={"foo\\bar"}
str a={"\101"} b={"\x42"}
str a={"\u0041"} b={"✈"} c={"\u2708"}
str a={"\U00000061"} b={"🍺"} c={"\U0001f37a"}


; Bridge

bridge a=foo{"bar"} b={"foo"}bar c=foo{"bar"}biz d={"foo"}{"bar"}
bridge a=foo{"bar"}{"biz"}baz b={"foo"}bar{"biz"}
bridge a=foo{1} b={1}bar c=foo{1}biz d={1}{2} e=foo{1}{2}baz f={1}bar{2}
bridge a=foo"bar" b="foo"bar c=foo"bar"biz d="foo""bar"
bridge a=foo"bar""biz"baz b="foo"bar"biz"


; Boolean operators

bool_op a={not true} b={not false} c={not 0} d={not 1} e={not "true"}
bool_op   f={not "false"}
bool_op a={true and true} b={true and false} c={false and true}
bool_op   d={false and false}
bool_op a={true or true} b={true or false} c={false or true} d={false or false}


; Arithmetic operators

arith_op a={1 + 2} b={1.1 + 2.2} c={"1" + "2"} d={true + false}
arith_op a={1 - 2} b={1.1 - 2.2} c={"1" - "2"} d={true - false}
arith_op a={1 * 2} b={1.1 * 2.2} c={"1" * "2"} d={true * false}
arith_op a={1 / 1} b={1.1 / 2.2} c={"1" / "2"} d={true / false}
arith_op a={false / true} b={1 / 0} c={1.1 / 0}
arith_op a={5 % 2} b={"5" % "2"}
arith_op a={2 ** 3} b={2.2 ** 3.3} c={"2" ** "3"}
arith_op a={"2" * 2} b={2 * "2"} c={"2" * 2.0} d={2.0 * "2"}


; Comparison operators

; Integer
int_comp a={1 < 0} b={1 < 1} c={1 < 2}
int_comp a={1 <= 0} b={1 <= 1} c={1 <= 2}
int_comp a={1 == 0} b={1 == 1} c={1 == 2}
int_comp a={1 > 0} b={1 > 1} c={1 > 2}
int_comp a={1 >= 0} b={1 >= 1} c={1 >= 2}

; Float
float_comp a={1.1 < 0.1} b={1.1 < 1.1} c={1.1 < 2.1}
float_comp a={1.1 <= 0.1} b={1.1 <= 1.1} c={1.1 <= 2.1}
float_comp a={1.1 == 0.1} b={1.1 == 1.1} c={1.1 == 2.1}
float_comp a={1.1 > 0.1} b={1.1 > 1.1} c={1.1 > 2.1}
float_comp a={1.1 >= 0.1} b={1.1 >= 1.1} c={1.0 >= 2.1}
float_comp a={inf == inf} b={-inf == inf} c={nan == nan}

; String
str_comp a={"b" < "a"} b={"b" < "b"} c={"b" < "c"}
str_comp a={"b" <= "a"} b={"b" <= "b"} c={"b" <= "c"}
str_comp a={"b" == "a"} b={"b" == "b"} c={"b" == "c"}
str_comp a={"b" > "a"} b={"b" > "b"} c={"b" > "c"}
str_comp a={"b" >= "a"} b={"b" >= "b"} c={"b" >= "c"}

; Boolean
bool_comp a={true < false} b={true < true} c={false < true}
bool_comp a={true <= false} b={true <= true} c={false <= true}
bool_comp a={true == false} b={true == true} c={false == true}
bool_comp a={true > false} b={true < true} c={false < true}
bool_comp a={true >= false} b={true >= true} c={false >= true}


; Casts

cast a={"1" + "2"} b={true + false}
cast a={1 ~ 2} b={1.1 ~ 2.2} c={true ~ false}
cast a={"false" or "false"} b={"false" or "true"}
cast a={0 or 0} b={0 or 1}
cast a={"0" or "0"} b={"0" or "1"} c={"foo" or "bar"}
cast a={1 == 2.1}
cast a={str(1)} b={boolean("false")} c={int("01")} d={float("1.1")}


; Precedence

precedence a={2 + 4 * 6}
precedence b={2 * 4 + 6}
precedence c={2 * (4 + 6)}
precedence d={not false == true}
precedence e={-1 + 2}


; Parameter references, lookup & dict serialization

param a={foo}
param b={foo.bar} c={foo["bar"]}
param a={foo.bar.biz} b={foo["bar"]["biz"]}


; Lexer errors (TODO)


; Misc
ECHO ERRORS FOLLOW ; Delineate where we expect errors in output
{0} ; Expr in first field

; Extended arguments without values
extended foo
extended foo ; error
extended foo; error
extended {"foo"}
extended {"foo"} ; error
extended {"foo"};error
extended foo=
extended foo= ; error
extended foo=; error
extended =foo


; Parser errors (TODO)


; Interpreter errors (TODO)

; Invalid lookups
param a={bar}
param a={foo["BAR"]}
param a={foo.biz}
param a={foo["biz"]}
param a={foo[1]}
param a={"foo".bar}
param a={"foo"["bar"]}


; No newline

goodbye
