# 🪄 Sigil

**Sigil** is a fictional programming language embedded within a narrative universe—but also a real-world, executable DSL with a ritual-based programming model and an evolving runtime.

In the world of the story, Sigil scripts alter physical reality through performed incantations. This toolchain makes that system technically coherent and executable—bridging narrative design, compiler theory, and atmospheric world-building.

---

## 🧠 Core Concepts

- **Expression-First**: Sigil is a functional DSL. Every operation—arithmetic, ritual calls, and essence invocations—is an expression.
- **Hierarchical Namespaces**: Core reality-shards are accessed via dot-notation (e.g., `Essence.heat`, `Action.long_press`, `World.ping`).
- **Prefix-Call Semantics**: Ritual expressions use a Clojure-like prefix-call style in v2 parser forms.
- **Type & Instance Declarations**: Declarative `type` / `instance` forms (including metadata prefixes) are parser-supported in the current grammar.
- **Narrative Runtime (In Progress)**: The parser/compiler path is live today; full diegetic narrative execution is being integrated via the Rust CLI runtime.

---

## ⚙️ The Toolchain

Sigil currently uses a classical compiler pipeline, with narrative execution being integrated in the Rust CLI layer:
- **Lexer/Parser**: Flex + Bison (Handling indentation, hierarchical namespaces, and recursive expressions).
- **Semantic Validation**: Early-stage semantic checks exist; deeper interface/runtime validation is planned.
- **Runtime Layer**: Rust CLI (`sigil`) handles state loading, enchantment flow, and wield simulation.

### Building
```bash
cd compiler
make
```

### Running
```bash
./sigil_compiler ../examples/fireball_staff.sigil
```

## 📊 Feature Status

| Feature | Status | Notes |
| --- | --- | --- |
| `manifests` imports | Implemented | v2 module import syntax is parser-supported. |
| `incant` with bracket params | Implemented | Function declarations use `incant name [arg:Type]`. |
| Pipe execution (`|`) | Implemented | Sequential expression composition in v2 examples. |
| Prefix-call expressions | Implemented | Clojure-style call form is parser-supported. |
| `type` declarations | Implemented | Supports `type Name` and `type Name ^{...}` forms. |
| `instance` declarations | Implemented | Supports `instance name :Type`, `^{...}`, and `^~{...}` metadata forms. |
| Legacy v1 syntax (`import ... from`, `imbue into`, `given`) | Deprecated | Maintained only in historical docs/tests. |
| Full diegetic prose runtime in compiler path | In Progress | Current C compiler entrypoint prints AST; narrative execution is currently centered in Rust CLI flow. |

---

## 📜 Example Ritual: `fireball_staff.sigil`

```sigil
core manifests [ Factor, Type, Distance, Func, Vector3 ]
reality manifests [ Combustible, Enemy, WorldObject, Shape ]
paranormal manifests [ Essence, Psychic ]

incant target_enemy [type:Type, radius:Distance]
  (Psychic.ping (actual radius))
  | (Func.filter (join type Enemy))
  | (Func.first)

incant fireball [target:WorldObject, intensity:Factor]
  (hold [actual_intensity as (actual intensity)])
  | (Essence.heat actual_intensity)
  | (Shape.sphere (/ actual_intensity 2))
  | (Vector3.straight target)
  | (Essence.force (* actual_intensity 2))
```

Canonical v2 examples also live under `examples/v2/`.

---

## 🎭 Runtime Output

Current state:
- `compiler/sigil_compiler` parses and prints AST output for valid scripts.
- `sigil` (Rust CLI) executes stateful enchant/wield flows and emits textual simulation output.

Target state (roadmap):
- Unified diegetic "Reality Manifest" output across the end-to-end execution path.

Illustrative target output:

```text
--- AREA DESCRIPTION ---
The chamber is silent, smelled of ozone and ancient wood.

Resting here is ash_staff, an expertly carved focal point.
Resting here is goblin, a looming threat, clad in rags.

You begin to weave patterns into the ash_staff, binding its form to your will.
A faint glow emerges as shimmering waves of intense heat begins to flow...

You perform a focused gesture with the ash_staff.
A searing bolt of arcane energy erupts from the staff's tip!
The goblins are caught in the shimmering heat and sent tumbling backward by the crushing wall of kinetic pressure.
--- END OF LOG ---
```

---

## 🧬 Project Structure

```plaintext
.
├── grammar/               # BNF definitions and language design
├── compiler/              # Flex, Bison, and C implementation
├── examples/              # Sample "Gold Standard" rituals
└── README.md
```

---

## 📜 License

Sigil is dual-licensed:
- **Technical Tooling**: MIT License.
- **Narrative & Lore**: Protected under a Custom License.

🌌 Created by @jbatte47. A bridge between ritual expression and programming language theory.
