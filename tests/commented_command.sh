#!/bin/bash
if [ ! -f "../bin/rshell" ]; then
        echo "Run \`make\` first!"
        exit 1
fi

echo "Execute echo hello #this is a comment"
../bin/rshell << EOF
echo hello #this is a comment
echo
EOF

echo "Execute #echo hello"
../bin/rshell << EOF
#echo hello
echo
EOF

echo "Execute #echo hello || echo hello2; mkdir blabla"
../bin/rshell << EOF
#echo hello || echo hello2; mkdir blabla
echo
EOF

echo "Execute echo hello; echo hello || #echo hello; echo hello && echo hello"
../bin/rshell << EOF
echo hello; echo hello || #echo hello; echo hello && echo hello
echo
EOF

echo "Execute echo hello && echo hello; echo hi && #echo bye"
../bin/rshell << EOF
echo hello && echo hello; echo hi && #echo bye
echo
EOF

echo "Execute ls || echo hello && echo hello #hi && echo hello exit; #exit; echo hello"
../bin/rshell << EOF
ls || echo hello && echo hello #hi && echo hello exit; #exit; echo hello
echo
EOF

exit 0
