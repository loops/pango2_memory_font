all: pango2


.PHONY: pango2
pango2: pango2/out/bin/pango-list

pango2/out/bin/pango-list:
	git submodule init
	git submodule update
	cd pango2 ; meson setup --reconfigure ../out/build
	cd pango2 ; meson configure ../out/build --prefix=$(CURDIR)/out -Dc_link_args='-Wl,-rpath=$(CURDIR)/out/lib64'
	cd out/build  ; ninja install
	

