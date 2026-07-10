#!/bin/bash

# Test runner for Sigil parser

COMPILER="./sigil_compiler"
TEST_DIR="tests/fragments"
mkdir -p "$TEST_DIR"

if [ ! -x "$COMPILER" ]; then
    echo "Error: Compiler not found or not executable. Run 'make' first."
    exit 1
fi

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

# 1. Manifest declaration
assert_parse "manifest_decl" "core manifests [Enemy, Creature]" 0

# 2. v2 incant declaration
assert_parse "incant_v2" "incant fireball [target:Combustible, intensity:Factor]\n  target" 0

# 3. Type declaration without metadata
assert_parse "type_decl_plain" "type Enemy" 0

# 4. Type declaration with metadata
assert_parse "type_decl_metadata" "type Enemy ^{hp_base: 50, defense: 3}" 0

# 5. Instance declaration without metadata
assert_parse "instance_decl_plain" "instance goblin :Enemy" 0

# 6. Instance declaration with metadata
assert_parse "instance_decl_metadata" "instance ^{hp: 75, fierce: true} orc_warlord :Enemy" 0

# 7. Instance declaration with override metadata
assert_parse "instance_decl_override" "instance ^~{hp: 999, defense: 10} demigod :Enemy" 0

# 8. Legacy v1 incant syntax should fail
assert_parse "legacy_incant_parens" "incant fireball(target:Combustible):" 1

echo "-------------------"
echo "Tests Passed: $passed"
echo "Tests Failed: $failed"

if [ $failed -gt 0 ]; then
    exit 1
fi
exit 0
