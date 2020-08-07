---
id: keymaps
title: Keymaps & Behaviors
sidebar_label: Keymaps
---

ZMK uses an declarative approach to keymaps, instead of using C code for all keymap configuration.
Right now, ZMK uses the devicetree syntax to declare those keymaps; future work is envisioned for
supporting dynamic loading of declarative keymaps, e.g. over USB Mass Storage or via a custom BLE
service.

:::note
For advanced users looking to implement custom behaviors for their keymaps, this will be possible
in the future by allowing user configs to add to the CMake application built by Zephyr.
:::

## Big Picture

All keyboard definitions (complete boards or shields) include the _default_ keymap for that keyboard,
so ZMK can produce a "stock" firmware for that keyboard without any further modifications. For users
looking to customize their keyboard's behavior, they can copy the stock `.keymap` file into their
user config directory, and customize the keymap to their liking.

## Behaviors

ZMK implements the concept of "behaviors", which can be bound to a certain key position, sensor (encoder),
or layer, to perform certain actions when events occur for that binding (e.g. when a certain key position
is pressed or released, or an encoder triggers a rotation event).

For example, the simplest behavior in ZMK is the "key press" behavior, which responds to key position
(a certain spot on the keyboard), and when that position is pressed, send a keycode to the host, and
when the key position is released, updates the host to notify of the keycode being released.

For the full set of possible behaviors, start at the [Key Press](/docs/behavior/key-press) behavior.

## Layers

Like many mechanical keyboard firmwares, ZMK keymaps are composed of a collection of layers, with a
minimum of at least one layer that is the default, usually near the bottom of the "stack". Each layer
in ZMK contains a set of bindings that bind a certain behavior to a certain key position in that layer.

In addition to the base default layer (which can be changed), certain bound behaviors may also
enable/disable additional layers "on top" of the default layer.

**TODO**: A diagram to help visualize layers

When a key location is pressed/released, the stack of all active layers from "top to bottom" is used,
and the event is sent to the behavior bound at that position in each layer, for it to perform whatever
actions it wants to in reaction to the event. Those behaviors can choose to "handle" the event, and stop
it from being passed to any lower layers, or may choose to "pass it along", and let the next layer
in the stack _also_ get the event.

## Behavior Bindings

Binding a behavior at a certain key position may include up to two extra parameters that are used to
alter the behavior when that specific key position is activated/deactived. For example, when binding
the "key press" (`kp`) behavior at a certain key position, you must specific _which_ keycode should
be used for that key position.

```
&kp A
```

In this case, the `A` is actually a define for the raw HID keycode, to make keymaps easier to read and write.

For example of a binding that uses two parameters, you can see how "mod-tap" (`mt`) is bound:

```
&mt MOD_LSFT D
```

Here, the first parameter is the set of modifiers that should be used for the "hold" behavior, and the second
parameter is the keycode that should be sent when triggering the "tap" behavior.

## Keymap File

A keymap file is composed of several sections, that together make up a valid devicetree file for describing the keymap and its layers.

### Includes

THe devicetree files are actually preprocessed before being finally leveraged by Zephyr. This allows using standard C defines to create meaningful placeholders
for what would otherwise be cryptic integer keycodes, etc. This also allows bringing in _other_ devicetree nodes from separate files.

The top two lines of most keymaps should include:

```
#include <behaviors.dtsi>
#include <dt-bindings/zmk/keys.h>
```

The first defines the nodes for all the available behaviors in ZMK, which will be referenced in the behavior bindings. This is how bindings like `&kp` can reference the key press behavior defined with an anchor name of `kp`.

The second include brings in the defines for all the keycodes (e.g. `A`, `NUM_1`, `M_PLAY`) and the modifiers (e.g. `MOD_LSFT`) used for various behavior bindings.

### Root devicetree Node

ALl the remaining keymap nodes will be nested inside of the root devicetree node, like so:

```devicetree
/ {
    // Everything else goes here!
};
```

### Layers

```
	layers {
		compatible = "zmk,layers";

		default: layer_0 {
			label = "DEFAULT";
// ---------------------------------------------------------------------------------------------------------------------------------
// |  ESC  |  Q  |  W  |  E   |  R   |  T   |                                          |  Y   |  U    |  I    |  O   |   P   |   \  |
// |  TAB  |  A  |  S  |  D   |  F   |  G   |                                          |  H   |  J    |  K    |  L   |   ;   |   '  |
// | SHIFT |  Z  |  X  |  C   |  V   |  B   | L SHIFT | L SHIFT |  | L SHIFT | L SHIFT |  N   |  M    |  ,    |  .   |   /   | CTRL |
//                     | GUI  | DEL  | RET  |  SPACE  |   ESC   |  |   RET   |  SPACE  | TAB  | BSPC  | R-ALT |
			bindings = <
	&kp ESC  &kp Q &kp W &kp E        &kp R        &kp T                                            &kp Y &kp U        &kp I        &kp O   &kp P    &kp BSLH
	&kp TAB  &kp A &kp S &mt MOD_LCTL D &mt MOD_LSFT F &kp G                                            &kp H &mt MOD_LSFT J &mt MOD_LCTL K &kp L   &kp SCLN &kp QUOT
	&kp LSFT &kp Z &kp X &kp C        &kp V        &kp B &kp LSFT &kp LSFT        &kp LSFT &kp LSFT &kp N &kp M        &kp CMMA     &kp DOT &kp FSLH &kp RCTL
	              &kp LGUI &kp DEL       &kp RET &kp SPC &mo 1            &mo 2  &kp SPC  &kp RET &kp BKSP &kp RALT
			>;

			sensor-bindings = <&inc_dec_cp M_VOLU M_VOLD &inc_dec_kp PGUP PGDN>;
		};
    };
```

### Keymap

```
    keymap0: keymap {
		compatible = "zmk,keymap";
		label ="Default Kyria Keymap";
		layers = <&default>;
	};
```

### Chosen Keymap

```
	chosen {
		zmk,keymap = &keymap0;
	};
```

### Complete Example

You can see a complete example if you see the [stock Kyria keymap](https://github.com/zmkfirmware/zmk/blob/main/app/boards/shields/kyria/kyria.keymap).