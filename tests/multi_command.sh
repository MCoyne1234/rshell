#!/bin/bash

if [ ! -f "../bin/rshell" ]; then
	echo "Run \`make\` first!"
	exit 1
fi
 
echo "Execute ls -a; echo hello; mkdir test:"
../bin/rshell << EOF
ls -a; echo hello; mkdir test
echo
EOF

echo "Execute ls -a; echo hello && mkdir test || echo world; git status:"
../bin/rshell << EOF
ls -a; echo hello && mkdir test || echo world; git status
echo
EOF

echo "Exuete ls || echo 1 && echo 2:"
../bin/rshell << EOF
ls || echo 1 && echo 2
echo
EOF

echo "Test missing left side command of a binary operator (ls -a && and ls -a ||):"
../bin/rshell << EOF
ls -a &&
ls -a ||
echo
EOF

echo "Test missing right side command of a binary operator (&& ls -a and || ls -a):"
../bin/rshell << EOF
&& ls -a
|| ls -a
echo
EOF

echo "Test nested operator with missing command in the middle (ls -a && && && echo hello):"
../bin/rshell << EOF
ls -a && && && echo hello
echo
EOF

echo "Test missing operator (ls -a & echo yes and ls -a & echo yes):"
echo "[Currently, we treat & and | as illegal operator]"
../bin/rshell << EOF
ls -a & echo yes
ls -a | echo yes
echo
EOF

echo "Test accidently extra operator (ls -a &&& echo yes and ls -a ||| echo yes):"
../bin/rshell << EOF
ls -a &&& echo yes
ls -a ||| echo yes
echo
EOF

exit 0