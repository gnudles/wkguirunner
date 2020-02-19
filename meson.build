project('wkguirunner', 'c')
gtkdep = dependency('gtk+-3.0')
webkitdep = dependency('webkit2gtk-4.0', version: '>= 2.24.1')
executable('wkguirunner', 'gui_runner.c', dependencies : [gtkdep,webkitdep], install : true)

