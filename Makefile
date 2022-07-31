CFLAGS = -Wall -Wextra -std=c11 -Wformat-security

main:
	@rm -fdr out/ 
	mkdir out
	make gasm
	make gm

release:
	@rm -fdr out/
	mkdir out
	make gasm-release
	make gm-release

test: release
	rm -fdr test/gm
	mkdir -p test/gm
	./out/gasm test/gasm/test.gasm test/gm/testgm.gm
	./out/gm test/gm/testgm.gm

gasm: src/gasm.c
	$(CC) $(CFLAGS) -o out/gasm -O0 -ggdb src/gasm.c

gasm-release: src/gasm.c
	$(CC) $(CFLAGS) -o out/gasm -O0 -s src/gasm.c

gm: src/gm.c
	$(CC) $(CFLAGS) -o out/gm -O0 -ggdb src/gm.c

gm-release: src/gm.c
	$(CC) $(CFLAGS) -o out/gm -O2 -s src/gm.c

todo:
	@grep "TODO: " -rn . --exclude="flag.h" --exclude="Makefile" --exclude="TODO.md" --color=always
	

test-gm: test-gasm
	make gm-release
	./out/gm test/gasm/testgasm.bm