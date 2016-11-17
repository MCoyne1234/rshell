#!/bin/bash
if [ ! -f "../bin/rshell" ]; then
        echo "Run \`make\` first!"
        exit 1
fi

echo "Execute (echo a && echo b) || (echo c && echo d)"
../bin/rshell << EOF
(echo a && echo b) || (echo c && echo d)
echo
EOF

echo "Execute (ls && ls -a) || ls"
../bin/rshell << EOF
(ls && ls -a) || ls
echo
EOF

echo "Execute (echo a && echo b) || (echo c)"
../bin/rshell << EOF
(echo a && echo b) || (echo c)
echo
EOF


echo "Execute (((echo a && echo b) || (echo c)"
../bin/rshell << EOF
(((echo a && echo b) || (echo c)
echo
EOF

echo "Execute ((ls) || (echo a && echo b)) && (echo c)"
../bin/rshell << EOF
((ls) || (echo a && echo b)) && (echo c)
echo
EOF

echo "Execute ((ls) || (echo a && echo b)) && (ls)"
../bin/rshell << EOF
((ls) || (echo a && echo b)) && (ls)
echo
EOF

echo "Execute ((ls -a) && (echo a && echo b)) || (ls)"
../bin/rshell << EOF
((ls -a) && (echo a && echo b)) || (ls)
echo
EOF

echo "Execute (ls) && (ls -a) && (exit) || (ls); exit"
../bin/rshell << EOF
(ls) && (ls -a) && (exit) || (ls); exit
echo
EOF

echo "Execute (echo hello))))"
../bin/rshell << EOF
(echo hello))))
echo
EOF

exit 0
