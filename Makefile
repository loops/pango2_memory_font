all: pango2 oldenglish.h script.h


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
