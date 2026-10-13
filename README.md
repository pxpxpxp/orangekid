# Zelda 64: Orange Kid

A native **640×480 Ocarina of Time** clarity build targeting the **MiSTer N64 94MHz+ core**. Orange Kid combines crisp text, familiar artwork, selected Redux conveniences, and rendering fixes while keeping the original NTSC **20 FPS** target.

**[Open the player’s guide](https://pxpxpxp.github.io/orangekid/)**

## Download v1

| Version | Framing | BPS patch |
| --- | --- | --- |
| Normal | Original cinematic and Z-targeting bars | [Download normal](https://pxpxpxp.github.io/orangekid/downloads/Zelda_64_Orange_Kid_v1.bps) |
| Fullscreen | Removes visible cinematic and Z-targeting bars; retains cameras, projection, and timing | [Download fullscreen](https://pxpxpxp.github.io/orangekid/downloads/Zelda_64_Orange_Kid_v1_Fullscreen.bps) |

Both variants include the same text, quality-of-life, audio, and rendering changes. Fullscreen can increase graphics work by showing more of the scene.

## Apply the patch

1. Supply a clean, big-endian **Ocarina of Time US 1.0** ROM.
2. Apply your chosen BPS patch with a BPS-compatible patcher.
3. Load the resulting ROM on the **MiSTer N64 94MHz+ core** with **8 MB RDRAM**.
4. Keep a save backup. Start fresh rather than loading an emulator savestate containing older code and assets.

Required base-ROM MD5:

```text
5bd1fe107bf8106b2ab6650abecd54d6
```

The native output is **640×480 NTSC 480i, 4:3**. This repository distributes BPS patches and guide assets; game ROMs are not included.

## What changes

- **Crisp text:** Orange Kid dialogue and plain UI lettering, cleaned file-menu and name-entry glyphs, and point-sampled action labels including Save.
- **Original artwork:** familiar Zelda title logo, area-entry cards, and printed map position names are retained.
- **Selected Redux additions:** D-pad item shortcuts, faster normal English dialogue, and quicker block pushing and pulling.
- **Opening intro skip:** new files start outside Link’s house in Kokiri Forest.
- **Softer rupee pickup:** a dedicated replacement sound; music and other effects retain their original audio.
- **Rendering and performance fixes:** reused pause display lists and grayscale data, fog restoration, graphics-arena checks, and corrected grayscale framebuffer row loading.

D-pad shortcuts require ownership of the item: **Up** uses the Lens of Truth, **Down** uses the ocarina, **Left** toggles Iron Boots, and **Right** toggles Hover Boots. Boots are for adult Link; pressing the same shortcut again restores Kokiri Boots. Normal gameplay restrictions apply.

Orange Kid includes a focused selection of Redux features, not the full Redux feature set.

## MiSTer reference settings

| Setting | Value |
| --- | --- |
| Video Out | Original (VI) |
| VI Colorbits | 24 |
| VI Bilinear | Off |
| VI Deblur | On |
| VI Gamma | Off |
| VI Dedither / Antialias | Original |
| VI Divot / Noisedither | Original |
| Texture Filter | Original |
| Dithering / LOD Textures | Original |
| Error Overlay | Off |

The guide includes nine chapters, native gameplay captures, an interactive dialogue comparison, HUD and menu previews, rupee audio comparisons, and performance notes. The dialogue comparison restores original glyphs and filtering within the same 640×480 build; it is not a stock 320×240 capture. Emulator screenshots do not exactly reproduce MiSTer’s VI deblur output.

## Validation

The final build passed build, checksum, DMA, BPS, and targeted regression checks described in the [test pipeline guide](https://pxpxpxp.github.io/orangekid/downloads/Test_Pipeline_v1.txt). An earlier baseline was user-tested on the MiSTer N64 94MHz+ core. A full hardware playthrough and measured MiSTer FPS comparison of the final fixes have not been established.

For the static website:

```sh
python3 check_site.py
node test_interactions.cjs
```

The Python check requires Pillow. The site is served from `main:/docs` with `.nojekyll`; no site build dependencies are required.

## Redux credits

Ocarina of Time Redux was started by **[Maroc](https://github.com/Maroc-rh/Ocarina-of-Time-REDUX)**, with continuation and documentation by **[ShadowOne333](https://github.com/ShadowOne333/Zelda64-Redux-Documentation)**. Orange Kid integrates selected quality-of-life additions from this work.

This is an unofficial fan project. Original game artwork belongs to its respective rights holders. No new license is assigned to third-party artwork or fonts.
