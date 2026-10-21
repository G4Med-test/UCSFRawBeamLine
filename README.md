# UCSFRawBeamLine

**67.5 MeV proton Bragg curves in water**: energy deposit from a
characterized 67.5 MeV proton pencil beam (UC Davis cyclotron), including
the full beamline (Tantalum scattering foil, 101.6 or 381.0 µm thick) ahead
of the water phantom. Reference: Faddegon B. et al., *Med. Phys.* 42(7),
4199-4210 (2015), Table III.

---

## 🔬 What this test does

The 67.5 MeV proton beam is scattered by a thin Tantalum foil, then the
depth-dose curve (Bragg curve) in a water phantom located 494 cm downstream
(source-to-surface distance) is scored and compared to the experimental
PTW-diode measurements (`ExperimentalData/`, `compare.py`).

## ⚠️ Naming note

The slide item "67.5 MeV proton Bragg curves in water" does not map to the
repository literally named `BraggPeak` in `geant-validation-tests/src`
(that one is actually a **carbon-ion** Bragg-peak benchmark at 62/200/400
MeV/u, unrelated to protons). `UCSFRawBeamLine` is the test that actually
matches the slide's energy, particle and medium (67.5 MeV, proton, water).

## 📚 Reference

- Source: `geant-validation-tests/src/UCSFRawBeamLine`.
- Faddegon B. et al., *Med. Phys.* 42(7), 4199-4210 (2015).

## Import notes

- `main.cc` is `UCSFRaw.cc` (identical duplicate of `UCSFRawBeamLine.cc` in
  the original tree — only one copy is kept here). `include/`, `src/`
  copied as-is.
- `macro/` contains the 4 macros found directly in that source tree
  (`thinFoil.mac`, `thickFoil.mac`, `run_option.mac`, `vis.mac`).
- `validation/parser.py` was **rewritten** for the `metadata()`/`parse()`
  module API that `ci-workflows/validation/export.py` actually calls (the
  first version, copied unmodified from
  `geant-config-generator/tests/geant4/UCSFRawBeamLine/parser.py`, used the
  old `gts.BaseParser` class API and would have failed CI the same way
  `fanoCavity` initially did). See `validation/README.md` for details. The
  vendored `gts` package is no longer needed and was removed.
