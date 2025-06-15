CC      := clang
CFLAGS  := -Wall -Wextra -pedantic -O2
INCDIRS := -I../include/libcrypto -Iutils
UTILDIR := utils
SRCDIR  := ../src

# Test executables
CHA_TEST := chacha20/test_chacha20
SHA_TEST := sha256/test_sha256

.PHONY: all test clean

all: $(CHA_TEST)
# $(SHA_TEST)

$(CHA_TEST): $(SRCDIR)/symmetric/chacha20.c $(UTILDIR)/test_utils.c chacha20/test_chacha20.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCDIRS) \
		$(SRCDIR)/symmetric/chacha20.c $(UTILDIR)/test_utils.c \
		chacha20/test_chacha20.c -o $@

# $(SHA_TEST): $(SRCDIR)/hash/sha256.c $(UTILDIR)/test_utils.c sha256/test_sha256.c
# 	@mkdir -p $(@D)
# 	$(CC) $(CFLAGS) $(INCDIRS) \
# 		$(SRCDIR)/hash/sha256.c $(UTILDIR)/test_utils.c \
# 		sha256/test_sha256.c -o $@
#
test: all
	$(CHA_TEST)
# $(SHA_TEST)

clean:
	rm -f $(CHA_TEST)
# $(SHA_TEST)
