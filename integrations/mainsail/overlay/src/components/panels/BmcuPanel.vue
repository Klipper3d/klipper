<template>
    <panel :icon="mdiMulticast" :title="$t('Panels.BmcuPanel.Headline')" :collapsible="true" card-class="bmcu-panel">
        <v-card-text>
            <div class="d-flex align-center mb-4">
                <v-icon small class="mr-2" :color="panel.online ? 'success' : 'warning'">
                    {{ panel.online ? mdiCheckCircleOutline : mdiLanDisconnect }}
                </v-icon>
                <span>{{ $t(panel.online ? 'Panels.BmcuPanel.Connected' : 'Panels.BmcuPanel.Offline') }}</span>
                <v-spacer />
                <span class="text-caption text--secondary">{{ $t('Panels.BmcuPanel.FourChannels') }}</span>
            </div>
            <v-row dense>
                <v-col v-for="channel in panel.channels" :key="channel.channel" cols="6" sm="3">
                    <button
                        type="button"
                        class="bmcu-spool pa-3"
                        :class="{ 'bmcu-spool-loaded': channel.state === 'loaded' }"
                        :disabled="!panel.canEdit || saving"
                        :aria-label="$t('Panels.BmcuPanel.EditChannel', { channel: channel.channel })"
                        @click="openEditor(channel)">
                        <span class="d-block text-caption text--secondary mb-2">
                            {{ $t('Panels.BmcuPanel.Channel', { channel: channel.channel }) }}
                        </span>
                        <spool-icon :color="channel.color" width="64" height="64" aria-hidden="true" />
                        <strong class="d-block text-body-2 mt-2 text-truncate">
                            {{ channel.material || $t('Panels.BmcuPanel.Unspecified') }}
                        </strong>
                        <span class="d-block text-caption mt-1">{{ stateLabel(channel.state) }}</span>
                    </button>
                </v-col>
            </v-row>
            <v-alert v-if="panel.refillPending" type="warning" text dense class="mt-4 mb-0">
                {{ $t('Panels.BmcuPanel.Refill', { channel: panel.refillChannel }) }}
            </v-alert>
            <p v-else class="text-caption text--secondary mt-4 mb-0">
                {{ $t(panel.canEdit ? 'Panels.BmcuPanel.EditHint' : 'Panels.BmcuPanel.ReadOnly') }}
            </p>
        </v-card-text>

        <v-dialog v-model="dialog" max-width="440" :persistent="saving">
            <v-card>
                <v-card-title>{{ $t('Panels.BmcuPanel.EditChannel', { channel: selectedChannel }) }}</v-card-title>
                <v-card-text>
                    <p>{{ $t('Panels.BmcuPanel.MetadataOnly') }}</p>
                    <v-alert v-if="error" type="error" text dense>{{ error }}</v-alert>
                    <v-alert v-if="dialogChanged" type="warning" text dense>
                        {{ $t('Panels.BmcuPanel.Changed') }}
                    </v-alert>
                    <v-text-field
                        v-model="material"
                        :label="$t('Panels.BmcuPanel.Material')"
                        :hint="$t('Panels.BmcuPanel.MaterialHint')"
                        :disabled="saving || !panel.canEdit"
                        maxlength="32"
                        outlined
                        dense
                        persistent-hint />
                    <v-text-field
                        v-model="color"
                        :label="$t('Panels.BmcuPanel.Color')"
                        :disabled="saving || !panel.canEdit"
                        maxlength="7"
                        outlined
                        dense>
                        <template #append>
                            <input
                                v-model="color"
                                type="color"
                                :disabled="saving || !panel.canEdit"
                                :aria-label="$t('Panels.BmcuPanel.Color')"
                                class="bmcu-color-input" />
                        </template>
                    </v-text-field>
                </v-card-text>
                <v-card-actions>
                    <v-spacer />
                    <v-btn text :disabled="saving" @click="dialog = false">{{ $t('Panels.BmcuPanel.Cancel') }}</v-btn>
                    <v-btn color="primary" :loading="saving" :disabled="!canSave" @click="save">
                        {{ $t('Panels.BmcuPanel.Save') }}
                    </v-btn>
                </v-card-actions>
            </v-card>
        </v-dialog>
    </panel>
</template>

<script lang="ts">
import { Component, Mixins } from 'vue-property-decorator'
import { mdiCheckCircleOutline, mdiLanDisconnect, mdiMulticast } from '@mdi/js'
import BaseMixin from '@/components/mixins/base'
import Panel from '@/components/ui/Panel.vue'
import SpoolIcon from '@/components/ui/SpoolIcon.vue'
import { BmcuChannel, BmcuPanelState, ChannelState, metadataCommand, normalizePanel } from './Bmcu/bmcu'

@Component({ components: { Panel, SpoolIcon } })
export default class BmcuPanel extends Mixins(BaseMixin) {
    mdiCheckCircleOutline = mdiCheckCircleOutline
    mdiLanDisconnect = mdiLanDisconnect
    mdiMulticast = mdiMulticast
    dialog = false
    saving = false
    selectedChannel = 1
    revision = ''
    material = ''
    color = '#808080'
    error = ''

    get panel(): BmcuPanelState {
        return normalizePanel(this.$store.state.printer.bmcu_ender_integration?.panel, this.klipperReadyForGui)
    }

    get dialogChanged(): boolean {
        return (
            this.panel.channels.find((channel) => channel.channel === this.selectedChannel)?.revision !== this.revision
        )
    }

    get canSave(): boolean {
        if (this.saving) return false
        try {
            metadataCommand(this.panel, this.selectedChannel, this.material, this.color, this.revision)
            return true
        } catch {
            return false
        }
    }

    stateLabel(state: ChannelState): string {
        return this.$t(`Panels.BmcuPanel.States.${state}`) as string
    }

    openEditor(channel: BmcuChannel): void {
        if (!this.panel.canEdit || this.saving) return
        this.selectedChannel = channel.channel
        this.revision = channel.revision
        this.material = channel.material
        this.color = channel.color
        this.error = ''
        this.dialog = true
    }

    async save(): Promise<void> {
        if (!this.canSave) return
        const script = metadataCommand(this.panel, this.selectedChannel, this.material, this.color, this.revision)
        this.saving = true
        this.error = ''
        try {
            await this.$socket.emitAndWait('printer.gcode.script', { script })
            this.dialog = false
        } catch {
            this.error = this.$t('Panels.BmcuPanel.SaveFailed') as string
        } finally {
            this.saving = false
        }
    }
}
</script>

<style scoped>
.bmcu-spool {
    width: 100%;
    border: 1px solid currentColor;
    border-radius: 4px;
    color: inherit;
    text-align: center;
}
.bmcu-spool:focus-visible {
    outline: 3px solid currentColor;
    outline-offset: 2px;
}
.bmcu-spool-loaded {
    box-shadow: inset 0 -3px currentColor;
}
.bmcu-color-input {
    width: 28px;
    height: 24px;
    padding: 0;
    border: 0;
    cursor: pointer;
}
</style>
