# CopAnims

Carry guns the way cops do in GTA IV.

Draw a rifle as the player and the game swaps your whole movement set — `move_player`
becomes `move_rifle`, or `move_f@armed`, or `move_crouch_rifle` when you duck. Every step,
turn and idle now comes out of the rifle set, which is why an armed player walks like
nobody else in the game.

A cop never does that. A cop keeps their own walkstyle and wears the gun as a **partial
animation layered on top**, out of `gun@cops` — four animations, and that's the whole
dictionary:

| animation | when |
|---|---|
| `SWAT_RIFLE` | two-handed weapon |
| `SWAT_RIFLE_CROUCH` | ... ducked |
| `PISTOL_PARTIAL_A` | one-handed, jogging or sprinting |
| `PISTOL_PARTIAL_B` | one-handed, standing or walking |

This mod gives the player the cop's way.

## Which pose a weapon gets

The engine already has a function that decides this — `chooseUpperCombatAnim` — and this
mod **calls it** rather than reimplementing it. Its whole rule is the `2HANDED` flag in
`WeaponInfo.xml`: set it and the weapon gets the rifle pose, clear it and it gets the
pistol pose. Add a weapon, flag it, and it works here with no code change.

Rocket launchers are the exception. They're slot `HEAVY`, so that function refuses them and
`gun@cops` has no launcher animation. `Rpg = 1` gives them a pose anyway — `RpgPose` names
which one, from `gun@cops` or from any other animation set (`move_rpg:Idle`). If the pose
can't play, the vanilla `move_rpg` walk stays rather than leaving you with a launcher and
no animation at all.

## Install

Drop `CopAnims.asi` and `CopAnims.ini` next to `GTAIV.exe`. Needs an ASI loader
(ZolikaPatch, Ultimate ASI Loader, xliveless — whatever you already use).

Works on 1.0.7.0, 1.0.8.0, EFLC 1.1.2.0 and the Complete Edition. Nothing here is pinned to
an address: every function and every structure offset is found by byte signature at load, so
a build that moves one reports it in `CopAnims.log` and that half of the feature stays
vanilla instead of crashing.

## Settings

The two halves are both needed for the effect and both optional:

* `KeepWalkstyle` — rewrites `move_rifle` / `move_f@armed` back to your own walkstyle, and
  `move_crouch_rifle` to plain `move_crouch`. On its own: an armed player walking
  empty-handed.
* `Partial` — plays the gun pose. On its own: a partial pose layered over the full-body
  rifle walk that's already playing.

The pose is held only during plain locomotion. That's read off the animations actually
playing rather than from a list of animation sets, so it follows custom walkstyles and
cancels itself for aiming, jumping, melee, swimming and everything else:

* `OnlyWhileMoving` — only on a locomotion movement set.
* `StopWhenBusy` — drop it while anything else animates the upper body. Turning this off
  causes jitter; the pose and the weapon's own aim/fire/reload animations share one channel
  and fight over it.
* `IncludeCrouch` — crouching and its transitions count as moving.

`ToggleKey` turns the whole effect on and off in game. `Enabled = 0` is the real off
switch — it installs no hooks at all.

`PistolPose` / `PistolPoseJogging` / `PistolPoseSprinting` each take `A`, `B` or `vanilla`.
The defaults are deliberate: **`PISTOL_PARTIAL_A` is authored for jogging and sprinting**
and judders on its own at a stand — that's a fault in the stock animation, not in this mod,
and it's why half the cops in vanilla judder standing around. `B` is the clean
standing/walking one. `vanilla` leaves the engine's per-ped coin flip alone.

## Also available inside TACE-Patch

This is the same code that ships in [TACE-Patch](https://github.com/ClaudeIII/TACE-Patch) as
its `[COPANIMS]` feature. **Run one or the other, not both** — if you use TACE-Patch, set its
`[COPANIMS] Enabled = 0` before adding this .asi.

## Building

Visual Studio 2022, `Release | Win32`. Everything it needs is in `deps/`
([Hooking.Patterns](https://github.com/ThirteenAG/Hooking.Patterns) and
[injector](https://github.com/thelink2012/injector)); output lands in
`build/Release/bin/CopAnims.asi`.
