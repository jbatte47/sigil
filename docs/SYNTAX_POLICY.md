# Sigil Syntax Policy: Canonical Calls & Aliases

This document defines the normative standards for ritual calls and core system invocations in Sigil.

## 1. Canonical Syntax: Dot-Notation

The hierarchical namespace form (`Namespace.member()`) is the **canonical and preferred** syntax for all core system interactions and ritual calls.

### Why Dot-Notation?
- **Universal Chaining**: Integrates seamlessly with the flow pipeline (`->`).
- **Namespace Clarity**: Explicitly identifies the domain (e.g., `Essence`, `Trigger`, `World`).
- **Parser Stability**: Avoids the need for specialized keyword tokens and prevents reserved-word collisions.

### Canonical Examples
```sigil
# 1. Core Essence Invocation
Essence.heat(intensity=100)

# 2. Trigger Events
Trigger.gesture(action=Action.sweep)

# 3. Geometric Primitives
World.sphere(radius=50)
```

---

## 2. Removed Aliases: Keyword-Based Forms

Sigil previously supported keyword-based aliases for `Essence` and `Trigger` systems. These have been **removed from the grammar**.

### Removed Forms
| Removed Syntax | Canonical Equivalent | Status |
| :--- | :--- | :--- |
| `essence:heat(...)` | `Essence.heat(...)` | Removed |
| `trigger:gesture(...)` | `Trigger.gesture(...)` | Removed |

> [!IMPORTANT]
> All scripts must use the dot-notation. The `essence:` and `trigger:` keywords no longer match any production in the Sigil grammar.

---

## 3. Migration Guidance

When updating existing `.sigil` scripts or contributing to the core library:

1. **Replace** any use of `essence:` with `Essence.`.
2. **Replace** any use of `trigger:` with `Trigger.`.
3. **Ensure** that all named arguments use the `=` operator (e.g., `intensity=100`).

---

## 4. Stability Commitment

The hierarchical dot-notation is the source of truth for all future grammar and compiler developments.

- **Phase 1**: Removed alias support from the grammar and parser.
- **Phase 2**: Normalizing AST representation to a uniform `NODE_MEMBER_ACCESS` and `NODE_CALL` structure.
