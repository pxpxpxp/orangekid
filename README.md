# Zelda 64: Orange Kid — Player’s Guide

An illustrated, interactive guide to the native 640×480 Ocarina of Time clarity build targeting the MiSTer N64 94MHz+ core.

Nine chapters include the actual stock-dialogue versus Orange Kid comparison slider, full-scene and enlarged detail views, HUD and menu previews, selected Redux additions, the MiSTer video profile, rupee audio previews, performance notes, and BPS patch downloads.

## Publish on GitHub Pages

1. Upload this package’s contents to a GitHub repository on the `main` branch.
2. Open **Settings → Pages**.
3. Choose **Deploy from a branch**, then **main** and **/docs**, and save.
4. GitHub displays the site URL after deployment finishes.

All asset paths are relative, so the guide works at a repository subpath. `.nojekyll` serves the static HTML directly. No build step or dependencies are required.

## Validate

Run `python3 check_site.py` (requires Pillow) and `node test_interactions.cjs`.

## Redux credits

Ocarina of Time Redux was started by [Maroc](https://github.com/Maroc-rh/Ocarina-of-Time-REDUX), with continuation and documentation by [ShadowOne333](https://github.com/ShadowOne333/Zelda64-Redux-Documentation). Orange Kid includes selected quality-of-life additions from this work.

## Captures and downloads

Dialogue images are actual 640×480 raw emulator framebuffer captures. Original dialogue is rendered within the same high-resolution build for a controlled font comparison. Enlarged crops use nearest-neighbor display. Captures do not exactly reproduce MiSTer’s VI deblur output.

Downloads contain BPS patches, not game ROMs. Applying them requires the matching user-supplied US 1.0 base ROM. The guide lists its required checksum.

This is an unofficial fan project. Original game artwork belongs to its respective rights holders. No new license is assigned to third-party artwork or fonts.
