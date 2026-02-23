#!/bin/bash

# Test runner for Sigil parser

COMPILER="./sigil_compiler"
TEST_DIR="tests/fragments"
mkdir -p "$TEST_DIR"

passed=0
failed=0

assert_parse() {
    local name=$1
    local code=$2
    local expected_exit=$3
    local fragment_file="$TEST_DIR/$name.sigil"
    
    echo -e "$code" > "$fragment_file"
    
    $COMPILER "$fragment_file" > /dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq $expected_exit ]; then
        echo "PASS: $name"
        passed=$((passed + 1))
    else
        echo "FAIL: $name (expected $expected_exit, got $exit_code)"
        failed=$((failed + 1))
    fi
}

# 1. Basic Declaration
assert_parse "basic_decl" "instance fireball_staff : weapon;" 0

# 2. String Literal
assert_parse "string_literal" "incant test():\n  let msg = \"Hello Sigil\"" 0

# 3. Decorators
assert_parse "decorators" "instance staff : weapon .fire(level=10);" 0

# 4. Named Args (Equals)
assert_parse "named_args_equals" "incant test():\n  cast(spell=fireball, power=10)" 0

# 5. Named Args (Colon - EXPECT FAIL)
assert_parse "named_args_colon" "incant test():\n  cast(spell:fireball)" 1

# 6. Gesture Statement (Full)
assert_parse "gesture_full" "given User.simulate():\n  gesture (target=foe) with staff (power=5);" 0

# 7. Member Access Recursion
assert_parse "member_access" "incant test():\n  let x = player.stats.health" 0

# 8. Primary Expr Dot Gesture
assert_parse "dot_gesture" "incant test():\n  let x = staff.gesture" 0

echo "-------------------"
echo "Tests Passed: $passed"
echo "Tests Failed: $failed"

if [ $failed -gt 0 ]; then
    exit 1
fi
exit 0
