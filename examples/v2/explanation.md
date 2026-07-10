# Sigil Script Files (v2) Explanation

Terms, symbols, expressions, and syntax details will be called out in order of appearance, using `staff_of_fireball.sigil` as the primary example. Where standard imports occur, all relevant types and functions under that module will be called out, including those found in other local modules. Where local imports occur, focus will shift to the imported file and its contents.

## `body`

`body` is a core module that is provided by the compiler. Its purpose is to enumerate the parts of the body that can be used to cast spells, or be the subject of spells.

### `RightHand`

`RightHand` is a type that is provided by the `body` module. It is a type that represents the right hand of the user. It is a subtype of `Hand`, which is a subtype of `BodyPart`, which is a subtype of `BiologicalObject`, which is a subtype of `Object`.

### `Thumb`

`Thumb` is a type that is provided by the `body` module. It is a type that represents the thumb of the user. It is a subtype of `Finger`, which is a subtype of `BodyPart`, which is a subtype of `BiologicalObject`, which is a subtype of `Object`.

## `manifests`

`manifests` is a keyword that is used in a similar way to `import` in other languages. It is used to declare the types and functions that are available in the current scope from a specific module.

## Keywords and Syntax (v2)

### `incant`

`incant` is a keyword used to declare a named function. Unlike standard functions, incantations in Sigil often describe a sequence of mystical effects or state transformations. Arguments for an `incant` are always enclosed in square brackets `[]`. The return value of an incantation is the value of the last expression in its block.

### Functions as First-Class Citizens

In Sigil, functions (including those declared with `incant`) are first-class citizens and can be passed as arguments to other functions.

#### Function Call Syntax
Function calls in Sigil follow a Clojure-like syntax where expressions are enclosed in parentheses, for example: `(func arg1 arg2)`. At the top level, parentheses are optional.

#### Anonymous Functions
Anonymous functions follow the syntax `([[arg1:Type, arg2:Type], (func ...)])`. For functions with an arity of 1, the square brackets around the arguments can be omitted.

### `actual`

`actual` is a global function that returns the `Actualized` type. It is used to retrieve the "resolved" or "transformed" value of a property or argument, ensuring that atmospheric or vessel-specific modifiers (e.g., skill level, adornment effects) are applied to the base value.

### `hold` and `as`

`hold` is a global function that takes an array as input. The `as` keyword serves as an "alias" operator. Together, they function identically to Clojure's `let` block, allowing for local variable bindings within an incantation or expression.

### `join`

`join` is a global function that takes a spread of arguments. It leverages an internal `Object` function called `merge`. Every implementation that inherits from `Object` can extend or override the `merge` behavior; `join` provides the high-level interface to invoke this merging logic.

### `|` (The Flow Pipe)

The pipe operator `|` is a keyword and special operator used to combine multiple expressions. It binds the input/output ports of subsequent expressions and merges their runtime metadata. Pipes are executed sequentially, and the return value is the result of the last expression in the chain.

### `actualize_number`

`actualize_number` is a key in a dictionary (often passed within `Rune.caveats`). `Rune.caveats` takes an array of such dictionaries. The `Rune` system scans these dictionaries for specific keys like `actualize_number` to determine how to modify numeric values or trigger other specialized logic during execution.

## Built-in Functions and Operations

### `eq`

`eq` is a global function that takes a spread of arguments. It leverages the internal `equals` function of the `Object` type, allowing different types to define their own criteria for equality while providing a unified interface for comparison.

### `get`

A property retrieval function used to access metadata or state from an object or attribute (e.g., `(get adorns rune)`).

### `adorns` and `can_adorn`

These are properties of the `Attribute` base type. `adorns` identifies which `Vessel` an attribute (like a `Rune`) is currently attached to, while `can_adorn` defines the set of types the attribute is permitted to be attached to.

### Numeric Operators (`+`, `-`, `*`, `/`, `%`)

The standard numeric operators are aliases for global functions. They take a spread of arguments and apply the operation sequentially to them (e.g., `(+ 1 2 3)` evaluates to `6`, and `(% 64 30 3)` evaluates to `1`).

### Ternary Operator (`<expr> if <cond> else <expr>`)

Sigil supports an inline ternary operator for conditional value resolution. It evaluates the condition and returns the first expression if true, or the second expression if false (e.g., `2 if (eq (get adorns rune) Wand) else 1`).

### Attribute Application (`:Attribute`)

The leading colon syntax (e.g., `:Rune`) is used to apply an `Attribute` to an object. When an attribute is applied at the top level of a script, it is applied to the global runtime state for that script.

## `core`

`core` is a core module provided by the compiler. It contains a set of types and functions that are commonly used in sigil scripts. 

### `Number`

`Number` is a type that is provided by the `core` module. It is a type that represents a number.

### `Factor`

`Factor` is a type that represents a normalized value or multiplier, often used for intensity or skill-based adjustments.

### `Vector3`

`Vector3` represents a three-component spatial coordinate or direction. Use `straight` to define a linear path toward a target.

### `Distance`

`Distance` is a type representing a spatial measurement or radius.

### `Type`

`Type` is a type that is provided by the `core` module. It is a type that represents a type. It is a subtype of `Object`, and is available as a public property on the `Object` type.

### `Func`

`Func` is a utility namespace containing functional operators like `filter` (to narrow collections) and `first` (to select the primary match).

## `paranormal`

`paranormal` is a core module provided by the compiler. It contains a set of types and functions that are commonly used in sigil scripts. 

### `Gesture`

`Gesture` is a type that is provided by the `paranormal` module. It is a type that represents a gesture. Common members include:
- `long_press`: A sustained interaction with a body part.
- `sweep`: A broad directional movement.

### `Rune`

`Rune` is an attribute type that is provided by the `paranormal` module. It is a type that represents a rune. It is a subtype of `Attribute`, which is a subtype of `Object`. It is used to declare the runic properties of the compiled sigil script. A sigil script can only have one `Rune` attribute. The compiler will throw an error if multiple `Rune` attributes are present.

### `FireSigil`

`FireSigil` is a specific runic constant or type representing the elemental symbol of fire.

### `Essence`

`Essence` is a sub-namespace for core magical energies, including `heat` (thermal energy) and `force` (kinetic energy).

### `Psychic`

`Psychic` is a sub-namespace for mental or extrasensory effects, such as `ping` (locating targets via psychic resonance).

## `vessel`

`vessel` is a core module provided by the compiler. It contains a set of types and functions that are commonly used in sigil scripts. 

### `Staff`

`Staff` is a type that is provided by the `vessel` module. It is a type that represents a staff. It is a subtype of `TwoHandedMelee`, which is a subtype of `Vessel`, which is a subtype of `Object`.

### `Wand`

`Wand` is a type that is provided by the `vessel` module. It is a type that represents a wand. It is a subtype of `OneHandedRanged`, which is a subtype of `Vessel`, which is a subtype of `Object`.

## `reality`

`reality` is a core module provided by the compiler. It contains a set of types and functions that are commonly used in sigil scripts. 

### `Combustible`

`Combustible` is a metatype that is provided by the `reality` module. It is a type that represents a combustible object. It is a subtype of `Targetable`, which is a subtype of `Metatype`,  which is a subtype of `Object`.

### `Enemy`

`Enemy` is a metatype that is provided by the `reality` module. It is a type that represents an enemy. It is a subtype of `Targetable`, which is a subtype of `Metatype`, which is a subtype of `Object`.

### `WorldObject`

`WorldObject` is the base type for all entities physically present in the reality of the simulation. It is a subtype of `Object`.

### `Shape`

`Shape` is a utility for manifesting geometric effects. Use `sphere` to create a three-dimensional volume of effect.

## `local`

`local` is a core module provided by the compiler. It provides the ability to load local sigil scripts as modules. 

# `core` Types Not Explained Above

## `Object`

`Object` is a type that is provided by the compiler. It is a type that represents an object. It is the base type of all types in the sigil language. 

## `BiologicalObject`

`BiologicalObject` is a type that represents an object of biological nature. It is a subtype of `Object`.

## `BodyPart`

`BodyPart` is a type that represents a part of an animal's body. It is a subtype of `BiologicalObject`.

## `Hand` and `Finger`

`Hand` and `Finger` are intermediate base types within the `body` module, categorizing appendages for gesture-based casting.

## `Vessel`, `OneHandedRanged`, and `TwoHandedMelee`

`Vessel` is the base type for magical focus objects. It branches into functional subtypes like `OneHandedRanged` (e.g., `Wand`) and `TwoHandedMelee` (e.g., `Staff`).

## `Metatype` and `Targetable`

`Metatype` is a high-level classification system for objects based on their properties rather than their biological or physical structure. `Targetable` is a specific metatype indicating an object can be the subject of a spell's effect.

## `Attribute`

`Attribute` is the base type for metadata that modifies or defines the properties of scripts, vessels, or objects (e.g., `Rune`). It provides the fundamental logic for traits that can be attached to other objects, including the `adorns` and `can_adorn` properties.
