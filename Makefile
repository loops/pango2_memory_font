CFLAGS += $(shell PKG_CONFIG_PATH="$(CURDIR)/out/lib64/pkgconfig" pkg-config --cflags --libs pango2cairo)
CFLAGS += -Wl,-rpath="$(CURDIR)/out/lib64"
CFLAGS += -g -O2 -Wall -Wextra

all: pango2 oldenglish.h script.h hello

hello: hello.c

.PHONY: pango2
pango2: out/bin/pango-list

out/bin/pango-list:
	git submodule init
	git submodule update
	cd pango2 ; meson setup --reconfigure ../out/build
	cd pango2 ; meson configure ../out/build --prefix=$(CURDIR)/out -Dc_link_args='-Wl,-rpath=$(CURDIR)/out/lib64'
	cd out/build  ; ninja install

oldenglish.h: OldeEnglish.ttf
	xxd -i -n oldenglish OldeEnglish.ttf > oldenglish.h
	
script.h: script.ttf
	xxd -i -n script script.ttf > script.h
