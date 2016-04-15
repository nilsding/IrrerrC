# IrrerrC

A Qt5 IRC client.

## Building IrrerrC

### Unix and Unix-like

    $ mkdir build && cd build
    $ qmake-qt5 ..
    $ make -j9
    $ ./IrrerrC

### Windows

Building on Windows requires:

* Visual Studio 2013 (or newer)
* Qt 5.4 (built for Visual Studio 2013)
* PowerShell (yes, really)
* NSIS (optional, for creating an installer)

After installing the build requirements, start PowerShell, navigate to the
`./utils/` directory, and start `./build_windows.ps1` (you might want to check
the directories in the `$dirs` dictionary of that file first).  If you want to
build an installer, run the `./mknsis.ps1` script after building IrrerrC.

## TODO list

- [x] Scripting
- [ ] Customizable popup menus (similar to KVIrc/mIRC, implemented via scripting)
- [ ] Logging
- [ ] favourite channels?
- [ ] Documentation of:
  - [ ] Core methods and members
  - [ ] Scripting functions
- [ ] language switcher
- [ ] store toolbar positions
- [ ] store subwindow position + size
- [ ] set charset for network
- [ ] highlight notifications
- [ ] colour schemes
- [ ] CTCP
- [ ] DCC
- [ ] sort user list correctly
- [ ] Lagometer
- [ ] Connection timer
- [ ] SSL
