# for the autogeneration of source files (lex/yacc)
LEX = ./flex-2.6.4/src/flex -t -d
YACC = yacc -t -d -v 

.PHONY: bin src yacc-msgc
bin: src
	$(MAKE) -C bin

msgc.yy.cpp: msgc.yy.l
	$(LEX) $< >$@

yacc-msgc: msgc.tab.y
	$(YACC) $<
	mv y.tab.c msgc.tab.cpp
	mv y.tab.h msgc.tab.h

msgc.tab.cpp msgc.tab.h: yacc-msgc

src: msgc.yy.cpp msgc.tab.cpp msgc.tab.h

clean:
	rm -f msgc.yy.cpp msgc.tab.cpp msgc.tab.d msgc.tab.h y.output
