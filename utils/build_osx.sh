#!/bin/sh

utildir=`pwd`
repodir=`pwd`/../
builddir=`pwd`/../build
outdir=`pwd`/../build/output
qtdir=~/Qt/5.6/clang_64/bin

buildapppath=`pwd`/../build/IrrerrC.app
outputapppath=`pwd`/../build/output/IrrerrC.app

qmakeflags="QT+=svg -config release"
makeflags="-j2"
macdeployqtflags="-appstore-compliant -dmg -verbose=2"

# -------------- Functions --------------

require() {
  if ! which "$1" >/dev/null 2>&1; then
    echo "!! $2 was not found."
    printf "   Please install it using \033[1;33m$3\033[0m\n"
    exit 1
  fi
}

require_success() {
  if [ $1 -ne 0 ]; then
    printf "\033[1;31m        Build failed!\033[0m\n"
    exit 1
  fi
  if [ -n "$2" ]; then
    msg "$2" 'green'
  fi
}

msg() {
  color='0'
  case $2 in
    'red')    color='31' ;;
    'green')  color='32' ;;
    'yellow') color='33' ;;
    *) ;;
  esac
  printf "\033[${color};1m>>>\033[0;${color}m $1\033[0m\n"
}

# ---------------- Code ----------------

PATH="$qtdir:$PATH"

require 'clang' 'The XCode toolchain' 'the Mac app store'
for x in 'macdeployqt' 'qmake'; do
  require $x 'Qt 5.6 or newer' 'the installer from http://qt.io'
done

msg 'Preparing build directory' 'green'
if [ -e "$builddir" ]; then
  rm -rf "$builddir"
fi
mkdir -p "$builddir"
cd "$builddir"

msg 'Configuring (qmake)' 'green'
time qmake $qmakeflags $repodir
require_success $? 'Configuration successful.'

msg 'Starting build (make)' 'green'
time make $makeflags
require_success $? 'Build successful.'

msg 'Preparing output directory' 'green'
if [ -e "$outdir" ]; then
  rm -rf "$outdir"
fi
mkdir -p "$outdir"
cd "$outdir"
mv "$buildapppath" "$outputapppath"

msg "Deploying to ${outdir} (macdeployqt)" 'green'
time macdeployqt "$outputapppath" $macdeployqtflags
require_success $? "Deployment successful."

printf "\033[1;32m        Build complete!\033[0m\n"
open "$outdir"

