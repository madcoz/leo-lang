<pre>
Leo bootstrap Compiler(lc)

lc is a bootstrap compiler for leo (LEOpard) programming language.
leo is a static programming language, this language is implemented 
to get a better understanding of how programming language and compiler works.
For now the language and the compiler are still in its infancy.

All source files are distributed under the GNU GPLv2 or later license
found in the LICENSE file.

How to build:
1) Build from source, the binaries will be located in &lt;src&gt;/bin
   make all
   make

2) Clean binaries
   make clean

Usage:
1) Compile source
   lc &lt;options&gt; &lt;filename&gt;

2) Get lc's version
   lc --version
   lc -V
   
3) Display lexical scanning details
   lc -s &lt;filename&gt;
   
4) Display syntax parsing details
   lc -p &lt;filename&gt;
   
5) Display program logging details, &lt;lv&gt; range from 1 to 3
   with the more higher level it is, the more information it contains
   lc -v&lt;lv&gt; &lt;filename&gt;
</pre>