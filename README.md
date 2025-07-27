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
	•	Flex + Bison: Lexing and parsing
	•	AST + Type System: Internally enforced and readable
	•	Optional LLVM backend: Experimental compiled output
	•	REPL: For interactive prototyping
	•	Executable Output: Compiled .sigil files produce terminal output describing their magical effects

---

🧪 Sample Output

When you compile and execute a Sigil script, it prints an in-universe effect report:

```text
[LEY EXECUTION REPORT]
──────────────────────────────
🪄 Invocation: ignite

Essence Used:       Fire
Bound Parameters:   intensity = 5
Supplied at Runtime: target = dry_brush

Decorative Effects:
- Residue: heat (3s linger)

Expected Outcome:
- Target ignites within 1.5s
- Sustained flame for ~18s
- Risk to nearby flammables within 1m radius
──────────────────────────────
```

🧬 Contributing

This project welcomes developers, storytellers, systems designers, and magic system nerds.

Things to help with:
	•	Extending the BNF grammar
	•	Refining type system rules
	•	Building out the REPL and CLI tooling
	•	Writing example spells with real output
	•	Packaging everything so fans can install sigil locally and play

---

📜 License

TBD — this language is part of a broader narrative fiction project. Use with respect.

---

🌌 Credits

Created by @jbatte47. Conceptualized as a bridge between ritual expression, programming language theory, and narrative design. Sigil is a language both fictional and real.