# Native BMCU dashboard card

**English** | [Português brasileiro](pt-BR/Ender3_V3_SE_Mainsail.md)

## Supported versions

The 2.0.1 overlay targets Mainsail **2.19.0**, commit
`5fb9e77fb9f4e60cf0725d9dc7f57cf7b84bbd70`, and the Ender integration with
BMCU-Klipper **1.1.2** / firmware **1.0.4**. It is a native dashboard component,
not an iframe, a second server or a substitute for the official BMCU package.
Complete the [package/firmware maintenance](releases/v2.0.1.md#upgrade-and-rollback)
before installing the card. The installer never flashes either controller.

## Display and editing

Four colored spools represent physical channels 1–4. Material/color are native
BMCU metadata, not proof of filament position or an automatic temperature profile.
State labels distinguish loaded, parked, empty, uncertain and offline channels.
The card reuses Mainsail's theme, dashboard layout, spool icon and Moonraker
connection. It supports English and Brazilian Portuguese and a two-column
mobile layout. If hidden, enable BMCU in the dashboard layout settings.

Select a spool while the printer and BMCU are ready and idle to edit material
and color. Saving calls `BMCU_ENDER_SET_FILAMENT`, which validates the current
channel revision and delegates persistence to the native metadata handler.
It does not heat, move, select tools or reconcile routes. Printing, pause,
active operations, uncertain paths, shutdown and disconnection disable editing.
If a dialog becomes stale, close it and reopen it; do not retry an uncertain
save blindly. Check the console and the channel's current metadata first.

The read-only `bmcu_ender_integration.panel` projection reads current in-memory
state, not BMCU's public snapshot frozen during printing/pauses. Rendering does
not refresh the device or send UART traffic. The Ender module is named
`bmcu_ender_panel.py`; the official `bmcu_panel.py` is preserved.

On runout, insert replacement filament in the indicated channel and use the
normal **Resume** button on Mainsail or the stock display. Native BMCU loading
and Ender purge/wiping finish before printing continues. A failed load stays
paused. See [print recovery](Ender3_V3_SE_Print_Recovery.md). This is not
recovery after a restart or a power failure.

## Install and roll back

After the release is published, obtain `mainsail-bmcu.zip` and `SHA256SUMS`
from the same release as the host source. Finish printing and BMCU operations.
Confirm the actual web-root path from your web-server configuration; the example
below assumes an existing, non-Git Mainsail installation at `~/mainsail`.
Do not point it at a source checkout, filesystem root or unrelated website.

```sh
sha256sum --check SHA256SUMS
python3 scripts/ender3-v3-se/install_mainsail_panel.py \
  --archive mainsail-bmcu.zip --destination "$HOME/mainsail" \
  --sha256 YOUR_ARCHIVE_SHA256
```

Use the checksum for the ZIP from the verified release checksum list. The
installer verifies the pinned version, canonical overlay/build fingerprints,
licenses, every file hash and archive paths before activation. It checks the
local Moonraker's idle state twice, preserves the previous complete web tree,
copies `config.json` and `.theme`, and activates by directory rename. It does
not edit nginx/Moonraker configuration or deploy to another host.

Refresh the browser after installation. The printed rollback record points to
the private backup beside the web root. To restore that exact installation:

```sh
python3 scripts/ender3-v3-se/install_mainsail_panel.py \
  --rollback /absolute/path/to/mainsail-panel-backup-ID/rollback.json
```

Rollback refuses to discard later file/directory changes, including custom
theme/configuration edits. Review those changes instead of deleting them to
force rollback. Keep the matching 2.0.1 source available for this utility.

An ordinary Mainsail updater replaces this custom build and removes the card.
Review automatic updates for this web root; updating the upstream version
requires a new overlay/build verification. The installer does not silently
disable the upstream updater.

## Rebuild and verify

Use Node.js **24.21.0**, npm and Python 3 on Linux. Exporting the exact Git
revision ignores private/untracked source files. The upstream lockfile fixes
dependencies; the archive uses sorted paths, fixed timestamps and a SHA-256
file manifest. Identical packaging inputs produce the same ZIP; this does not
promise byte-identical bundles across different toolchains or operating systems.

```sh
git clone --depth 1 --branch v2.19.0 \
  https://github.com/mainsail-crew/mainsail.git /path/to/mainsail-upstream
python3 scripts/ender3-v3-se/build_mainsail_panel.py \
  --source /path/to/mainsail-upstream --output /path/to/new-build \
  --artifact /path/to/mainsail-bmcu.zip
```

The builder runs upstream/overlay unit tests, source lint, overlay formatting,
TypeScript checking and the production/PWA build, then includes
`Mainsail-LICENSE` and `Ender-Integration-LICENSE`. The release pipeline requires
both firmware and card jobs and verifies both artifacts before publishing.

For local browser inspection, prepare a separate build with `--prepare-only`,
install its dependencies and serve it on loopback. Open
`tests/bmcu-preview.html`; it uses only synthetic state and records metadata
commands locally, without a printer or Moonraker connection. This test entry
is not included in the production ZIP. Hardware loading, purge detachment and
real runout acceptance still require supervised commissioning.
