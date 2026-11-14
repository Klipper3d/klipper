import { describe, expect, it } from 'vitest'
import * as bmcu from '../src/components/panels/Bmcu/bmcu'

const rawPanel = () => ({
    schema: 1,
    package_version: '1.1.2',
    required_firmware: '1.0.4',
    online: true,
    busy: false,
    can_edit: true,
    refill_pending: false,
    refill_channel: -1,
    channels: [1, 2, 3, 4].map((channel) => ({
        channel,
        material: 'PETG',
        color: '#123ABC',
        state: channel === 4 ? 'loaded' : 'parked',
        revision: 'a'.repeat(64),
    })),
})

describe('native BMCU card', () => {
    it('keeps the four physical channels and their actual states', () => {
        expect(typeof bmcu.normalizePanel).toBe('function')
        const panel = bmcu.normalizePanel(rawPanel(), true)
        expect(panel.canEdit).toBe(true)
        expect(panel.channels.map((channel) => channel.channel)).toEqual([1, 2, 3, 4])
        expect(panel.channels[3].state).toBe('loaded')
    })

    it('does not present stale loaded state while disconnected', () => {
        expect(typeof bmcu.normalizePanel).toBe('function')
        const panel = bmcu.normalizePanel(rawPanel(), false)
        expect(panel.canEdit).toBe(false)
        expect(panel.channels.every((channel) => channel.state === 'offline')).toBe(true)
    })

    it.each(['version', 'schema', 'duplicate', 'missing', 'revision', 'busy'])(
        'fails closed for %s data',
        (invalid) => {
            expect(typeof bmcu.normalizePanel).toBe('function')
            const raw = rawPanel()
            if (invalid === 'version') raw.package_version = '1.0.6'
            if (invalid === 'schema') raw.schema = 2
            if (invalid === 'duplicate') raw.channels[1].channel = 1
            if (invalid === 'missing') raw.channels.pop()
            if (invalid === 'revision') raw.channels[0].revision = ''
            if (invalid === 'busy') raw.busy = true
            expect(bmcu.normalizePanel(raw, true).canEdit).toBe(false)
        }
    )

    it('builds only the guarded metadata command, with no route or motion parameters', () => {
        expect(typeof bmcu.normalizePanel).toBe('function')
        expect(typeof bmcu.metadataCommand).toBe('function')
        const panel = bmcu.normalizePanel(rawPanel(), true)
        expect(bmcu.metadataCommand(panel, 2, 'petg-cf', '1122aa')).toBe(
            `BMCU_ENDER_SET_FILAMENT CHANNEL=2 MATERIAL=PETG-CF COLOR=1122AA REVISION=${'a'.repeat(64)}`
        )
    })

    it.each(['PETG\nG28', 'PETG G28', 'PETG;G28', '{action_raise_error(1)}', 'A'.repeat(33)])(
        'rejects unsafe material %s',
        (material) => {
            expect(typeof bmcu.metadataCommand).toBe('function')
            const panel = bmcu.normalizePanel(rawPanel(), true)
            expect(() => bmcu.metadataCommand(panel, 1, material, '#FFFFFF')).toThrow()
        }
    )

    it('rejects invalid colors and stale/offline edit requests', () => {
        expect(typeof bmcu.metadataCommand).toBe('function')
        const panel = bmcu.normalizePanel(rawPanel(), true)
        for (const color of ['#FFFFFF\nG28', '#FFF', '#FFF000FF', 'url(evil)']) {
            expect(() => bmcu.metadataCommand(panel, 1, 'PETG', color)).toThrow()
        }
        expect(() => bmcu.metadataCommand(panel, 0, 'PETG', '#FFFFFF')).toThrow()
        expect(() => bmcu.metadataCommand(bmcu.normalizePanel(rawPanel(), false), 1, 'PETG', '#FFFFFF')).toThrow()
    })
})
