# 🪄 Sigil

**Sigil** is a fictional programming language embedded within a narrative universe—but also a real-world, executable DSL with a ritual-based programming model and a diegetic narrative engine.

In the world of the story, Sigil scripts alter physical reality through performed incantations. This toolchain makes that system technically coherent and executable—bridging narrative design, compiler theory, and atmospheric world-building.

---

## 🧠 Core Concepts

- **Expression-First**: Sigil is a functional DSL. Every operation—arithmetic, ritual calls, and essence invocations—is an expression.
- **Hierarchical Namespaces**: Core reality-shards are accessed via dot-notation (e.g., `Essence.heat`, `Action.long_press`, `World.ping`).
- **Implicit Currying**: All rituals and built-in functions support partial application. Calling a function with subset arguments returns a curried function for later use.
- **Instance Decoration**: Objects can be embellished with properties that semantically alter their reality at compile-time (e.g., `instance goblin:Enemy().with_clothing('rags')`).
- **Diegetic Runtime**: Instead of technical logs, the toolchain emits atmospheric, MUD-style narrative descriptions to `STDOUT`.

---

## ⚙️ The Toolchain

Sigil uses a classical compiler pipeline tailored for narrative output:
- **Lexer/Parser**: Flex + Bison (Handling indentation, hierarchical namespaces, and recursive expressions).
- **Semantic Validator**: Enforces "Reality Interfaces" (e.g., `Combustible`, `Staff`) and tracks port compatibility.
- **Narrative Engine**: Translates AST nodes into diegetic prose via a spatial lexicon.

### Building
```bash
cd compiler
make
```

### Running
```bash
./sigil_compiler ../examples/fireball_staff.sigil
```

---

## 📜 Example Ritual: `fireball_staff.sigil`

```sigil
import Combustible, Sophont, Enemy, Staff from core;

# 1. Expressive Rituals
incant fireball(target:Combustible, intensity:Factor):
  let fireball = (Essence.heat(intensity=intensity) -> World.sphere(radius=(2 * (intensity / 100))))
  Essence.force(intensity=(intensity / 2)) -> Vector.straight(target=target) -> fireball

# 2. Decorated Instances
instance ash_staff:Staff;
instance goblin:Enemy().with_clothing('rags');

# 3. Enchantment with Currying
imbue into ash_staff:
  let intensity = (Trigger.gesture(action=(Action.long_press(hand=Right, finger=Thumb) + Action.sweep)) -> Func.trigger_intensity)
  find_target(type=Combustible) -> fireball(intensity=intensity)
```

---

## 🎭 Diegetic Output

When executed, Sigil produces a "Reality Manifest":

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
