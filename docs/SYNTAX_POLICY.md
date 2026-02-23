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

## 2. Alias Strategy: Keyword-Based Forms

Sigil previously supported keyword-based aliases for `Essence` and `Trigger` systems. These are now **deprecated**.

### Deprecated Forms
| Deprecated Syntax | Canonical Equivalent | Status |
| :--- | :--- | :--- |
| `essence:heat(...)` | `Essence.heat(...)` | Deprecated |
| `trigger:gesture(...)` | `Trigger.gesture(...)` | Deprecated (Broken) |

> [!WARNING]
> The `trigger:` alias is currently incompatible with `gesture` due to parser collisions. All users must immediately migrate to `Trigger.gesture()`.

---

## 3. Migration Guidance

When updating existing `.sigil` scripts or contributing to the core library:

1. **Replace** any use of `essence:` with `Essence.`.
2. **Replace** any use of `trigger:` with `Trigger.`.
3. **Ensure** that all named arguments use the `=` operator (e.g., `intensity=100`).

---

## 4. Compatibility Commitment

While the parser may continue to accept `essence:` for a limited time to avoid breaking legacy examples, new features and grammar refinements will assume the dot-notation as the source of truth.

- **Phase 1**: Update all documentation and examples to use canonical syntax.
- **Phase 2**: (Future) Issue compiler warnings for deprecated aliases.
- **Phase 3**: (Future) Remove alias support from the grammar.
