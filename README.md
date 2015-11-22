# nIRC

A Qt5 IRC client.

## Building nIRC

### Unix and Unix-like

    $ mkdir build && cd build
    $ qmake-qt5 ..
    $ make -j9
    $ ./nIRC

### Windows

Building on Windows requires:

* Visual Studio 2013 (or newer)
* Qt 5.4 (built for Visual Studio 2013)
* PowerShell (yes, really)
* NSIS (optional, for creating an installer)

After installing the build requirements, start PowerShell, navigate to the
`./utils/` directory, and start `./build_windows.ps1` (you might want to check
the directories in the `$dirs` dictionary of that file first).  If you want to
uild an installer, run the `./mknsis.ps1` script after building nIRC.

## TODO list

- [x] Write a TODO list
- [x] make network settings work
- [x] format messages (colours, bold etc.)
- [ ] Scripting (Ruby? ChaiScript? Lisp? Tcl?)
- [x] Aliases
- [ ] Customizable popup menus (similar to KVIrc/mIRC)
- [x] Channel listing
- [x] Userlist in channel view
- [ ] Rename the client before open-sourcing the repo
- [ ] Logging
- [ ] favourite channels?
- [ ] Document all methods.  (except for the getters and setters, maybe.)
- [ ] language switcher?
- [x] fix queries
- [ ] store toolbar positions
- [ ] store subwindow position + size
- [ ] set charset for network
- [ ] highlight notifications
- [x] theming support
- [ ] reverse video formatting (0x16)
- [ ] colour schemes
- [ ] CTCP
- [ ] DCC
- [ ] sort user list correctly
- [ ] Lagometer
- [ ] Connection timer
- [ ] A scripted MP3 player!  (Because we totally need that!)

