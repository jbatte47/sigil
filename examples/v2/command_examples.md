# Sigil v2 CLI & Command Examples

The `sigil` CLI provides a precompiled REPL and invoker for executing Sigil scripts, managing environmental state, and performing "Level 2" operations (binding compiled scripts to objects, wielding enchanted objects, etc.).

## State Management

Before executing a script that interacts with the world, the environment and user state must be established.

### 1. User Loading
Load user-specific metadata, such as skill levels and mastery.
```bash
$ sigil user load --file grand_wizard.yml
Welcome, Grand Wizard Bob!
```

### 2. Environment Loading
Establish the physical and atmospheric context for the simulation.
```bash
$ sigil env load --file deadly_jungle.yml
Environment "Deadly Jungle" loaded.
```

### 3. NPC Loading
Populate the environment with targetable or interactive entities.
```bash
$ sigil npcs load --file orc_horde.yml
NPC group "Orc Horde" loaded.
```

### 4. Inventory Loading
Equip the user with vessels and items.
```bash
$ sigil inventory load --file wizard_inventory.yml
Jewel "Gem of Cittorak" equipped!
Potion "Grand Healing Potion" equipped!
Two-handed Melee Weapon "Oak Staff" equipped!
```

---

## Level 2 Operations

Level 2 operations involve binding a compiled script (and its associated attributes) to a specific vessel or object.

### Enchanting
Bind a `.sigil` script to an item. The CLI validates the `can_adorn` properties of the script's `Rune`.

```bash
$ sigil enchant --spell "staff_of_fireball.sigil" --item "Oak Staff"

Your mastery of the Field is unmatched! You have produced a perfect enchantment!
Two-handed Magical Melee Weapon "Supreme Oak Staff of Flaming Retribution" equipped!
```

### Wielding
Activate the bound logic of an enchanted vessel. This triggers the `trigger` and `spell` definitions within the script's `Rune`.

```bash
$ sigil wield "Supreme Oak Staff of Flaming Retribution"

An orc was targeted!
A sphere of intense heat rockets toward the target!
The target was incinerated!
```
