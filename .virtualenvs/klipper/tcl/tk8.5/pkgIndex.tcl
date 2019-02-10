if {[catch {package present Tcl 8.5.0}]} { return }
if {($::tcl_platform(platform) eq "unix") && ([info exists ::env(DISPLAY)]
	|| ([info exists ::argv] && ("-display" in $::argv)))} {
    package ifneeded Tk 8.5.15 [list load [file join $dir .. .. bin libtk8.5.dll] Tk]
} else {
    package ifneeded Tk 8.5.15 [list load [file join $dir .. .. bin tk85.dll] Tk]
}
