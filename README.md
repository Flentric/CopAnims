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

Heavy weapons are the exception, and they split two ways. `chooseUpperCombatAnim` throws out
everything in the `HEAVY` inventory slot before it looks at any flag, so none of them get a
pose from it.

Those whose data sets `HEAVY_WEAPON_USES_RIFLE_ANIMS` — the grenade launcher, the minigun,
the flamethrower — are the game telling you which family they belong to, so they get
`SWAT_RIFLE` like any other two-hander.

That leaves the actual launchers, which have no `gun@cops` animation at all. `Rpg = 1` gives them a pose anyway — `RpgPose` names
which one, from `gun@cops` or from any other animation set (`move_rpg:Idle`). If the pose
can't play, the vanilla `move_rpg` walk stays rather than leaving you with a launcher and
no animation at all.

## NPCs

`Peds = 1` gives every armed NPC the same treatment. Cops already carry guns this way, and
AI peds get the pose from the engine while they are *in combat* — this is what covers the
armed pedestrian who is simply walking around, which vanilla puts on `move_rifle` exactly
like the player.

Two differences from the player's path, both deliberate:

* **An NPC's walkstyle is only taken off `move_rifle` once its pose is actually up.** There
  can be dozens of them and any number of reasons a pose does not start, and stripping the
  rifle walk without putting the gun pose in its place leaves a ped holding a rifle with its
  arms down — worse than vanilla. The player doesn't wait, because that would add a hitch to
  every draw.
* **Launchers are the player's alone.** An NPC pose comes out of `gun@cops` and nowhere
  else, which is what lets every NPC share a single streaming request no matter how many are
  armed.

### Multiplayer

`Peds` covers other players too, because a remote player is an ordinary ped on your client.
If everyone in the session has the mod, everyone sees everyone carrying guns the cop way.

Nothing is sent over the network to make that happen and nothing needs to be. Each client
works the pose out for itself from the weapon in hand, using the engine's own rule — so the
same weapon lands on the same pose on every machine, without a packet. What has to match is
the `2HANDED` flag in `WeaponInfo.xml`: a client running edited weapon data will see its own
answer, not yours.

A player without the mod just sees vanilla carry. This only ever plays animations, so there
is nothing for them to be out of step *with* — no state, no positions, no hit registration.

Per-ped state lives in a fixed table sized for 96 simultaneous posing peds, walked from the
game's own per-ped update and never allocating. Peds beyond that simply go without, and
entries are reaped when a ped stops ticking — which is the only signal there is that it died
or streamed out.

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
* `StopWhenScripted` — drop it while a mission is animating you. All five of the
  `TASK_PLAY_ANIM` natives — plain, `_NON_INTERRUPTABLE`, `_UPPER_BODY`,
  `_SECONDARY_UPPER_BODY` and `_WITH_ADVANCED_FLAGS` — build the same task internally, so
  one check covers every one of them, plus sequences, cutscene animations and anything
  another mod plays the same way.
* `IncludeCrouch` — crouching and its transitions count as moving.

`ToggleKey` turns the whole effect on and off in game. `Enabled = 0` is the real off
switch — it installs no hooks at all.

`PistolPose` / `PistolPoseJogging` / `PistolPoseSprinting` each take `A`, `B`, `vanilla` or
`none`. `none` turns the cop carry off at that pace only — **the walkstyle goes back with the
pose**, so what you get is the stock armed animations rather than a pistol carried with the
arms down. Set the paces you like and leave the rest vanilla:
`PistolPose = B`, `PistolPoseJogging = none` keeps the cop carry at a stand and a walk and
hands jogging back to the game.
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
