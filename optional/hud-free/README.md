# Optional HUD-Free v0.6 test build

[Download the BPS](../../docs/downloads/OrangeKid_HUD_Free_Test_v0_6_US10.bps)

Apply to a clean, big-endian Ocarina of Time US 1.0 ROM with MD5 `5bd1fe107bf8106b2ab6650abecd54d6`. Use 8 MB RDRAM. This is the normal-framing Orange Kid variant. Existing v1 downloads remain the defaults.

HUD-Free hides hearts, magic, item/action buttons, counters, minimap, horse/minigame indicators and clock digits during gameplay. **Timers still run even though their digits are hidden.** Targeting, dialogue, pause/equip graphics and interface fades are retained.

The unsafe early-return prototype is superseded. The current implementation executes the original `Interface_Draw` logic and helpers, then rewinds the overlay write pointer for four HUD command spans. It retains CPU-side updates and allocations, rather than treating every draw helper as side-effect-free. It is not a CPU optimization.

v0.6 adds regression coverage and validation documentation. Its ROM is byte-identical to the v0.5 fix; no additional gameplay-code change was warranted by this audit. Its BPS metadata identifies v0.6.

## Validation and limits

- Build, N64 checksum, all 1,510 DMA payloads and BPS byte-for-byte round trip passed.
- Audited the 39 changed code files and four added headers against the original source; 1,669 code files were unchanged. Unchanged boss scripts are source evidence, not a boss-by-boss gameplay test.
- The previous 17 regression executables passed, covering font/name entry, text control boundaries, D-pad restrictions, block collision steps, depth/glow bounds, graphics arenas, room fog, pause restoration and grayscale rendering. Thirteen embedded function snapshots were checked against current source.
- New tests compile the **current complete `Interface_Draw`, `Sram_OpenSave` and `Sram_WriteSave` functions** with 32-bit layouts. Graphics/audio and external I/O dependencies are stubbed. These are host function tests, not console execution.
- Tests cover main/sub countdown and count-up, timer handoff, hazard damage, eight blocking contexts, pause/resume, all three trade-item expiry/revert paths, all four endgame escape timeout/switch cases, marathon cutoff, HUD suppression, targeting retention and fades.
- Save tests cover all three main/backup slots and checksums; 28 dungeon and 10 endgame age/scene combinations; quest/dungeon flags; child/adult forest returns; Zelda-letter/song and minimum-health recovery.
- Mutation tests deliberately reintroduce early return, frozen countdown and missing trade-item reversion. All three are rejected.
- Emulator smoke-check details are recorded in `emulator-validation.json`.

**A complete beginning-to-ending playthrough and MiSTer hardware validation are not established.** These checks reduce specific regression risks; they cannot guarantee every puzzle, boss, scene, cutscene or ending. Back up saves and report the scene, action and reproduction steps for any failure. Do not replace a known-working build solely on the basis of the automated tests.

ROM SHA-256: `90e51eebc151070649aac344a79f00e33313ee04f2a0ecc023dcbaa660639a5a`.

## Run the new regressions

Supply a prepared source tree with Orange Kid and the HUD-Free patch applied. No ROM is distributed here.

```sh
python3 -m venv .venv
. .venv/bin/activate
pip install -r optional/hud-free/tests/requirements.txt
python optional/hud-free/tests/run_regressions.py /path/to/prepared/oot-source \
  --mutations --report regression-results.json
```

Requires GCC capable of producing 32-bit objects. A 32-bit libc is not needed. Each run extracts the function bodies and relevant inventory/save tables from the supplied source, rather than testing a frozen hand copy. The harness uses real game headers with eight-byte alignment for 64-bit fields.

`hud-free.patch` applies to the earlier Orange Kid HUD-Free v0.2 source. It is not a patch against unmodified ZeldaRET source. The full private source package includes the complete patched tree and original build workflow.
