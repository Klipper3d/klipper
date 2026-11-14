import copy
import importlib.util
import unittest

from test_integracao import ROOT, import_module


MODULE = ROOT / 'scripts/ender3-v3-se/migrate_v2.py'
DISPATCH_BENCH = import_module(
    'v2_dispatch_bench', ROOT / 'test/e3v3se_display/test_display.py',
)


class V2MigrationTests(unittest.TestCase):
    def migration(self):
        self.assertTrue(MODULE.is_file(), 'The v2 migrator is not implemented')
        spec = importlib.util.spec_from_file_location('migration_v2', MODULE)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        return module

    def test_config_migrates_api_and_preserves_calibration(self):
        migration = self.migration()
        original = '''[bmcu_integracao_ender]
dispositivo: bmcu0
captura_mm: 23.75
tempo_switch: 120
state_file: /home/pi/private-bmcu-state.json

[gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES]
variable_habilitado: 1
variable_angulo_recolhido: 170.0
variable_angulo_acionado: 10.0
variable_x_aproximacao: 180.0

[gcode_macro CORTADOR_SERVO]
gcode:
  BMCU_ENDER_SERVO POSICAO=RECOLHIDO CONFIRMAR=1

[gcode_macro ENDER_INICIAR_IMPRESSAO]
gcode:
  M140 S{params.TEMP_MESA}
  M104 S{params.TEMP_BICO}

#*# [bltouch]
#*# z_offset = 1.950
#*# [heater_bed]
#*# pid_kp = 66.123
'''
        result = migration.migrate_config(original)
        for expected in (
            '[bmcu_ender_integration]', 'device: bmcu0',
            'capture_mm: 23.75', 'sensor_timeout: 120',
            'state_file: /home/pi/private-bmcu-state.json',
            '[gcode_macro _BMCU_ENDER_CUTTER_SETTINGS]',
            'variable_enabled: 1', 'variable_retracted_angle: 170.0',
            'variable_deployed_angle: 10.0', 'variable_approach_x: 180.0',
            '[gcode_macro CUTTER_SERVO]',
            'BMCU_ENDER_SERVO POSITION=RETRACTED CONFIRM=1',
            '[gcode_macro ENDER_START_PRINT]',
            'params.BED_TEMP', 'params.NOZZLE_TEMP',
            '#*# z_offset = 1.950', '#*# pid_kp = 66.123',
        ):
            self.assertIn(expected, result)
        self.assertEqual(result, migration.migrate_config(result))

    def test_preserves_unknown_sections_comments_and_custom_macros(self):
        migration = self.migration()
        original = '''# Keep my calibration and comments.
[gcode_macro MY_PRINT]
variable_message: "CORTADOR_SERVO is a note, not a command"
gcode:
  CORTADOR_SERVO POSICAO=ACIONADO CONFIRMAR=1
  M117 CARREGAR_CANAL_1

[my_extension]
dispositivo: private-value
'''
        result = migration.migrate_config(original)
        self.assertIn('[gcode_macro MY_PRINT]', result)
        self.assertIn('CUTTER_SERVO POSITION=DEPLOYED CONFIRM=1', result)
        self.assertIn('variable_message: "CORTADOR_SERVO is a note', result)
        self.assertIn('M117 CARREGAR_CANAL_1', result)
        self.assertIn('[my_extension]\ndispositivo: private-value', result)
        self.assertIn('# Keep my calibration and comments.', result)

    def test_rejects_colliding_command_definitions(self):
        migration = self.migration()
        original = '''[gcode_macro CORTADOR_SERVO]
gcode:
  M117 original
[gcode_macro CUTTER_SERVO]
gcode:
  M117 custom
'''
        with self.assertRaisesRegex(ValueError, 'conflict|collision'):
            migration.migrate_config(original)

    def test_rejects_conflicting_configuration_keys(self):
        migration = self.migration()
        original = '''[bmcu_integracao_ender]
captura_mm: 20
capture_mm: 45
'''
        with self.assertRaisesRegex(ValueError, 'conflict|collision'):
            migration.migrate_config(original)

    def test_state_only_migrates_known_endpoint_callbacks(self):
        migration = self.migration()
        original = {
            'endpoints': {
                'extruder': {
                    'toolhead_prepare_macro': 'BMCU_PREPARAR_ENDER',
                    'before_pullback_macro': 'BMCU_LIBERAR_ENDER',
                    'custom': 'CORTADOR_SERVO is personal metadata',
                },
                'other': {'toolhead_prepare_macro': 'MY_CUSTOM_PREPARE'},
            },
            'devices': {'TEST_DEVICE': {'calibration': [1, 2, 3]}},
            'print_session': {'active': False},
        }
        before = copy.deepcopy(original)
        result = migration.migrate_state(original)
        self.assertEqual(before, original)
        self.assertEqual(
            'BMCU_PREPARE_ENDER',
            result['endpoints']['extruder']['toolhead_prepare_macro'],
        )
        self.assertEqual(
            'BMCU_RELEASE_ENDER',
            result['endpoints']['extruder']['before_pullback_macro'],
        )
        self.assertEqual(before['devices'], result['devices'])
        self.assertEqual(before['print_session'], result['print_session'])
        self.assertEqual(
            before['endpoints']['other'], result['endpoints']['other'],
        )
        self.assertEqual(
            before['endpoints']['extruder']['custom'],
            result['endpoints']['extruder']['custom'],
        )
        self.assertEqual(result, migration.migrate_state(result))

    def test_native_callback_migration_is_exact_and_idempotent(self):
        migration = self.migration()
        original = '''def callback(name):
    return name == 'BMCU_LIBERAR_ENDER'
# BMCU_LIBERAR_ENDER is an old callback name.
'''
        result = migration.migrate_callback_source(original)
        self.assertIn("name == 'BMCU_RELEASE_ENDER'", result)
        self.assertIn('# BMCU_LIBERAR_ENDER is an old callback', result)
        self.assertEqual(result, migration.migrate_callback_source(result))

    def test_gcode_preserves_line_endings_comments_and_post_body_options(self):
        migration = self.migration()
        original = (
            '[gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES]\r\n'
            'gcode:\r\n'
            '  # params.CONFIRMAR is an old note.\r\n'
            '  CORTADOR_SERVO POSICAO="ACIONADO" CONFIRMAR=1\r\n'
            'variable_angulo_recolhido: 170.0\r\n'
        )
        result = migration.migrate_config(original)
        self.assertEqual(original.count('\r\n'), result.count('\r\n'))
        self.assertIn('# params.CONFIRMAR is an old note.', result)
        self.assertIn('POSITION="DEPLOYED" CONFIRM=1', result)
        self.assertIn('variable_retracted_angle: 170.0', result)
        self.assertEqual(result, migration.migrate_config(result))

    def test_native_source_preserves_fstrings_and_non_callback_literals(self):
        migration = self.migration()
        original = '''def callback(name):
    print(f"Current callback: {name}")
    return name == 'BMCU_LIBERAR_ENDER'
'''
        result = migration.migrate_callback_source(original)
        self.assertIn('f"Current callback: {name}"', result)
        self.assertIn("name == 'BMCU_RELEASE_ENDER'", result)

    def test_invalid_state_is_rejected_without_mutating_input(self):
        migration = self.migration()
        for original in ([], {'endpoints': []}, {'endpoints': {'e': None}}):
            with self.subTest(original=original):
                before = copy.deepcopy(original)
                with self.assertRaises(ValueError):
                    migration.migrate_state(original)
                self.assertEqual(before, original)

    def test_callback_rollback_preserves_new_routes_and_user_changes(self):
        migration = self.migration()
        original = {'endpoints': {'extruder': {
            'toolhead_prepare_macro': 'BMCU_PREPARAR_ENDER',
            'before_pullback_macro': 'BMCU_LIBERAR_ENDER',
            'custom_setting': 10,
        }}, 'devices': {'TEST_DEVICE': {'route_state': 'EMPTY'}}}
        current = migration.migrate_state(original)
        current['devices']['TEST_DEVICE']['route_state'] = 'PARKED'
        current['endpoints']['extruder']['custom_setting'] = 29
        before = copy.deepcopy(current)
        self.assertTrue(hasattr(migration, 'restore_state_callbacks'))
        result = migration.restore_state_callbacks(current, original)
        self.assertEqual(before, current)
        self.assertEqual(before['devices'], result['devices'])
        self.assertEqual(29, result['endpoints']['extruder']['custom_setting'])
        self.assertEqual(
            'BMCU_PREPARAR_ENDER',
            result['endpoints']['extruder']['toolhead_prepare_macro'],
        )
        self.assertEqual(
            result, migration.restore_state_callbacks(result, original),
        )
        current['endpoints']['extruder']['toolhead_prepare_macro'] = 'MY_CUSTOM'
        self.assertEqual(
            'MY_CUSTOM', migration.restore_state_callbacks(current, original)
            ['endpoints']['extruder']['toolhead_prepare_macro'],
        )

    def test_case_insensitive_legacy_commands_and_keys_are_migrated(self):
        migration = self.migration()
        original = '''[bmcu_integracao_ender]
Captura_MM: 23.75
[gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES]
Variable_Angulo_Recolhido: 170.0
[gcode_macro MY_PRINT]
gcode:
  cortador_servo posicao='recolhido' confirmar=1
  BMCU_SET_ENDPOINT toolhead_prepare_macro=bmcu_preparar_ender
'''
        result = migration.migrate_config(original)
        self.assertIn('capture_mm: 23.75', result)
        self.assertIn('variable_retracted_angle: 170.0', result)
        self.assertIn("CUTTER_SERVO POSITION='RETRACTED' CONFIRM=1", result)
        self.assertIn('toolhead_prepare_macro=BMCU_PREPARE_ENDER', result)

    def test_template_migration_only_changes_known_api_references(self):
        migration = self.migration()
        original = '''[gcode_macro ENDER_INICIAR_IMPRESSAO]
gcode:
  {% if 'temp_mesa' in params %}
    M140 S{params.temp_mesa}
  {% endif %}
  M104 S{params.get("TEMP_BICO", 200)}
  {action_respond_info("CANAL")}
[gcode_macro MY_PRINT]
gcode:
  CORTADOR_SERVO POSICAO=RECOLHIDO ; CONFIRMAR=1 is a note
  BMCU_ENDER_MANUAL ACTION=LOAD CANAL={params.CANAL}
  {% set cutter = printer['gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES'] %}
  {% set z = cutter.z_minimo %}
  M117 {params.CANAL}
'''
        result = migration.migrate_config(original)
        self.assertIn("'BED_TEMP' in params", result)
        self.assertIn('params.BED_TEMP', result)
        self.assertIn('params.get("NOZZLE_TEMP", 200)', result)
        self.assertIn('action_respond_info("CANAL")', result)
        self.assertIn('; CONFIRMAR=1 is a note', result)
        self.assertIn('CHANNEL={params.CANAL}', result)
        self.assertIn('gcode_macro _BMCU_ENDER_CUTTER_SETTINGS', result)
        self.assertIn('cutter.minimum_z', result)

    def test_variable_commands_migrate_exact_macro_and_field_names(self):
        migration = self.migration()
        original = (
            '[gcode_macro MY_CALIBRATION]\ngcode:\n'
            '  SET_GCODE_VARIABLE MACRO=_BMCU_ENDER_CORTADOR_AJUSTES '
            'VARIABLE=angulo_recolhido VALUE=170\n'
            '  SET_GCODE_VARIABLE MACRO=MY_MACRO '
            'VARIABLE=angulo_recolhido VALUE=170\n'
        )
        result = migration.migrate_config(original)
        self.assertIn('MACRO=_BMCU_ENDER_CUTTER_SETTINGS', result)
        self.assertIn('VARIABLE=retracted_angle VALUE=170', result)
        self.assertIn('MACRO=MY_MACRO VARIABLE=angulo_recolhido', result)

    def test_mixed_legacy_and_english_parameters_are_rejected(self):
        migration = self.migration()
        for command in (
            'BMCU_ENDER_MANUAL CANAL=1 CHANNEL=2',
            'CUTTER_SERVO POSICAO=RECOLHIDO POSITION=DEPLOYED',
            'BMCU_ENDER_SERVO CONFIRMAR=1 CONFIRM=0',
            'BMCU_SET_ENDPOINT TOOLHEAD_PREPARE_MACRO=BMCU_PREPARAR_ENDER '
            'toolhead_prepare_macro=MY_CALLBACK',
        ):
            with self.subTest(command=command):
                with self.assertRaisesRegex(ValueError, 'collision|conflict'):
                    migration.migrate_config(
                        '[gcode_macro MY_PRINT]\ngcode:\n  ' + command + '\n'
                    )

    def test_parameter_values_are_not_parsed_as_nested_assignments(self):
        migration = self.migration()
        original = '''[gcode_macro MY_PRINT]
gcode:
  BMCU_ENDER_MANUAL CANAL={params.CANAL|default(1)} NOTE="CANAL=2 CONFIRMAR=1"
  CUTTER_SERVO POSICAO='ACIONADO' CONFIRMAR={1 if params.OK else 0}
'''
        result = migration.migrate_config(original)
        self.assertIn('CHANNEL={params.CANAL|default(1)}', result)
        self.assertIn('NOTE="CANAL=2 CONFIRMAR=1"', result)
        self.assertIn(
            "POSITION='DEPLOYED' CONFIRM={1 if params.OK else 0}", result,
        )

    def test_same_name_overrides_are_not_mistaken_for_rename_collisions(self):
        migration = self.migration()
        original = '''[bmcu_integracao_ender]
captura_mm: 20
captura_mm: 23.75
[my_extension]
custom: first
[my_extension]
custom: second
[bmcu_integracao_ender]
purga_mm: 31
'''
        result = migration.migrate_config(original)
        self.assertIn('capture_mm: 20\ncapture_mm: 23.75', result)
        self.assertEqual(2, result.count('[my_extension]'))
        self.assertEqual(2, result.count('[bmcu_ender_integration]'))

    def test_callback_whitespace_accepted_by_native_contract_is_migrated(self):
        migration = self.migration()
        original = {'endpoints': {'extruder': {
            'toolhead_prepare_macro': '  bmcu_preparar_ender  ',
        }}}
        migrated = migration.migrate_state(original)
        self.assertEqual(
            'BMCU_PREPARE_ENDER',
            migrated['endpoints']['extruder']['toolhead_prepare_macro'],
        )
        self.assertEqual(
            original, migration.restore_state_callbacks(migrated, original),
        )

    def test_inline_gcode_callback_runs_the_real_english_dispatcher(self):
        migration = self.migration()
        original = (
            '[gcode_macro BMCU_PREPARAR_ENDER]\r\n'
            'gcode: BMCU_ENDER_CARREGAR {rawparams}\r\n'
        )
        migrated = migration.migrate_config(original)
        printer = DISPATCH_BENCH.FakePrinter()
        gcode = DISPATCH_BENCH.GCodeDispatch(printer)
        printer.objects['gcode'] = gcode
        cfg = DISPATCH_BENCH.ConfigFileReader().build_fileconfig(
            migrated, '<inline callback>',
        )
        config = DISPATCH_BENCH.ConfigWrapper(
            printer, cfg, {}, 'gcode_macro BMCU_PREPARE_ENDER',
        )
        template_engine = DISPATCH_BENCH.PrinterGCodeMacro(config)
        printer.objects['gcode_macro'] = template_engine
        DISPATCH_BENCH.GCodeMacro(config)
        calls, output = [], []
        gcode.register_command(
            'BMCU_ENDER_LOAD',
            lambda command: calls.append(command.get_command_parameters()),
        )
        gcode.register_output_handler(output.append)
        printer.send_event('klippy:ready')
        gcode.run_script_from_command('BMCU_PREPARE_ENDER CHANNEL=2')
        self.assertEqual([{'CHANNEL': '2'}], calls)
        self.assertFalse(any('Unknown command' in item for item in output))
        self.assertEqual(2, migrated.count('\r\n'))
        self.assertEqual(migrated, migration.migrate_config(migrated))

    def test_known_status_references_migrate_with_the_public_schema(self):
        migration = self.migration()
        original = '''[gcode_macro MY_STATUS]
gcode:
  {% set status = printer['bmcu_integracao_ender'] %}
  {% if status.cortador_incerto %}
    M117 {status.ultimo_corte.resultado}
  {% endif %}
'''
        result = migration.migrate_config(original)
        self.assertIn('status.cutter_uncertain', result)
        self.assertIn('status.last_cut.result', result)
        self.assertEqual(result, migration.migrate_config(result))

    def test_legacy_diagnostic_enum_conditions_require_manual_review(self):
        migration = self.migration()
        predicates = (
            "status.ultimo_corte.resultado == 'interrompido'",
            "'interrompido' != status.ultimo_corte.resultado",
            "status.ultimo_corte.etapa in ['curso_da_lamina']",
            "status.last_cut.result == 'interrompido'",
        )
        for predicate in predicates:
            with self.subTest(predicate=predicate):
                text = ('[gcode_macro STATUS]\ngcode:\n'
                        "  {% set status = "
                        "printer['bmcu_integracao_ender'] %}\n"
                        '  {% if ' + predicate + ' %}\n'
                        '    M117 CUT_FAILED\n  {% endif %}\n')
                with self.assertRaisesRegex(ValueError, 'diagnostic.*enum'):
                    migration.migrate_config(text)

    def test_private_labels_and_current_diagnostic_enums_are_preserved(self):
        migration = self.migration()
        text = '''[gcode_macro STATUS]
gcode:
  {% set status = printer['bmcu_integracao_ender'] %}
  {% set private_label = 'interrompido' %}
  {% if status.ultimo_corte.resultado == 'interrupted' %}
    M117 {private_label}
  {% endif %}
'''
        result = migration.migrate_config(text)
        self.assertIn("private_label = 'interrompido'", result)
        self.assertIn("status.last_cut.result == 'interrupted'", result)

    def render_status_macro(self, text, status):
        printer = DISPATCH_BENCH.FakePrinter()
        printer.objects['gcode'] = DISPATCH_BENCH.GCodeDispatch(printer)
        cfg = DISPATCH_BENCH.ConfigFileReader().build_fileconfig(
            text, '<status migration>',
        )
        config = DISPATCH_BENCH.ConfigWrapper(
            printer, cfg, {}, 'gcode_macro STATUS',
        )
        engine = DISPATCH_BENCH.PrinterGCodeMacro(config)
        printer.objects['gcode_macro'] = engine
        return engine.load_template(config, 'gcode').render({'printer': status})

    def test_chained_status_aliases_fail_before_silent_template_changes(self):
        migration = self.migration()
        for assignment in (
            'state.ultimo_corte', "state['ultimo_corte']", 'state',
            "printer['bmcu_integracao_ender'].ultimo_corte",
        ):
            with self.subTest(assignment=assignment):
                predicate = ("cut.ultimo_corte.resultado == 'interrompido'"
                             if assignment == 'state' else
                             "cut.resultado == 'interrompido'")
                text = ('[gcode_macro STATUS]\ngcode:\n'
                        "  {% set state = printer['bmcu_integracao_ender'] %}\n"
                        '  {% set cut = ' + assignment + ' %}\n'
                        '  {% if ' + predicate + ' %}\n'
                        '    M117 CUT_FAILED\n  {% endif %}\n')
                self.assertIn('M117 CUT_FAILED', self.render_status_macro(
                    text, {'bmcu_integracao_ender': {
                        'ultimo_corte': {'resultado': 'interrompido'},
                    }},
                ))
                with self.assertRaisesRegex(ValueError, 'manual review'):
                    migration.migrate_config(text)

    def test_dotted_status_access_requires_review_with_real_templates(self):
        migration = self.migration()
        for section in ('bmcu_integracao_ender', 'bmcu_ender_integration'):
            with self.subTest(section=section):
                text = ('[gcode_macro STATUS]\ngcode:\n'
                        '  {% if printer.' + section + '.manual_busy %}\n'
                        '    M117 BUSY\n  {% endif %}\n')
                self.assertIn('M117 BUSY', self.render_status_macro(
                    text, {section: {'manual_busy': True}},
                ))
                with self.assertRaisesRegex(ValueError, 'manual review'):
                    migration.migrate_config(text)

    def test_status_alias_names_in_private_literals_are_not_rejected(self):
        migration = self.migration()
        text = '''[gcode_macro STATUS]
gcode:
  {% set state = printer['bmcu_integracao_ender'] %}
  {% set private_label = 'state interrompido printer.bmcu_integracao_ender' %}
  M117 {'set' if state.manual_busy else 'idle'}
  {% if state.ultimo_corte.resultado == 'interrupted' %}
    M117 {private_label}
  {% endif %}
'''
        result = migration.migrate_config(text)
        self.assertIn(
            "'state interrompido printer.bmcu_integracao_ender'", result,
        )
        self.assertIn('M117 state interrompido printer.bmcu_integracao_ender',
                      self.render_status_macro(result, {
                          'bmcu_ender_integration': {
                              'last_cut': {'result': 'interrupted'},
                              'manual_busy': True,
                          },
                      }))

    def test_only_redundant_default_display_shortcuts_are_removed(self):
        migration = self.migration()
        original = '''[e3v3se_display]
language: pt_BR
[e3v3se_display macro load_filament]
label: Load filament
gcode: LOAD_FILAMENT
[e3v3se_display macro unload_filament]
label: Unload filament
gcode:
  UNLOAD_FILAMENT
[e3v3se_display macro my_filament]
label: My custom load
gcode: LOAD_FILAMENT TEMP=220
'''
        result = migration.migrate_config(original)
        self.assertNotIn('[e3v3se_display macro load_filament]', result)
        self.assertNotIn('[e3v3se_display macro unload_filament]', result)
        self.assertIn('[e3v3se_display macro my_filament]', result)
        self.assertIn('gcode: LOAD_FILAMENT TEMP=220', result)
        self.assertIn('language: pt_BR', result)
        self.assertEqual(result, migration.migrate_config(result))


if __name__ == '__main__':
    unittest.main()
