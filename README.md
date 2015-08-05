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
- [ ] format messages (colours, bold etc.)
- [ ] Ruby scripting
- [ ] Aliases
- [x] Channel listing
- [x] Userlist in channel view
- [ ] Rename the client before open-sourcing the repo
- [ ] Logging
- [ ] favourite channels?
- [ ] Document all methods.  (except for the getters and setters, maybe.)
