PRIV_DIR := $(MIX_APP_PATH)/priv
C_SRC := c_src
build:
	gcc -fPIC -shared \
		-I/home/moses/.asdf/installs/erlang/28.1.1/erts-16.1.1/include \
		-I/usr/local/include \
		$(C_SRC)/h3.c \
		-L/usr/local/lib \
		-Wl,-rpath,/usr/local/lib \
		-lh3 \
		-o .$(PRIV_DIR)/h3.so