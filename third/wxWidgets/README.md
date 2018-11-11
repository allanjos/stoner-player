Settings used for wxWidgets configuration.

### Linux

#### Clone source tree

    cd <WXWIDGETS_SRC_DIR>

    mkdir trunk; mkdir build

    svn checkout https://svn.wxwidgets.org/svn/wx/wxWidgets/trunk trunk

#### Build

    cd build

    ../trunk/configure --prefix=<WXWIDGETS_SRC_DIR>/dist \
    --enable-static --disable-shared --with-libpng --with-libjpeg --without-libtiff \
    --with-libxm --with-libiconv --with-regex=builtin --disable-stc --enable-htmlhelp \
    --enable-aui --enable-grid --enable-timer --enable-dynlib --enable-dynamicloader \
    --with-gtk=3 --enable-debug --enable-debug_gdb

    make

#### Install

    make install
    
### Windows

#### Build

##### MinGW 32

    SET PATH=%PATH%;C:\MinGW\bin

    cd <wxWidgets_SOURCE_DIR>\build\msw

    mingw32-make -f makefile.gcc USE_XRC=1 SHARED=0 MONOLITHIC=0 BUILD=release UNICODE=1

wxWidgets files to be able to compile a wxWidgets application are in directories:

Headers: <wxWidgets_SOURCE_DIR>\include
Libraries: <wxWidgets_SOURCE_DIR>\lib\gcc_lib.

##### Mingw32, MSys

    ../wxWidgets/configure --prefix=<WXWIDGETS_SRC_DIR>/dist \
     --enable-static --disable-shared --with-libpng --with-libjpeg --without-libtiff \
    --with-libxm --with-libiconv --with-regex=builtin --disable-stc --enable-htmlhelp \
    --enable-aui --enable-grid --enable-timer --enable-dynlib --enable-dynamicloader \
    --enable-debug --enable-debug_gdb --with-expat=builtin

Also used on some projects:

    ../wxWidgets/configure --prefix=<WXWIDGETS_SRC_DIR>/dist --build=x86-winnt-mingw32 --disable-shared --disable-threads --enable-cxx11

    make
    
##### Mingw64, MSys2

    ../wxWidgets/configure --prefix=/c/Pessoal/packages/devel/wxWidgets/dist --build=x86_64-w64-mingw32 --disable-shared --enable-cxx11

#### Install

    make install

