export type ChannelState = 'empty' | 'parked' | 'loaded' | 'uncertain' | 'offline'

export interface BmcuChannel {
    channel: number
    material: string
    color: string
    state: ChannelState
    revision: string
}

export interface BmcuPanelState {
    online: boolean
    canEdit: boolean
    channels: BmcuChannel[]
    refillPending: boolean
    refillChannel: number
}

const states: ChannelState[] = ['empty', 'parked', 'loaded', 'uncertain', 'offline']
const colorPattern = /^#?[0-9A-F]{6}$/i
const materialPattern = /^[A-Z][A-Z0-9+_.-]{0,31}$/
const revisionPattern = /^[0-9a-f]{64}$/

function record(value: unknown): Record<string, unknown> {
    return value !== null && typeof value === 'object' && !Array.isArray(value)
        ? (value as Record<string, unknown>)
        : {}
}

export function normalizePanel(raw: unknown, connected: boolean): BmcuPanelState {
    const source = record(raw)
    const compatible = source.schema === 1 && source.package_version === '1.1.2' && source.required_firmware === '1.0.4'
    const online = connected && compatible && source.online === true
    const rows = Array.isArray(source.channels) ? source.channels.map(record) : []
    const validRows =
        rows.length === 4 && [1, 2, 3, 4].every((id) => rows.filter((row) => row.channel === id).length === 1)
    const channels: BmcuChannel[] = [1, 2, 3, 4].map((channel) => {
        const row = validRows ? (rows.find((item) => item.channel === channel) ?? {}) : {}
        const state = states.includes(row.state as ChannelState) ? (row.state as ChannelState) : 'uncertain'
        const color = typeof row.color === 'string' && colorPattern.test(row.color) ? row.color : '#808080'
        return {
            channel,
            material: typeof row.material === 'string' ? row.material : '',
            color: color.startsWith('#') ? color : `#${color}`,
            state: online ? state : 'offline',
            revision: typeof row.revision === 'string' ? row.revision : '',
        }
    })
    return {
        online,
        canEdit:
            online &&
            validRows &&
            source.can_edit === true &&
            source.busy === false &&
            channels.every((channel) => revisionPattern.test(channel.revision)),
        channels,
        refillPending: online && source.refill_pending === true && [1, 2, 3, 4].includes(Number(source.refill_channel)),
        refillChannel: Number(source.refill_channel),
    }
}

export function metadataCommand(
    panel: BmcuPanelState,
    channel: number,
    material: string,
    color: string,
    expectedRevision?: string
): string {
    const target = panel.channels.find((item) => item.channel === channel)
    if (!panel.canEdit || !target || !revisionPattern.test(target.revision)) throw new Error('unavailable')
    if (expectedRevision !== undefined && expectedRevision !== target.revision) throw new Error('changed')
    const identifier = material.toUpperCase()
    if (!materialPattern.test(identifier)) throw new Error('material')
    if (!colorPattern.test(color)) throw new Error('color')
    const hex = color.replace(/^#/, '').toUpperCase()
    return `BMCU_ENDER_SET_FILAMENT CHANNEL=${channel} MATERIAL=${identifier} COLOR=${hex} REVISION=${target.revision}`
}
