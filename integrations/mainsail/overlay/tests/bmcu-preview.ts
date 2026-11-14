// Local-only browser fixture. This entry is not part of the production build.
import Vue from 'vue'
import Vuex from 'vuex'
import VueI18n from 'vue-i18n'
import Vuetify from 'vuetify'
import 'vuetify/dist/vuetify.min.css'
import BmcuPanel from '../src/components/panels/BmcuPanel.vue'
import en from '../src/locales/en.json'
import pt from '../src/locales/pt.json'

Vue.use(Vuex)
Vue.use(VueI18n)
Vue.use(Vuetify)

const initial = () => ({
    schema: 1,
    package_version: '1.1.2',
    required_firmware: '1.0.4',
    online: true,
    busy: false,
    can_edit: true,
    refill_pending: false,
    refill_channel: -1,
    channels: ['#F5B041', '#3498DB', '#A569BD', '#2ECC71'].map((color, index) => ({
        channel: index + 1,
        color,
        material: ['PLA', 'PETG', 'TPU', 'PETG-CF'][index],
        state: ['parked', 'empty', 'uncertain', 'loaded'][index],
        revision: String(index + 1).repeat(64),
    })),
})
const store = new Vuex.Store({
    state: {
        socket: { isConnected: true },
        server: { klippy_connected: true, klippy_state: 'ready' },
        printer: { bmcu_ender_integration: { panel: initial() } },
    },
    getters: { 'gui/getPanelExpand': () => () => true },
})
const vuetify = new Vuetify({ theme: { dark: true }, icons: { iconfont: 'mdiSvg' } })
const i18n = new VueI18n({ locale: 'en', fallbackLocale: 'en', messages: { en, pt } })
const commands: string[] = []
let failSave = false

Object.defineProperty(Vue.prototype, '$socket', {
    value: {
        async emitAndWait(method: string, params: { script: string }) {
            if (method !== 'printer.gcode.script') throw new Error('Unexpected RPC')
            commands.push(params.script)
            if (failSave) throw new Error('Simulated failure')
            const match = params.script.match(/CHANNEL=(\d) MATERIAL=(\S+) COLOR=(\S+) REVISION=/)
            if (!match) throw new Error('Invalid metadata command')
            const row = store.state.printer.bmcu_ender_integration.panel.channels[Number(match[1]) - 1]
            Object.assign(row, { material: match[2], color: `#${match[3]}`, revision: 'b'.repeat(64) })
            return 'ok'
        },
    },
})

new Vue({
    vuetify,
    i18n,
    store,
    render: (h) => h('v-app', [h('v-main', [h('v-container', { style: { maxWidth: '700px' } }, [h(BmcuPanel)])])]),
}).$mount('#app')

const preview = {
    commands,
    scenario(name: 'ready' | 'offline' | 'busy' | 'refill' | 'uncertain') {
        const panel = initial()
        panel.channels[2].state = name === 'uncertain' ? 'uncertain' : 'parked'
        panel.can_edit = name === 'ready'
        panel.busy = name === 'busy' || name === 'refill'
        panel.refill_pending = name === 'refill'
        panel.refill_channel = name === 'refill' ? 4 : -1
        store.state.socket.isConnected = name !== 'offline'
        store.state.printer.bmcu_ender_integration.panel = panel
    },
    language(locale: 'en' | 'pt') {
        i18n.locale = locale
        document.documentElement.lang = locale === 'pt' ? 'pt-BR' : 'en'
    },
    theme(dark: boolean) {
        vuetify.framework.theme.dark = dark
    },
    failSave(value: boolean) {
        failSave = value
    },
    changeRevision() {
        store.state.printer.bmcu_ender_integration.panel.channels[0].revision = 'c'.repeat(64)
    },
}
declare global {
    interface Window {
        bmcuPreview: typeof preview
    }
}
window.bmcuPreview = preview
preview.scenario('ready')
