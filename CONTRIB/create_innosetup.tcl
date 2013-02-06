#!tclsh

# Copyright by Andres Garcia, 2012, and contributed to
# Source Navigator NG under the GPL

# With this script you can create a Inno Setup script file to make
# and Windows installer for Source Navigator
# After compiling and installing it at, say:
#     c:\Temp\SN-4.5
# You put this file at c:\Temp, execute it and a sourcenav.iss file
# will apppear, double click on it to invoke Inno Setup.
# Click on the run icon and Inno Setup will take care of everything.

proc FindFilesDir {dir} {
    global handle

    cd [file tail $dir]

    set ficheros [glob -nocomplain *]
    foreach file $ficheros {
        if {[file isdirectory $file]} continue
        set file [file nativename $dir/$file]
        set source \"$file\"
        regsub SN-4.5 $file {} file
        set file [file nativename [file dirname $file]]
        puts $handle "Source: $source ;	Destdir:\"{app}$file\""
    }

    set directorios  [glob -nocomplain */]

    foreach directory $directorios {
        FindFilesDir $dir/$directory
        cd ..
    }
    return
}

cd [file dir [info script]]
set handle [open sourcenav.iss w]
puts $handle "; -- sourcenav.iss --"
puts $handle "; Script to try and package Source Navigator."

puts $handle "\n\[Setup\]"
puts $handle "AppName=Source Navigator"
puts $handle "AppVerName=Source Navigator NG 4.5"
puts $handle "AppCopyright=Copyright (C) 1997, 1998, 1999, 2000, 2001 Red Hat, Inc - Copyright (c) 2007-2011 Sourcenav NG Development Group"
puts $handle "DefaultDirName={pf}\\Source Navigator NG"
puts $handle "DefaultGroupName=Source Navigator NG 4.5"
#puts $handle "LicenseFile=sourcenav-52b2\\COPYING"
puts $handle "OutputBaseFileName=sourcenav-ng-setup-v4.5"

puts $handle "\n\[Files\]"

FindFilesDir  SN-4.5
cd ..

puts $handle "\n\[Icons\]"
puts $handle "Name: \"{group}\\Source Navigator\"; Filename: \"{app}\\bin\\snavigator.exe\"; Tasks: startmenu ;WorkingDir: \"{app}\\bin\"; Comment: \"Source Navigator\""
puts $handle "Name: \"{group}\\User's Guide\"; Filename: \"{app}\\share\\snavigator\\html\\index.html\"; Tasks: startmenu ;WorkingDir: \"{app}\\html\"; Comment: \"User's guide\""
puts $handle "Name: \"{group}\\Uninstall\"; Filename: \"{app}\\unins000.exe\"; Tasks: startmenu; Comment: \"Uninstall Source Navigator\""
puts $handle "Name: \"{userdesktop}\\Source Navigator NG 4.5\"; Filename: \"{app}\\bin\\snavigator.exe\"; Tasks: desktopicon ;WorkingDir: \"{app}\\bin\""

puts $handle "\n\[Tasks\]"
puts $handle "Name: desktopicon; Description: \"Create a &desktop icon\""
puts $handle "Name: startmenu;   Description: \"Create a &Start Menu entry\""

close $handle

exit
