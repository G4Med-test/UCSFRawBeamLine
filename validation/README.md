# Portal JSON parser

Rewritten from `geant-config-generator/tests/geant4/UCSFRawBeamLine/parser.py`
(old `gts.BaseParser` + PyROOT, merging repeated-seed job groups) for the
module-level `metadata()`/`parse()` API that
`ci-workflows/validation/export.py` actually calls. ROOT is read with
`uproot` (no PyROOT on the runner); there is no job-group merge any more —
one macro means one run, so the original's `/ njobs` rescaling (always `/1`
here) is dropped.

`config.json` selects the two self-contained macros already present in
`geant-validation-tests/src/UCSFRawBeamLine` (`thinFoil.mac`/`thickFoil.mac`,
101.6/381.0 µm Tantalum foil) — no macro generation was needed for this
test. Beam particle/energy (proton, 67.5 MeV) are hardcoded in
`PrimaryGeneratorAction.cc`, not macro-configurable, so `metadata()` doesn't
read them — `parse()` fills them in as constants.

`metadata()` reads the ROOT output path straight from the macro's own
`/analysis/setFileName` (same simplification as `Bremsstrahlung`'s port) —
note the macros themselves name it e.g. `output/thinScatter.mac` (a `.mac`
suffix on a ROOT file, apparently a typo in the original repository); since
that isn't a recognized ROOT extension, `G4AnalysisManager` appends `.root`
to it as-is, giving `output/thinScatter.mac.root`, which this parser
reproduces rather than second-guessing.

Histogram `"h1"` (1000 bins, 0.05-50 mm) is read via `uproot` and yielded
twice, exactly as in the original: once as raw energy deposit vs depth, once
rescaled to a percentage of its own peak value ("normalized dose"). No
asymmetric error option is set on this histogram, so errors are read once
and reused for both `yStatErrorsPlus`/`Minus` (as with the other ported
parsers' ROOT histograms in this organization).
