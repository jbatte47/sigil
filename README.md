# 🪄 Sigil

**Sigil** is a fictional programming language embedded within a narrative universe—but also a real-world, Turing-complete language with working tooling, readable syntax, and a ritual-based programming model.

In the world of the story, Sigil scripts alter physical reality through performed incantations. In this repo, we aim to make that system technically coherent and executable—bridging narrative design, compiler theory, and symbolic systems.

This repository is both a development environment and a living artifact.

---

## 🧠 Core Concepts

- **Intent-as-Code**: Sigil is declarative, readable, and performable. A novice can understand the text; only a master can execute it effectively.
- **Strongly Typed**: All types are explicit. Type declarations may include constructors (`ctor`) to define instance behavior.
- **Decorators Are Types**: Decorators are implemented as special types marked with `@decorator`, and define `on_call` and/or `on_ctor` methods.
- **Partial Invocation & Vessels**: Any function may be partially compiled into a vessel (animate or inanimate), which can later be activated with the remaining arguments.
- **Output-Driven Runtime**: All compilation and REPL activity emits detailed, in-universe magical descriptions to `STDOUT`.

---

## 📦 Project Structure

```plaintext
.
├── grammar/               # BNF definitions for parsing Sigil
├── compiler/              # Core parser/compiler implementation
├── runtime/               # REPL, evaluator, execution reporting
├── examples/              # Sample Sigil programs and vessels
├── tests/                 # Integration tests for spell behavior
└── README.md
```

---

## ⚙️ Toolchain Goals

Sigil is being built with a classical compiler pipeline in mind:
- Flex + Bison: Lexing and parsing
- AST + Type System: Internally enforced and readable
- Optional LLVM backend: Experimental compiled output
- REPL: For interactive prototyping
- Executable Output: Compiled .sigil files produce terminal output describing their magical effects

---

Sample Script

In-universe, Sigil scripts are performed by the wielders of magic. In a way, the wielders are the JIT compilers. Scripts written in Sigil are similar to:

```sigil
incant light; given target:Surface:
  invoke essence:Light as charm
  bind charm to target
  yield lumen of target
```

Executing such scripts in our world is simpler:

```bash
$ sigil run light.sigil --target=stone_tablet
```

🧪 Sample Output

When you compile and execute such a Sigil script, it prints an in-universe effect report:

```text
[SIGIL EXECUTION REPORT]
──────────────────────────────
🪄 Invocation: light

Essence Used:       Light
Alias Form:         charm
Target Bound:       stone_tablet (Surface)

Returned:
- Property: lumen of stone_tablet
- Type: Lumen
- Value Projection: visible light field (faint, white-yellow, diffuse)

Runtime Manifestation:
- Projected into wielder’s immediate surroundings
- Will fade within 6–8 seconds unless captured or redirected
- May be referenced in follow-up incantations

──────────────────────────────
```

🧬 Contributing

This project welcomes developers, storytellers, systems designers, and magic system nerds.

Things to help with:
- Extending the BNF grammar
- Refining type system rules
- Building out the REPL and CLI tooling
- Writing example spells with real output
- Packaging everything so fans can install sigil locally and play

---

## 📜 License

Sigil is dual-licensed:

- All compiler code and technical tooling is released under the [MIT License](LICENSES/MIT.txt).
- All narrative, grammar, symbolic, and lore-based content is protected under a [Custom License](LICENSES/SIGIL-CUSTOM.txt).

If you wish to build on Sigil’s fictional components, please contact the author for permission.

---

🌌 Credits

Created by @jbatte47. Conceptualized as a bridge between ritual expression, programming language theory, and narrative design. Sigil is a language both fictional and real.
