M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
M18
G1 X=0
G1 X=0 Y=0
✈
🍺
X1 B=7
X1 A=12 B=cd E=123
X1 A=12 B=cd E=123
X1 A=3 B=7 c=5 d=6
X1 A=
X1 A=
X1 A=
X1 a=
X1 a=
X1 a=
EXTENDED FOO=bar BIZ=baz DEE=dum WEE=wum
EXTENDED foobar=bizbaz dee=dum wee=wum
EXTENDED foobar=bizbaz dee=dum wee=wum
EXTENDED FOO=bar BIZ=baz DEE=dum WEE=wum
EXTENDED foobar=bizbaz dee=dum wee=wum
EXTENDED foobar=bizbaz dee=dum wee=wum
EXTENDED FOO=bar BIZ=baz DEE=dum WEE=wum
EXTENDED foobar=bizbaz dee=dum wee=wum
EXTENDED FOO=bar BIZ=baz DEE=dum WEE=wum
EXTENDED foobar=bizbaz dee=dum wee=wum
EXTENDED FOO=bar BIZ=baz DEE=dum WEE=wum
A FOO=bar
M117 *=Anything should be allowed here; there aren't even comments
ECHO *=Although expressions are OK
INT A=0 B=0 C=0 D=0
INT A=10 B=2 C=16 D=8
INT A=1 B=1 C=-1 D=1 E=1 F=10 G=10 H=-10
DEC_F A=0 B=1 C=1.1 D=12.34 E=1 F=-1 G=0 H=0.1 I=0.12
DEC_F A=100 B=1e+10 C=1e-10 D=1.234e+57 E=1.234e-55 F=1e+09 G=1.2e+09
DEC_F H=1.2e-11
HEX_F A=0 B=1 C=1.0625 D=18.2031 E=171.801 F=1
HEX_F H=-1
HEX_F A=1024 B=0.000976562 C=1.31167e+18 D=2.52619e-16
HEX_F A=175924 B=2.38421e-15 C=175924
SPEC_F A=nan B=nan C=nan
SPEC_F A=inf B=inf C=inf
SPEC_F A=-inf B=-inf C=-inf
SPEC_F A=inf B=inf D=inf
BOOL A=true B=true C=true
BOOL A=false B=false C=false
STR A=x B=xy
STR A=foo	bar B=foo\bar
STR A=A B=B
STR A=A B=✈ C=✈
STR A=a B=🍺 C=🍺
BRIDGE A=foobar B=foobar C=foobarbiz D=foobar
BRIDGE A=foobarbizbaz B=foobarbiz
BRIDGE A=foo1 B=1bar C=foo1biz D=12 E=foo12baz F=1bar2
BRIDGE A=foobar B=foobar C=foobarbiz D=foobar
BRIDGE A=foobarbizbaz B=foobarbiz
BOOL_OP A=false B=true C=true D=false E=false
BOOL_OP F=true
BOOL_OP A=true B=false C=false
BOOL_OP D=false
BOOL_OP A=true B=true C=true D=false
ARITH_OP A=3 B=3.3 C=3 D=1
ARITH_OP A=-1 B=-1.1 C=-1 D=1
ARITH_OP A=2 B=2.42 C=2 D=0
ARITH_OP A=1 B=0.5 C=0.5 D=nan
ARITH_OP A=0 B=nan C=nan
ARITH_OP A=1 B=1
ARITH_OP A=8 B=13.4895 C=8
ARITH_OP A=4 B=4 C=4 D=4
INT_COMP A=false B=false C=true
INT_COMP A=false B=true C=true
INT_COMP A=false B=true C=false
INT_COMP A=true B=false C=false
INT_COMP A=true B=true C=false
FLOAT_COMP A=false B=false C=true
FLOAT_COMP A=false B=true C=true
FLOAT_COMP A=false B=true C=false
FLOAT_COMP A=true B=false C=false
FLOAT_COMP A=true B=true C=false
FLOAT_COMP A=true B=false C=false
STR_COMP A=false B=false C=true
STR_COMP A=false B=true C=true
STR_COMP A=false B=true C=false
STR_COMP A=true B=false C=false
STR_COMP A=true B=true C=false
BOOL_COMP A=false B=false C=true
BOOL_COMP A=false B=true C=true
BOOL_COMP A=false B=true C=false
BOOL_COMP A=true B=false C=true
BOOL_COMP A=true B=true C=false
CAST A=3 B=1
CAST A=12 B=1.12.2 C=truefalse
CAST A=false B=true
CAST A=false B=true
CAST A=false B=true C=true
CAST A=false
CAST A=1 B=false C=1 D=1.1
PRECEDENCE A=26
PRECEDENCE B=14
PRECEDENCE C=20
PRECEDENCE D=true
PRECEDENCE E=1
PARAM A=#<dict:foo>
PARAM B=#<dict:bar> C=#<dict:bar>
PARAM A=baz B=baz
ECHO *=ERRORS FOLLOW ; Delineate where we expect errors in output
*** ERROR: Expressions not allowed in command name (line 231:2)
*** ERROR: Expected '=' after parameter name (line 234:14)
*** ERROR: Expected '=' after parameter name (line 235:15)
*** ERROR: Expected '=' after parameter name (line 236:14)
*** ERROR: Expected '=' after parameter name (line 237:18)
*** ERROR: Expected '=' after parameter name (line 238:19)
*** ERROR: Expected '=' after parameter name (line 239:18)
*** ERROR: Expected '=' after parameter name (line 240:15)
*** ERROR: Expected '=' after parameter name (line 241:16)
*** ERROR: Expected '=' after parameter name (line 242:15)
*** ERROR: Expected parameter name before '=' (line 242:15)
*** ERROR: Undefined parameter 'bar'
*** ERROR: Undefined property 'BAR'
*** ERROR: Undefined property 'biz'
*** ERROR: Undefined property 'biz'
*** ERROR: Undefined property '1'
*** ERROR: Undefined property 'bar'
*** ERROR: Undefined property 'bar'
GOODBYE
