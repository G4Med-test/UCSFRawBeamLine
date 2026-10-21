#!/usr/bin/env python

# UCSFRawBeamLine portal parser: metadata() reads the macro, parse() the
# "h1" depth-dose histogram written to the ROOT file named by the macro's
# own /analysis/setFileName.
#
# Ported from geant-config-generator/tests/geant4/UCSFRawBeamLine/parser.py
# (gts.BaseParser + PyROOT, merging repeated-seed jobs) to the module-level
# metadata()/parse() API expected by ci-workflows/validation/export.py,
# reading ROOT with uproot (no PyROOT on the runner, no job-group merge:
# one macro now means one run, so the original's `/ njobs` rescaling is a
# no-op here and is dropped).
import os

from geantval import getJSON, one_command, single_run

# Beam energy (67.5 MeV) is hardcoded in PrimaryGeneratorAction.cc, not
# configurable from the macro.
BEAM_ENERGY_MEV = 67.5
BEAM_PARTICLE = "proton"


def metadata(commands):
    single_run(commands)
    filename = one_command(commands, "/analysis/setFileName")
    thickness, unit = one_command(commands, "/UCSFRawBeam/Geometry/ScatteringFoil/SetThickness").split()
    return {"TEST": "UCSFRawBeamLine",
            "MODEL": one_command(commands, "/UCSFRawBeam/PhysicsList/AddPhysics"),
            "THICKNESS": thickness, "THICKNESS_UNIT": unit,
            "OUTPUT": filename if filename.endswith(".root") else filename + ".root"}


def read_h1(path):
    import uproot

    with uproot.open(path) as root:
        hist = root["h1"]
        edges = hist.axis().edges(flow=False).tolist()
        values = hist.values(flow=False).tolist()
        errors = hist.errors(flow=False).tolist()
    return edges[:-1], edges[1:], values, errors


def parse(job):
    filename = os.path.join(job["path"], job["OUTPUT"])
    binEdgeLow, binEdgeHigh, binContent, binErrors = read_h1(filename)

    parameters = [{"names": "thickness", "values": "%s %s" % (job["THICKNESS"], job["THICKNESS_UNIT"])}]

    yield getJSON(job, "histogram",
                  mctool_name="GEANT4",
                  mctool_model=job["MODEL"],
                  observableName="energy deposition",
                  targetName="water",
                  beamParticle=BEAM_PARTICLE,
                  beamEnergies=[BEAM_ENERGY_MEV],
                  secondaryParticle="None",
                  title="Energy deposit at depth",
                  xAxisName="water, mm",
                  yAxisName="1/N",
                  binEdgeLow=binEdgeLow,
                  binEdgeHigh=binEdgeHigh,
                  binContent=binContent,
                  parameters=parameters,
                  yStatErrorsPlus=binErrors,
                  yStatErrorsMinus=binErrors)

    ymax = max(binContent)
    normalized = [v / ymax * 100. for v in binContent]
    normalizedErrors = [e / ymax * 100. for e in binErrors]

    yield getJSON(job, "histogram",
                  mctool_name="GEANT4",
                  mctool_model=job["MODEL"],
                  observableName="energy deposition (normalized)",
                  targetName="water",
                  beamParticle=BEAM_PARTICLE,
                  beamEnergies=[BEAM_ENERGY_MEV],
                  secondaryParticle="None",
                  title="Energy deposit at depth",
                  xAxisName="water, mm",
                  yAxisName="normalized dose (%)",
                  binEdgeLow=binEdgeLow,
                  binEdgeHigh=binEdgeHigh,
                  binContent=normalized,
                  parameters=parameters,
                  yStatErrorsPlus=normalizedErrors,
                  yStatErrorsMinus=normalizedErrors)
