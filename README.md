Pirateplayer
============

Build instructions
-------------------

Pirateplayer depends upon [Qt4](http://qt.digia.com/), [Qt Desktop Components](http://qt-project.org/wiki/QtDesktopComponents), 
[librtmp](http://rtmpdump.mplayerhq.hu/), 
[libarchive](http://libarchive.github.com/), [libtidy-html5](http://github.com/w3c/tidy-html5), [Ffmpeg](http://ffmpeg.org/) and the [Pirateplay.se API](http://pirateplay.se/api/manual.html).

### Unix/Linux:

1. Run 'qmake' (optionally 'qmake PREFIX=yourdir' for custom prefix [default: /usr/local/]).
2. Run 'make'.
3. Optionally run 'make clean' to remove files created during compilation.
4. Run 'make install' as root to install and you're done! To uninstall: run 'make uninstall' as root.

### OS X:

1. Follow steps one and two from Unix/Linux instructions.
2. Run 'cd build/target'
2. Run 'macdeployqt -dmg pirateplayer.app'
3. You now have an Apple disk image, "pirateplayer.dmg" to install!
