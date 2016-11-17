#!/bin/bash

if [ ! -f "../bin/rshell" ]; then
        echo "Run \`make\` first!"
        exit 1
fi

echo "Execute test -d nonexistant"
../bin/rshell << EOF
test -d nonexistant
echo
EOF

echo "Execute [ -d nonexistant ]"
../bin/rshell << EOF
[ -d nonexistant ]
echo
EOF

echo "Execute test -d commented_command.sh"
../bin/rshell << EOF
test -d commented_command.sh
echo
EOF

echo "Execute [ -d commented_command.sh ]"
../bin/rshell << EOF
[ -d commented_command.sh ]
echo
EOF

echo "Execute mkdir hello, test -d hello"
../bin/rshell << EOF
mkdir hello
test -d hello
echo
EOF

echo "Execute mkdir hello, [ -d hello ]" 
../bin/rshell << EOF
mkdir hello
[ -d hello ]
echo
EOF

echo "Execute mkdir hello, test hello"
../bin/rshell << EOF
mkdir hello
test hello
echo
EOF

echo "Execute mkdir hello, test -e hello"
../bin/rshell << EOF
mkdir hello
test -e hello
echo
EOF

echo "Execute mkdir hello, [ -e hello ]"
../bin/rshell << EOF
mkdir hello
[ -e hello ]
echo
EOF

echo "Execute mkdir hello, [ hello ]"
../bin/rshell << EOF
mkdir hello
[ hello ]
echo
EOF

echo "Execute mkdir hello, test -f hello"
../bin/rshell << EOF
mkdir hello
test -f hello
echo
EOF

echo "Execute mkdir hello, [ -f hello ]"
../bin/rshell << EOF
mkdir hello
[ -f hello ]
echo
EOF

echo "Execute test -f commented_command.sh"
../bin/rshell << EOF
test -f commented_command.sh
echo
EOF

echo "Execute [ -f commented_command.sh ]"
../bin/rshell << EOF
[ -f commented_command.sh ]
echo
EOF

echo "Execute [ ]"
../bin/rshell << EOF
[ ]
echo
EOF

echo "Execute test"
../bin/rshell << EOF
test
echo
EOF

echo "Execute (test -f commented_command.sh) || echo hello"
../bin/rshell << EOF
(test -f commented_command.sh) || echo hello
echo
EOF

echo "Execute [ test -f commented_command.sh ] && echo hello"
../bin/rshell << EOF
(test -f commented_command.sh) || echo hello
echo
EOF

echo "Execute (test commented_command.sh) || echo hello"
../bin/rshell << EOF
(test commented_command.sh) || echo hello
echo
EOF

echo "Execute ( [ commented_command.sh ] ) || echo hello"
../bin/rshell << EOF
( [ commented_command.sh ] ) || echo hello
echo
EOF

exit 0

