build:
	gcc -fPIC -shared \
		-I/home/moses/.asdf/installs/erlang/28.1.1/erts-16.1.1/include \
		-I/usr/local/include \
		h3.c \
		-L/usr/local/lib \
		-Wl,-rpath,/usr/local/lib \
		-lh3 \
		-o h3.so