# unicodeout.tcl --
#
# This demonstration script shows how you can produce output (in label
# widgets) using many different alphabets.

if {![info exists widgetDemo]} {
    error "This script should be run from the \"widget\" demo."
}

package require Tk

# On Windows, we need to determine whether the font system will render
# right-to-left text.

if {[tk windowingsystem] eq {win32}} {
    set rkey [join {
	HKEY_LOCAL_MACHINE
	SOFTWARE
	Microsoft
	{Windows NT}
	CurrentVersion
	LanguagePack
    } \\]
    set w32langs {}
    if {![catch {package require registry}]} {
	if {[catch {registry values $rkey} w32langs]} {
	    set w32langs {}
	}
    }
}

set w .unicodeout
catch {destroy $w}
toplevel $w
wm title $w "Unicode Label Demonstration"
wm iconname $w "unicodeout"
positionWindow $w

label $w.msg -font $font -wraplength 4i -anchor w -justify left \
	-text "This is a sample of Tk's support for languages that use\
	non-Western character sets.  However, what you will actually see\
	below depends largely on what character sets you have installed,\
	and what you see for characters that are not present varies greatly\
	between platforms as well.  The strings are written in Tcl using\
	UNICODE characters using the \\uXXXX escape so as to do so in a\
	portable fashion."
pack $w.msg -side top

## See Code / Dismiss buttons
set btns [addSeeDismiss $w.buttons $w]
pack $btns -side bottom -fill x

pack [label $w.wait -text "Please wait while loading fonts..." \
	-font {Helvetica 12 italic}]
pack [frame $w.f] -expand 1 -fill both -padx 2m -pady 1m
grid columnconfigure $w.f 1 -weight 1

set i 0
proc addSample {w language args} {
    global font i
    set sample [join $args ""]
    set j [incr i]
    label $w.f.l$j -font $font -text "${language}:" -anchor nw -pady 0
    label $w.f.s$j -font $font -text $sample -anchor nw -width 30 -pady 0
    grid $w.f.l$j $w.f.s$j -sticky ew -pady 0
    grid configure $w.f.l$j -padx 1m
}

# Processing when some characters are missing might take a while, so make
# sure we're displaying something in the meantime...

set oldCursor [$w cget -cursor]
$w conf -cursor watch
update

if {[tk windowingsystem] eq {x11}
    || (([tk windowingsystem] eq {win32}) && ({ARABIC} ni $w32langs))} {
    # Using presentation forms (pre-layouted)
    addSample $w Arabic \
	"\uFE94\uFEF4\uFE91\uFEAE\uFECC\uFEDF\uFE8D " \
	"\uFE94\uFEE4\uFEE0\uFEDC\uFEDF\uFE8D"
} else {
    # Using standard text characters
    addSample $w Arabic \
	"\u0627\u0644\u0643\u0644\u0645\u0629 " \
	"\u0627\u0644\u0639\u0631\u0628\u064A\u0629"
}
addSample $w "Trad. Chinese" "\u4E2D\u570B\u7684\u6F22\u5B57"
addSample $w "Simpl. Chinese" "\u6C49\u8BED"
addSample $w Greek \
	 "\u0395\u03BB\u03BB\u03B7\u03BD\u03B9\u03BA\u03AE " \
	 "\u03B3\u03BB\u03CE\u03C3\u03C3\u03B1"
if {[tk windowingsystem] eq {x11}
    || (([tk windowingsystem] eq {win32}) && ({HEBREW} ni $w32langs))} {
    # Visual order (pre-layouted)
    addSample $w Hebrew \
	"\u05EA\u05D9\u05E8\u05D1\u05E2 " \
	"\u05D1\u05EA\u05DB"
} else {
    # Standard logical order
    addSample $w Hebrew \
	"\u05DB\u05EA\u05D1 " \
	"\u05E2\u05D1\u05E8\u05D9\u05EA"
}
addSample $w Japanese \
	 "\u65E5\u672C\u8A9E\u306E\u3072\u3089\u304C\u306A, " \
	 "\u6F22\u5B57\u3068\u30AB\u30BF\u30AB\u30CA"
addSample $w Korean "\uB300\uD55C\uBBFC\uAD6D\uC758 \uD55C\uAE00"
addSample $w Russian \
	"\u0420\u0443\u0441\u0441\u043A\u0438\u0439 \u044F\u0437\u044B\u043A"

# We're done processing, so change things back to normal running...
destroy $w.wait
$w conf -cursor $oldCursor
