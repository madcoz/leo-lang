Leo bootstrap Compiler(lc)

lc is a bootstrap compiler for leo (LEOpard) programming language.
leo is a static programming language, this language is implemented 
to get a better understanding of how programming language and compiler works.
For now the language and the compiler are still in its infancy.

All source files are distributed under the GNU GPLv2 or later license
found in the LICENSE file.

How to build:<br />
1) Build from source, the binaries will be located in <src>/bin<br />
   make all<br />
   make<br />

2) Clean binaries<br />
   make clean<br />

Usage:<br />
1) Get lc's version<br />
   lc --version<br />
   lc -V<br />
   
2) Display lexical scanning details<br />
   lc -s <filename><br />
   
3) Display syntax parsing details<br />
   lc -p <filename><br />
   
4) Display program logging details, <lv> range from 1 to 3
   with the more higher level it is, the more information it contains
   lc -v<lv>