# std.tcl --
#
#  Test package for pkg_mkIndex.
#  Does a package require of direct1, whose pkgIndex.tcl entry (in pkg1)
#  should be a -direct entry.
#  This tests that pkg_mkIndex can handle code that is sourced in pkgIndex.tcl
#  files.
#
# Copyright (c) 1998 by Scriptics Corporation.
# All rights reserved.
# 
# RCS: @(#) $Id: std.tcl 144 2003-02-05 10:56:26Z mdejong $

package require direct1

package provide std 1.0

namespace eval std {
    namespace export p1 p2
}

proc std::p1 { stg } {
    return [string tolower $stg]
}

proc std::p2 { stg } {
    return [string toupper $stg]
}
